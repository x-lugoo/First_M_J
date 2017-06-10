/*------------------------------------------------------------
* FileName: payment_page.c
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






static XuiWindow *payment_page_win_addr()
{
	static XuiWindow *s_win=NULL;
	if (!s_win) {
		s_win=XuiCreateCanvas(payment_menu_win_addr(),book_page_win_x(),book_page_win_y(),book_page_win_width(),book_page_win_height());
	}
	return s_win;
}

XuiWindow *payment_page_win()
{
	static int first=1;
	if (first) {
		XuiCanvasSetBackground(payment_page_win_addr(),0,0,create_xui_color(minipos_page_background_color()));
		//payment_page_btn(0);
		first=0;
	}
	return payment_page_win_addr();
}

XuiWindow *payment_page_btn(int i)
{
	static XuiWindow *s_list[64]={0};
	static int first=1;
	if (first) {
		int i;
		int page_max_btn_count=4;
		bf_set_width(settle_page_btn_width());
		bf_set_height(settle_page_btn_height());
		bf_set_bg_color(settle_page_btn_bg_color_default());
		bf_set_bg_color_p(settle_page_btn_p_bg_color_default());
		bf_set_parent(payment_page_win());
		for (i=0;i!=page_max_btn_count;++i) {
			bf_set_x(settle_page_btn_x(i));
			bf_set_y(settle_page_btn_y(i));
			s_list[i]=create_xui_button();
		}
		first=0;
	}
	return s_list[i];
}

int payment_page_refresh()
{
	//TIMER_START_DEF
	//XuiShowWindow(settle_menu_win(),XUI_SHOW,0);
	//XuiClearArea(settle_page_win(),settle_page_win_x(),settle_page_win_y(),settle_page_win_width(),settle_page_win_height());
	int i;
	int count=3;
	char *szPaymentWay[3] = {"1.Credit card","2.MobilePay","3.Cash"};
	
	for (i=0;i!=count;++i) {
		XuiWindow *btn=payment_page_btn(i);
		cur_line_btn_set_index(i);

			//set btn common stat
			XuiButtonStat stat_common;
			{
				memset(&stat_common,0,sizeof(XuiButtonStat));
				stat_common.text=szPaymentWay[i];
				stat_common.text_x=settle_page_btn_text_x();
				stat_common.text_y=settle_page_btn_text_y();
				stat_common.text_height=settle_page_btn_text_height();
				stat_common.text_font=get_text_font();
			}
			
			{//set btn normal stat
				XuiButtonStat stat;
				memcpy(&stat,&stat_common,sizeof(XuiButtonStat));
				stat.text_fg=create_xui_color(0xff000000);
				stat.btn_bg=create_xui_color(0xffffffff);
				XuiButtonSetStat(btn,XUI_BTN_NORMAL,&stat);
			}
			
			{//set btn pressed stat
				XuiButtonStat stat;
				memcpy(&stat,&stat_common,sizeof(XuiButtonStat));
				stat.text_fg=create_xui_color(0xff000000);
				stat.btn_bg=create_xui_color(0xff00ffff);
				XuiButtonSetStat(btn,XUI_BTN_PRESSED,&stat);
			}
			XuiButtonSetKey(btn,i + 2);
			XuiShowWindow(btn,XUI_SHOW,0);

			//»­ºÚ±ß
			payment_cur_page_draw_line();
	}
	//TIMER_CHECK
	return 0;
}

