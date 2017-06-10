#include "sm_all.h"
#ifdef __PROLIN__

#include <xui.h>
#include <stdlib.h>
#include "pax_log.h"

#define REF_AREA
XuiColor create_xui_color(int argb_hex);
#undef REF_AREA
extern char* glCurrencyName;
int settle_title_win_text_color() {return title_text_color();}


#define TITLE_AREA
void *title_product_text_font() {return get_text_font();}
char *title_product_count_text()
{
	static char s_str[32]={0};
	memset(s_str,0,sizeof(s_str));
	sprintf(s_str,"%d",settle_product_count());
	return s_str;
}
int title_product_count_text_color() {return book_title_win_text_color();}
int title_product_count_text_height() {return book_title_win_text_height();}
int title_product_count_text_width() {return XuiTextWidth(title_product_text_font(),title_product_count_text_height(),title_product_count_text());}
int title_product_count_text_x() {return book_title_square_x()+(book_title_square_width()-title_product_count_text_width())/2;}
int title_product_count_text_y() {return (book_title_win_height()-title_product_count_text_height())/2;}

static char *amount_to_format_str_save(int amount)//针对金额做每两位补一个逗号的处理
{
	static char s_amount_format_buf[32]="0";
	static int s_last_amount=0;
	if (s_last_amount!=amount) {
		if (amount<100) {
			sprintf(s_amount_format_buf,"%d",amount);
			return s_amount_format_buf;
		}
		char amount_str[32]={0};sprintf(amount_str,"%d",amount);
		int amount_str_len=strlen(amount_str);
		int first_dot_index=(amount_str_len%2) ? (1) : (2);
		memset(s_amount_format_buf,0,sizeof(s_amount_format_buf));
		int i,format_i;
		for (i=0,format_i=0;i!=amount_str_len;++i,++format_i) {
			if (i==first_dot_index) {
				s_amount_format_buf[format_i++]=',';
				first_dot_index+=2;
			}
			s_amount_format_buf[format_i]=amount_str[i];
		}
		s_last_amount=amount;
	}
	return s_amount_format_buf;
}
static char *amount_to_format_str(int amount)
{
	static char s_amount_format_buf[32]="0";
	
	memset(s_amount_format_buf,0,sizeof(s_amount_format_buf));
	snprintf(s_amount_format_buf,31,"%d",amount);
	return s_amount_format_buf;
}

char *title_product_amount_text()
{
	static char s_str[32]={0};
	memset(s_str,0,sizeof(s_str));
	sprintf(s_str,"TOTAL: %s %s",amount_to_format_str(settle_product_amount()),glCurrencyName);
	ERR("------------------------------amount=%s,%d",s_str,settle_product_amount());
	return s_str;
}
int title_product_amount_text_color() {return book_title_win_text_color();}
int title_product_amount_text_height() {return book_title_win_text_height();}
int title_product_amount_text_width() {return XuiTextWidth(title_product_text_font(),title_product_amount_text_height(),title_product_amount_text());}
int title_product_amount_text_x() {return book_title_win_width()-title_gap()-title_product_amount_text_width();}
int title_product_amount_text_y() {return (book_title_win_height()-title_product_amount_text_height())/2;}

#undef TITLE_AREA



#define SETTLE_WIN_AREA
int settle_title_win_color() {return minipos_title_background_color();}
static XuiWindow *settle_title_win_addr()
{
	static XuiWindow *s_win=NULL;
	if (!s_win) {
		s_win=XuiCreateCanvas(settle_menu_win_addr(),book_title_win_x(),book_title_win_y(),book_title_win_width(),book_title_win_height());
	}
	return s_win;
}
XuiWindow *settle_title_win()
{
	static int first=1;
	if (first) {
		XuiCanvasSetBackground(settle_title_win_addr(),0,0,create_xui_color(settle_title_win_color()));
		first=0;
	}
	return settle_title_win_addr();
}
#undef SETTLE_WIN_AREA


#define TITLE_SQUARE_CONFIG
int settle_title_square_gap() {return 3*pu();}
int settle_title_square_size() {return 2*pu();}
int settle_title_square_width() {return 2*settle_title_square_size()+2*settle_title_square_gap()+title_product_count_text_width();}
int settle_title_square_height() {return 2*settle_title_square_size()+2*settle_title_square_gap()+title_product_count_text_height();}
int settle_title_square_x() {return title_gap();}
int settle_title_square_y() {return (settle_title_win_height()-settle_title_square_height())/2;}
int settle_title_square_color() {return settle_title_win_text_color();}
#undef TITLE_SQUARE_CONFIG

int settle_title_refresh()
{
	XuiClearArea(settle_title_win(),0,0,book_title_win_width(),book_title_win_height());


	{//draw entire square
		XuiCanvasDrawRect(settle_title_win(),0,book_title_win_height()-1,book_title_win_width(),1,
			create_xui_color(book_title_square_color()),0,0);
	}
	//draw product_count
	{
		XuiCanvasDrawText(settle_title_win(),title_product_count_text_x(),title_product_count_text_y(),title_product_count_text_height(),
			title_product_text_font(),0,create_xui_color(title_product_count_text_color()),title_product_count_text());
	}

	{//draw product_count_square
		//XuiCanvasDrawRect(settle_title_win(),settle_title_square_x(),settle_title_square_y(),settle_title_square_width(),settle_title_square_height(),
			//create_xui_color(settle_title_square_color()),0,0);
	}
	
	{//draw product_count_square
		XuiCanvasDrawRect(settle_title_win(),book_title_square_x(),book_title_square_y(),book_title_square_width(),book_title_square_height(),
			create_xui_color(book_title_square_color()),0,0);
	}

	//draw product_amount
	{
		XuiCanvasDrawText(settle_title_win(),title_product_amount_text_x(),title_product_amount_text_y(),title_product_amount_text_height(),
			title_product_text_font(),0,create_xui_color(title_product_amount_text_color()),title_product_amount_text());
	}
	return 0;
}

#endif//__PROLIN__

