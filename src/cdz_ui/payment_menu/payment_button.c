/*------------------------------------------------------------
* FileName: payment_button.c
* Author: lugoo
* Date: 2017-05-22
------------------------------------------------------------*/
#include "sm_all.h"
	
#include <xui.h>
#include <stdlib.h>
#include "pax_log.h"
	
#define REF_AREA
	XuiColor create_xui_color(int argb_hex);
#undef REF_AREA

XuiWindow *payment_keypad_btn_index(int i)
{
	static XuiWindow *s_btn_list[64]={0};
	static int first=1;
	if (first) {
		int i;
		int payment_keypad_max_btn_count=2;
		bf_set_height(settle_keypad_btn_height());
		bf_set_bg_color(settle_page_btn_bg_color_default());
		bf_set_bg_color_p(settle_page_btn_p_bg_color_default());
		bf_set_parent(payment_keypad_win_addr());
		for (i=0;i!=payment_keypad_max_btn_count;++i) {
			int x_seq=i%settle_keypad_btn_count_x_max();
			int y_seq=i/settle_keypad_btn_count_x_max();
			int btn_x=x_seq*settle_keypad_btn_width() + (x_seq+1)*settle_keypad_btn_x_gap();
			int btn_y=y_seq*settle_keypad_btn_height() + (y_seq+1)*settle_keypad_btn_y_gap();

			//Pax_Log(LOG_INFO,"btn[%d]<x_seq:%d, y_seq:%d> <x:%d,y:%d,w:%d,h:%d>",i,x_seq,y_seq,btn_x,btn_y,settle_keypad_btn_width(),settle_keypad_btn_height());
			bf_set_x(btn_x);
			bf_set_y(btn_y);
			s_btn_list[i]=create_xui_button();
		}		
		first=0;
	}
	return s_btn_list[i];
}




#define BTN_ATTR
int payment_keypad_btn_back_locate_style() {return keypad_locate_style_a();}
int payment_keypad_btn_back_key() {return key_code_back();}
int payment_keypad_btn_back_btn_list_index() {return 0;}
int payment_keypad_btn_back_text_gap() {return settle_keypad_btn_text_gap();}
XuiFont *payment_keypad_btn_back_text_font() {return settle_keypad_btn_text_font();}
char *payment_keypad_btn_back_text() {return "Back";}
int payment_keypad_btn_back_text_color() {return  settle_keypad_btn_text_color();}
int payment_keypad_btn_back_text_height() {return settle_keypad_btn_text_height();}
int payment_keypad_btn_back_text_width() {return XuiTextWidth(settle_keypad_btn_back_text_font(),settle_keypad_btn_back_text_height(),settle_keypad_btn_back_text());}
char *payment_keypad_btn_back_img_path() {return keypad_btn_back_img_path();}

XuiImg *payment_keypad_btn_back_img() {static XuiImg *s=NULL;return (!s) ? (s=XuiImgLoadFromFile(settle_keypad_btn_back_img_path())) : (s);}

int payment_keypad_btn_back_img_width() {XuiImg *img=settle_keypad_btn_back_img();return (img) ? (img->width) : (0);}
int payment_keypad_btn_back_img_height() {XuiImg *img=settle_keypad_btn_back_img();return (img) ? (img->height) : (0);}
int payment_keypad_btn_back_img_x()
{
	if (payment_keypad_btn_back_locate_style()==keypad_locate_style_b()) {
		return settle_keypad_btn_width()-settle_keypad_btn_back_text_gap()-settle_keypad_btn_back_img_width();
	}
	return settle_keypad_btn_back_text_gap();
}
int payment_keypad_btn_back_img_y() {return (settle_keypad_btn_height()-settle_keypad_btn_back_img_height())/2;}
int payment_keypad_btn_back_text_x()
{
	if (settle_keypad_btn_back_locate_style()==keypad_locate_style_b()) {
		return settle_keypad_btn_width()-2*settle_keypad_btn_back_text_gap()-settle_keypad_btn_back_img_width()-settle_keypad_btn_back_text_width();
	}
	return settle_keypad_btn_back_img_x()+settle_keypad_btn_back_img_width()+settle_keypad_btn_back_text_gap();
}
int payment_keypad_btn_back_text_y() {return (settle_keypad_btn_height()-settle_keypad_btn_back_text_height())/2;}
int payment_keypad_btn_back_refresh()
{
	XuiWindow *btn=payment_keypad_btn_index(0);
	XuiButtonSetKey(btn,settle_keypad_btn_back_key());
	//set btn view
	XuiButtonStat stat;memset(&stat,0,sizeof(stat));
	stat.btn_bg=create_xui_color(settle_keypad_btn_back_bg_color());			
	stat.img=settle_keypad_btn_back_img();
	stat.img_x=settle_keypad_btn_back_img_x();
	stat.img_y=settle_keypad_btn_back_img_y();

	
	stat.text=settle_keypad_btn_back_text();
	stat.text_height=settle_keypad_btn_back_text_height();
	stat.text_x=settle_keypad_btn_back_text_x();
	stat.text_y=settle_keypad_btn_back_text_y();
	stat.text_font=settle_keypad_btn_back_text_font();
	stat.text_fg=create_xui_color(settle_keypad_btn_back_text_color());
	XuiButtonSetStat(btn,XUI_BTN_NORMAL,&stat);
	XuiButtonSetStat(btn,XUI_BTN_PRESSED,&stat);

	return 0;
}
#undef BTN_ATTR


