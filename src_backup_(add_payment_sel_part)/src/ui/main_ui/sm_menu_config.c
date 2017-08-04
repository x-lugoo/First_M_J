#include "sm_all.h"
#ifdef __PROLIN__



#include <stdlib.h>
#include <string.h>
#include "pax_log.h"

int book_menu_win_width() {return get_screen_width();}
int book_menu_win_height() {return get_screen_height();}
int book_menu_win_x() {return 0;}
int book_menu_win_y() {return 0;}
int book_menu_win_color() {return 0xffff0000;}
int book_menu_exit_delay_ms() {return 30*1000;}//only for test

int book_title_win_width() {return book_menu_win_width();}
int book_title_win_height() {return 42;}
int book_title_win_x() {return 0;}
int book_title_win_y() {return 0;}
int book_title_win_color() {return minipos_title_background_color();}
int book_title_win_text_color() {return title_text_color();}
int book_title_win_text_height() {return 16;}

int book_title_win_text_y() {return ( book_title_win_height()-book_title_win_text_height() )/2;}

int book_page_win_width() {return book_menu_win_width();}
int book_page_win_height()
{
	int iHeight = 0;

	iHeight = book_menu_win_height()*6/8;
	//Pax_Log(LOG_INFO,"book_menu_win_height=%d",iHeight);
	return 200;
}


int book_page_win_x() {return 0;}
int book_page_win_y() {return book_title_win_height();}
int book_page_win_color() {return minipos_page_background_color();}

int book_page_btn_count_x_max() {return 2;}
int book_page_btn_count_y_max() {return 3;}
int book_page_btn_count_max() {return book_page_btn_count_x_max()*book_page_btn_count_y_max();}
int book_page_btn_x_gap() {return title_gap();}
int book_page_btn_y_gap() {return title_gap();}
int book_page_btn_width() {return (book_page_win_width()-(book_page_btn_count_x_max()+1)*book_page_btn_x_gap())/book_page_btn_count_x_max();}
int book_page_btn_height() {return (book_page_win_height()-(book_page_btn_count_y_max()+1)*book_page_btn_y_gap())/book_page_btn_count_y_max();}

int book_page_btn_text_is_existed() {return 1;}
//int book_page_btn_text_height() {return book_page_btn_height()*1/4;}
int book_page_btn_text_height() {return 12;}

int book_page_btn_text_y() {return (book_page_btn_height()-book_page_btn_text_height())/2;}
int book_page_btn_key_base() {return 10000;}
int book_page_btn_bg_color_default() {return 0xff007766;}
//int book_page_btn_p_bg_color_default() {return 0xff742355;}
int book_page_btn_p_bg_color_default() {return 0xff000000;}

int book_page_btn_seq_icon_gap() {return 5;}
int book_page_btn_seq_icon_x_gap() {return book_page_btn_seq_icon_gap();}
int book_page_btn_seq_icon_y_gap() {return book_page_btn_seq_icon_gap();}


int book_page_under_text_is_existed() {return 0;}
int book_page_under_text_height() {return book_page_btn_y_gap()*2/4;}
int book_page_under_text_btn_gap() {return book_page_btn_y_gap()*1/4;}









#endif//ifdef _PROLIN2_4_

