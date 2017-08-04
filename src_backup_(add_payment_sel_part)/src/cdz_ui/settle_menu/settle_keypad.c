#include "sm_all.h"
#ifdef __PROLIN__

#include <xui.h>
#include <stdlib.h>
#include "pax_log.h"

#define REF_AREA
XuiColor create_xui_color(int argb_hex);
#undef REF_AREA


#define KEYPAD_WINDOW_CONFIG
int settle_keypad_win_color() {return minipos_page_background_color();}
int settle_keypad_win_width() {return book_menu_win_width();}
//int settle_keypad_win_height() {return book_menu_win_height()*1/9;}
int settle_keypad_win_height() {return book_menu_win_height()*1/10 + 25;}

int settle_keypad_win_x() {return 0;}
//int settle_keypad_win_y() {return book_title_win_height()+settle_page_win_height();}

int settle_keypad_win_y() {return book_title_win_height()+book_page_win_height();}

int settle_keypad_btn_count_x_max() {return 4;}
int settle_keypad_btn_count_y_max() {return 1;}
int settle_keypad_btn_x_gap() {return 2*pu();}
int settle_keypad_btn_y_gap() {return 2*pu();}
int settle_keypad_btn_width() {{return (settle_keypad_win_width()-(settle_keypad_btn_count_x_max()+1)*settle_keypad_btn_x_gap())/settle_keypad_btn_count_x_max();}}
int settle_keypad_btn_height() {return (settle_keypad_win_height()-(settle_keypad_btn_count_y_max()+1)*settle_keypad_btn_y_gap())/settle_keypad_btn_count_y_max();}
#undef KEYPAD_WINDOW_CONFIG


#define KEYPAD_ADDR_AREA
XuiWindow *settle_keypad_win_addr()
{
	static XuiWindow *s_win=NULL;
	if (!s_win) {
		s_win=XuiCreateCanvas(settle_menu_win_addr(),book_keypad_win_x(),book_keypad_win_y(),book_keypad_win_width(),book_keypad_win_height());
	}
	return s_win;
}
#undef KEYPAD_ADDR_AREA



#define KEYPAD_WIN_AREA
XuiWindow *settle_keypad_win()
{
	static int first=1;
	if (first) {
		XuiCanvasSetBackground(settle_keypad_win_addr(),0,0,create_xui_color(minipos_page_background_color()));
		settle_keypad_btn_back_refresh();
		settle_keypad_btn_ensure_refresh();
		settle_keypad_btn_delete_refresh();
		//settle_keypad_btn_add_refresh();
		first=0;
	}
	return settle_keypad_win_addr();
}
#undef KEYPAD_WIN_AREA


int settle_keypad_refresh()
{
	settle_keypad_btn_ensure_refresh();
	if (settle_page_process_get_select_node()!=NULL) {
		XuiShowWindow(settle_keypad_btn_index(settle_keypad_btn_delete_btn_list_index()),XUI_SHOW,0);
		//XuiShowWindow(settle_keypad_btn_index(settle_keypad_btn_add_btn_list_index()),XUI_SHOW,0);
	} else {
		//XuiShowWindow(settle_keypad_btn_index(settle_keypad_btn_add_btn_list_index()),XUI_HIDE,0);
		XuiShowWindow(settle_keypad_btn_index(settle_keypad_btn_delete_btn_list_index()),XUI_HIDE,0);
	}
	return 0;	
}


#endif//__PROLIN__
