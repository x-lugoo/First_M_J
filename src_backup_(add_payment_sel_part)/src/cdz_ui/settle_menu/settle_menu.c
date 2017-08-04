#include "sm_all.h"
#ifdef __PROLIN__

#include <xui.h>
#include <stdlib.h>
#include "pax_log.h"


int settle_menu_win_width() {return get_screen_width();}
int settle_menu_win_height() {return get_screen_height();}
int settle_menu_win_x() {return 0;}
int settle_menu_win_y() {return 0;}
extern int glSelectPageBtnFlag;
extern pthread_t selProductThread;
extern void *SettleProductSel(void);
extern int glStartSelProductThread;

#define SETTLE_MENU_ADDR
XuiWindow *settle_menu_win_addr()
{
	static XuiWindow *s_win=NULL;
	if (!s_win) {
		s_win=XuiCreateCanvas(XuiRootCanvas(),settle_menu_win_x(),settle_menu_win_y(),settle_menu_win_width(),settle_menu_win_height());
	}
	return s_win;
}
#undef SETTLE_MENU_ADDR

#define SETTLE_MENU_WIN_AERA
XuiWindow *settle_menu_win()
{
	static int first=1;
	if (first) {
		XuiShowWindow(settle_title_win(),XUI_SHOW,0);
		XuiShowWindow(settle_page_win(),XUI_SHOW,0);
		XuiShowWindow(settle_keypad_win(),XUI_SHOW,0);
		first=0;
	}
	return settle_menu_win_addr();
}
#undef SETTLE_MENU_WIN_AERA


#define SETTLE_PAGE_CALCULATE_AREA
int settle_menu_get_page_count()
{
	int btn_count=settle_product_list_node_count();
	int btn_count_max=settle_page_btn_count_max();
	if (btn_count<=btn_count_max) {
		return 1;
	}
	return (btn_count-1)/btn_count_max+1;
}
#undef SETTLE_PAGE_CALCULATE_AREA


int settle_menu_init() {return 0;}

int settle_menu_first_page() {return 0;}
static int s_settle_menu_cur_page=0;
int settle_menu_get_cur_page() {return s_settle_menu_cur_page;}
int settle_menu_set_cur_page(int page) {s_settle_menu_cur_page=page;return 0;}

static void *s_settle_page_process_select_node=NULL;
void *settle_page_process_get_select_node() {return s_settle_page_process_select_node;}
int settle_page_process_set_select_node(void *node) {s_settle_page_process_select_node=node;return 0;}

static int s_settle_menu_ret_value=0;

int WaitSelProThread(int pthread) //added by jeff_xiehuan20170528
{
     Pax_Log(LOG_INFO,"start to join pthread d=%d",pthread);
     pthread_join(pthread,NULL);
     if(glStartSelProductThread == 1)
     {
		glStartSelProductThread = 0;
	 }
	return 0;
}


int settle_menu_process()
{
	set_menu_stat(menu_stat_settle());
	settle_menu_set_cur_page(settle_menu_first_page());
	pthread_create(&selProductThread, NULL, SettleProductSel, NULL);
	WaitSelProThread(selProductThread);
	while (1) {
		int page_code=settle_page_process();//ERR("settle_page_process=%d",page_code);
		Pax_Log(LOG_INFO,"settle_page_process,=%d",page_code);
		if (page_code==page_code_to_prev_page()) {
			pthread_create(&selProductThread, NULL, SettleProductSel, NULL);
			WaitSelProThread(selProductThread);
			DestorySelButAndRefresh();
		}
		if (page_code==page_code_to_next_page()) {
			pthread_create(&selProductThread, NULL, SettleProductSel, NULL);
			WaitSelProThread(selProductThread);
			DestorySelButAndRefresh();
		}
		if (page_code==page_code_user_exit() || page_code==page_code_timeout_eixt() || page_code==page_code_error_exit()) {
			s_settle_menu_ret_value=page_code_user_exit();
			break;
		}
		if (page_code==page_code_back()) {
			s_settle_menu_ret_value=page_code_back();
			break;
		}
		if (page_code==page_code_ensure()) {
			s_settle_menu_ret_value=page_code_ensure();
			
			break;
		}
		if(page_code >= 100000)
		{
			Pax_Log(LOG_INFO,"page_code>100000,=%d",page_code);
		}
		if (page_code == page_code_delete() || page_code >= settle_page_btn_virtual_Sub_key_base()) {
			Pax_Log(LOG_INFO,"start to delete");
			void *node=settle_page_process_get_select_node();
			int node_count=product_node_get_count(node)-1;
			if(page_code == page_code_delete())
			{
				node_count = 0;
			}
			product_node_set_count(node,node_count);
			if (node_count==0) {
				settle_page_process_set_select_node(NULL);
			}
			if(product_node_get_count(node) == 0)
			{
				DestorySelButAndRefresh();
			}
			else 
			{
				pthread_create(&selProductThread, NULL, SettleProductSel, NULL);
				WaitSelProThread(selProductThread);
			}
		}
		else if(page_code == key_code_add() || (page_code >= settle_page_btn_virtual_Plus_key_base() && page_code < settle_page_btn_virtual_Sub_key_base())) //add by jeff_xiehuan
		{
			Pax_Log(LOG_INFO,"start add");
			void *node=settle_page_process_get_select_node();
			int node_count=product_node_get_count(node) + 1;
			product_node_set_count(node,node_count);
			if (node_count==0) {
				settle_page_process_set_select_node(NULL);
			}
			pthread_create(&selProductThread, NULL, SettleProductSel, NULL);
			WaitSelProThread(selProductThread);
		}
		if (page_code==page_code_select_product()) {
			void *node=settle_page_process_get_select_node();
			pthread_create(&selProductThread, NULL, SettleProductSel, NULL);
			WaitSelProThread(selProductThread);
		}
	}
	WaitSelProThread(selProductThread);
    DestorySelProductBut(1);
	XuiShowWindow(settle_menu_win(),XUI_HIDE,0);
	set_menu_stat(menu_stat_null());
	return 0;
}
int settle_menu_process_get_value() {return s_settle_menu_ret_value;}

#endif//__PROLIN__
