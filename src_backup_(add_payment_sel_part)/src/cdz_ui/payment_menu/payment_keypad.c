/*------------------------------------------------------------
* FileName: payment_keypad.c
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



XuiWindow *payment_keypad_win_addr()
{
	static XuiWindow *s_win=NULL;
	if (!s_win) {
		s_win=XuiCreateCanvas(payment_menu_win_addr(),book_keypad_win_x(),book_keypad_win_y(),book_keypad_win_width(),book_keypad_win_height());
	}
	return s_win;
}

XuiWindow *payment_keypad_win()
{
	static int first=1;
	if (first) {
		XuiCanvasSetBackground(payment_keypad_win_addr(),0,0,create_xui_color(minipos_page_background_color()));
		payment_keypad_btn_index(0);
		payment_keypad_btn_back_refresh();
		Pax_Log(LOG_INFO,"disappear payment back button");
		first=0;
	}
	return payment_keypad_win_addr();
}





