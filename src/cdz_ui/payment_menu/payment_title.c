/*------------------------------------------------------------
* FileName: payment_title.c
* Author: lugoo
* Date: 2017-05-22
------------------------------------------------------------*/

#include "sm_all.h"
#ifdef __PROLIN__

#include <xui.h>
#include <stdlib.h>
#include "pax_log.h"

#define REF_AREA
XuiColor create_xui_color(int argb_hex);
#undef REF_AREA
extern char* glCurrencyName;
int payment_title_win_text_color() {return title_text_color();}


#define TITLE_AREA

#undef TITLE_AREA



#define payment_WIN_AREA
int payment_title_win_color() {return minipos_title_background_color();}
static XuiWindow *payment_title_win_addr()
{
	static XuiWindow *s_win=NULL;
	if (!s_win) {
		s_win=XuiCreateCanvas(payment_menu_win_addr(),book_title_win_x(),book_title_win_y(),book_title_win_width(),book_title_win_height());
	}
	return s_win;
}


XuiWindow *payment_title_win()
{
	static int first=1;
	if (first) {
		XuiCanvasSetBackground(payment_title_win_addr(),0,0,create_xui_color(payment_title_win_color()));
		first=0;
	}
	return payment_title_win_addr();
}
#undef payment_WIN_AREA


#define TITLE_SQUARE_CONFIG
int payment_title_square_gap() {return 3*pu();}
int payment_title_square_size() {return 2*pu();}
int payment_title_square_width() {return 2*payment_title_square_size()+2*payment_title_square_gap()+title_product_count_text_width();}
int payment_title_square_height() {return 2*payment_title_square_size()+2*payment_title_square_gap()+title_product_count_text_height();}
int payment_title_square_x() {return title_gap();}
int payment_title_square_y() {return (settle_title_win_height()-settle_title_square_height())/2;}
int payment_title_square_color() {return payment_title_win_text_color();}
#undef TITLE_SQUARE_CONFIG

int payment_title_refresh()
{
	XuiClearArea(payment_title_win(),0,0,book_title_win_width(),book_title_win_height());

   
	//draw product_count
   	{//draw entire square
		XuiCanvasDrawRect(payment_title_win(),0,book_title_win_height()-1,book_title_win_width(),1,
			create_xui_color(book_title_square_color()),0,0);
	}

	{//draw product_count_square
		//XuiCanvasDrawRect(payment_title_win(),payment_title_square_x(),payment_title_square_y(),payment_title_square_width(),payment_title_square_height(),
			//create_xui_color(payment_title_square_color()),0,0);
	}
	XuiCanvasDrawRect(payment_title_win(),book_title_square_x(),book_title_square_y(),book_title_square_width(),book_title_square_height(),
			create_xui_color(book_title_square_color()),0,0);

	//draw product_amount
	{
		XuiCanvasDrawText(payment_title_win(),title_product_amount_text_x(),title_product_amount_text_y(),title_product_amount_text_height(),
			title_product_text_font(),0,create_xui_color(title_product_amount_text_color()),title_product_amount_text());
	}
	return 0;
}

#endif//__PROLIN__





