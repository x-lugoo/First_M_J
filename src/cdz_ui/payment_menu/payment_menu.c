/*------------------------------------------------------------
* FileName: payment_menu.c
* Author: lugoo
* Date: 2017-05-22
------------------------------------------------------------*/


#include "sm_all.h"
#ifdef __PROLIN__

#include <xui.h>
#include <stdlib.h>
#include "pax_log.h"


int payment_menu_win_width() {return get_screen_width();}
int payment_menu_win_height() {return get_screen_height();}
int payment_menu_win_x() {return 0;}
int payment_menu_win_y() {return 0;}


#define PAYMENT_MENU_ADDR
XuiWindow *payment_menu_win_addr()
{
	static XuiWindow *s_win=NULL;
	if (!s_win) {
		s_win=XuiCreateCanvas(XuiRootCanvas(),payment_menu_win_x(),payment_menu_win_y(),payment_menu_win_width(),payment_menu_win_height());
	}
	return s_win;
}

XuiWindow *payment_menu_win()
{
	static int first=1;
	if (first) {
		XuiShowWindow(payment_title_win(),XUI_SHOW,0);
		XuiShowWindow(payment_page_win(),XUI_SHOW,0);
		XuiShowWindow(payment_keypad_win(),XUI_SHOW,0);
		first=0;
	}
	return payment_menu_win_addr();
}

int payment_menu_process()
{
	
	XuiShowWindow(payment_menu_win(),XUI_SHOW,0);
	payment_title_refresh();
	payment_page_refresh();
	
	ST_TIMER timer={0};
	OsTimerSet(&timer,book_menu_exit_delay_ms());
	while (1) 
	{
		int key=0;
		if (XuiHasKey()) {
			key=XuiGetKey();
			Pax_Log(LOG_INFO,"key=%d,func:%s,Line:%d",key,__FUNCTION__,__LINE__);
		} else {
			if (OsTimerCheck(&timer)==0) {return page_code_timeout_eixt();}
			continue;
		}
		OsTimerSet(&timer,book_menu_exit_delay_ms());
		if (key==key_code_back())
		{
			return page_code_back();
		}
        else if(key == XUI_KEY1 || key == XUI_KEY2 || key == XUI_KEY3)
        {
        	return key;
        }
	}
	return 0;
}

#undef PAYMENT_MENU_ADDR

#endif
