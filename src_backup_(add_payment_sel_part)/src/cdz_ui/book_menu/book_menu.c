#include "sm_all.h"
#ifdef __PROLIN__

#include <xui.h>
#include <osal.h>
#include <stdlib.h>
#include <string.h>
#include "pax_log.h"


extern int glSelVarPriceFlag;

XuiWindow *book_menu_win_addr()
{
	static XuiWindow *s_win;
	static int first=1;
	if (first) {
		s_win=XuiCreateCanvas(XuiRootCanvas(),book_menu_win_x(),book_menu_win_y(),book_menu_win_width(),book_menu_win_height());
		first=0;
	}
	return s_win;
}

XuiWindow *book_menu_win()
{
	static int first=1;
	if (first) {
		XuiShowWindow(book_title_win(),XUI_SHOW,0);
		XuiShowWindow(book_page_win(),XUI_SHOW,0);
		XuiShowWindow(book_keypad_win(),XUI_SHOW,0);
		first=0;
	}
	return book_menu_win_addr();
}




void *book_menu_first_node() {return family_tree_get_node(0);}
static void *s_book_menu_cur_node=NULL;
void *book_menu_get_cur_page_node() {return (!s_book_menu_cur_node) ? (s_book_menu_cur_node=book_menu_first_node()) : (s_book_menu_cur_node);}
int book_menu_set_cur_page_node(void *node) {s_book_menu_cur_node=node;return 0;}

static int s_ret_value;
int book_menu_process()
{
	set_menu_stat(menu_stat_book());
	book_menu_set_cur_page_node(book_menu_first_node());
	book_title_refresh();
	book_page_refresh();
	book_keypad_refresh();
	while (1) {		
		int page_code=book_page_process();
		
		if (page_code==page_code_to_father_page()) {
			book_page_refresh();
			book_keypad_refresh();
		}
		if (page_code==page_code_to_son_page()) {
			book_page_refresh();
			book_keypad_refresh();
		}
		if (page_code==page_code_user_exit() || page_code==page_code_timeout_eixt() || page_code==page_code_error_exit()) {
			s_ret_value=page_code_exit();
			break;
		}
		if (page_code==page_code_ensure()) {
			s_ret_value=page_code_ensure();
			break;
		}
		if (page_code==page_code_select_product() && glSelVarPriceFlag == 0) {
			void *node=get_select_product_node();
			product_node_set_count(node,product_node_get_count(node)+1);
			book_title_refresh();
		}
		else if(page_code==page_code_select_product() && glSelVarPriceFlag == 1)
		{
			glSelVarPriceFlag = 0;
			book_title_refresh();
			
		}
		if(page_code == page_code_func())
		{
			s_ret_value = page_code_func();
			break;
		}
	}

	XuiShowWindow(book_menu_win(),XUI_HIDE,0);
	set_menu_stat(menu_stat_null());
	return 0;
}
int smenu_get_ret_value() {return s_ret_value;}


#endif//__PROLIN__
