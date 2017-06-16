#include "sm_all.h"
#ifdef __PROLIN__



#include <osal.h>
#include <xui.h>
#include <stdlib.h>
#include <string.h>
#include "pax_log.h"

#define REF_AREA
XuiColor create_xui_color(int argb_hex);
#undef REF_AREA


#define BOOK_KEYPAD_CONFIG
int book_keypad_win_width() {return book_menu_win_width();}
int book_keypad_win_height() {return book_menu_win_height()- book_page_win_height() - book_title_win_height();}
int book_keypad_win_x() {return 0;}
int book_keypad_win_y() {return book_title_win_height()+book_page_win_height();}
int book_keypad_win_color() {return minipos_page_background_color();}

int book_keypad_btn_count_x_max() {return 4;}
int book_keypad_btn_count_y_max() {return 1;}
int book_keypad_btn_count_max() {return book_keypad_btn_count_x_max()*book_keypad_btn_count_y_max();}
int book_keypad_btn_x_gap() {return 2*pu();}
int book_keypad_btn_y_gap() {return 2*pu();}
int book_keypad_btn_width() {{return (book_keypad_win_width()-(book_keypad_btn_count_x_max()+1)*book_keypad_btn_x_gap())/book_keypad_btn_count_x_max();}}
int book_keypad_btn_height() {return (book_keypad_win_height()-(book_keypad_btn_count_y_max()+1)*book_keypad_btn_y_gap())/book_keypad_btn_count_y_max();}
#undef BOOK_KEYPAD_CONFIG


#define ADDR_AREA
XuiWindow *book_keypad_win_addr()
{
	static XuiWindow *s_win;
	static int first=1;
	if (first) {
		s_win=XuiCreateCanvas(book_menu_win_addr(),book_keypad_win_x(),book_keypad_win_y(),book_keypad_win_width(),book_keypad_win_height());
		first=0;
	}
	return s_win;
}
#undef ADDR_AREA



XuiWindow *book_keypad_win()
{
	static int first=1;
	if (first) {
		XuiCanvasSetBackground(book_keypad_win_addr(),0,0,create_xui_color(book_keypad_win_color()));
		XuiShowWindow(book_keypad_btn_index(1),XUI_HIDE,0);
		XuiShowWindow(book_keypad_btn_index(2),XUI_HIDE,0);
		book_keypad_btn_ensure_refresh();
		first=0;
	}
	return book_keypad_win_addr();
}

int book_keypad_refresh()
{
	book_keypad_btn_back_refresh();
	return 0;	
}


#endif//__PROLIN__
