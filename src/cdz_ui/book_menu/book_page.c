#include "sm_all.h"
#ifdef __PROLIN__




#include <xui.h>
#include <osal.h>
#include <stdlib.h>
#include <string.h>
#include "pax_log.h"
#include <stdio.h>
#include "Display.h"
#include "dataExchange.h"
#define REF_AREA
XuiColor create_xui_color(int argb_hex);
int glSelVarPriceFlag = 0;
int glStartOfflineUploadMode = 0;
int glCurrentOfflineIdx = 0;


#define MAX_OFFLINE_UPLOAD_NUM 10
#undef REF_AREA

#define PAGE_AREA
#include <string.h>
XuiWindow *book_page_win_addr()
{
	static XuiWindow *s_win;
	static int first=1;
	if (first) {
		s_win=XuiCreateCanvas(book_menu_win_addr(),book_page_win_x(),book_page_win_y(),book_page_win_width(),book_page_win_height());
		first=0;
	}
	return s_win;
}

XuiWindow *book_sign_win_addr()
{
	static XuiWindow *s_win;
	static int first=1;
	if (first) {
		s_win=XuiCreateCanvas(book_menu_win_addr(),book_page_win_x(),book_page_win_y(),book_page_win_width(),book_page_win_height());
		first=0;
	}
	return s_win;
}



static int s_page_btn_count;
XuiWindow *page_btn_index(int i)
{
	static XuiWindow *s_btn_list[64]={0};
	static int first=1;
	if (first) {
		int i;
		int page_max_btn_count=book_page_btn_count_x_max()*book_page_btn_count_y_max();
		bf_set_width(book_page_btn_width());
		bf_set_height(book_page_btn_height());
		bf_set_bg_color(0xff00ff00);
		bf_set_bg_color_p(0xff0000ff);
		bf_set_parent(book_page_win_addr());
		for (i=0;i!=page_max_btn_count;++i) {
			int x_seq=i%book_page_btn_count_x_max();
			int y_seq=i/book_page_btn_count_x_max();
			int btn_x=x_seq*book_page_btn_width() + (x_seq+1)*book_page_btn_x_gap();
			int btn_y=y_seq*book_page_btn_height() + (y_seq+1)*book_page_btn_y_gap();
			bf_set_x(btn_x);
			bf_set_y(btn_y);
			s_btn_list[i]=create_xui_button();
			//ERR("btn[%d]<%d,%d> <x:%3d,y:%3d> <w:%3d,h:%3d>",i,x_seq,y_seq,btn_x,btn_y,book_page_btn_width(),book_page_btn_height());
			
		}
		s_page_btn_count=page_max_btn_count;
		first=0;
	}
	return s_btn_list[i];
}




int page_btn_count()
{
	page_btn_index(0);
	return s_page_btn_count;
}



XuiWindow *book_page_win()
{
	static int first=1;
	if (first) {
		XuiCanvasSetBackground(book_page_win_addr(),0,0,create_xui_color(book_page_win_color()));
		page_btn_index(0);
		first=0;
	}
	return book_page_win_addr();
}




int cur_page_get_index() {return 0;}
int book_page_refresh()
{
	//TIMER_START_DEF
	char signNamepath[20];
	XuiShowWindow(book_menu_win(),XUI_SHOW,0);
	int i;
	int iVaildNodeCount = 0;
	int count=page_btn_count();
	void *node=node_get_seq_son_node(book_menu_get_cur_page_node(),cur_page_get_index()*book_page_btn_count_max());
   
	for (i=0;i!=count;++i) {
		XuiWindow *btn=page_btn_index(i);
		if (node) {
			//set btn common stat
			iVaildNodeCount++;
			XuiButtonStat stat_common;
			{
				memset(&stat_common,0,sizeof(XuiButtonStat));
				if (node_get_btn_text_flag(node)) {
					stat_common.text=node_get_btn_text(node);
					stat_common.text_x=node_get_btn_text_x(node);
					stat_common.text_y=node_get_btn_text_y(node);
					stat_common.text_height=node_get_btn_text_height(node);
					stat_common.text_fg=create_xui_color(node_get_btn_text_color(node));
					stat_common.text_font=node_get_text_font(node);
					
				}
			}
		
			{//set btn normal stat
			
				XuiButtonStat stat;
				memcpy(&stat,&stat_common,sizeof(XuiButtonStat));
				memset(signNamepath,0,sizeof(signNamepath));
				if(node_is_folder(node))
				{
					sprintf(signNamepath,"./res/%d.png",iVaildNodeCount + 10);
				}
				else if(node_is_product(node))
				{
					sprintf(signNamepath,"./res/%d.png",iVaildNodeCount);
				}
				
				stat.img=XuiImgLoadFromFile(signNamepath);
				//stat.img=node_get_img(node);
				stat.img_x=node_get_img_x(node);
				stat.img_y=node_get_img_y(node);
				stat.btn_bg=create_xui_color(node_get_btn_bg_color(node));
				XuiButtonSetStat(btn,XUI_BTN_NORMAL,&stat);
			}
			
			{//set btn pressed stat
				XuiButtonStat stat;
				memcpy(&stat,&stat_common,sizeof(XuiButtonStat));
				stat.img=node_get_img_p(node);
				stat.img_x=node_get_img_x_p(node);
				stat.img_y=node_get_img_y_p(node);
				stat.btn_bg=create_xui_color(node_get_btn_bg_color_p(node));
				XuiButtonSetStat(btn,XUI_BTN_PRESSED,&stat);
			}
			
			XuiButtonSetKey(btn,node_get_btn_key(node));
			XuiShowWindow(btn,XUI_SHOW,0);
			
		} else {
			XuiShowWindow(btn,XUI_HIDE,0);
			node=node_get_next_brother_node(node);
		}
		node=node_get_next_brother_node(node);
	}

	//TIMER_CHECK
	return 0;
}

static int s_select_node=NULL;
int get_select_product_node() {return s_select_node;}
int set_select_product_node(void *node) {s_select_node=node;return 0;}

int book_page_process()
{
	ST_TIMER timer={0};
	int iBatteryLevel;
	OsTimerSet(&timer,book_menu_exit_delay_ms());
	int i;
	int iRet;
	
	while (1) {
		int key=0;
		iBatteryLevel = OsCheckBattery();
		if(iBatteryLevel == BATTERY_LEVEL_0)
		{
			XuiShowWindow(book_menu_win(),XUI_HIDE,0);
			DisplayPrompt("LOW BATTERY", "please charge your terminal", MSGTYPE_WARNING, 0);
			HidePromptWin();
			XuiShowWindow(book_menu_win(),XUI_SHOW,0);
		}
		if(glOfflineNum == MAX_OFFLINE_UPLOAD_NUM)
		{
			XuiShowWindow(book_menu_win(),XUI_HIDE,0);
			glStartOfflineUploadMode = 1;
			do
			{
				for(i = 0;i < MAX_OFFLINE_UPLOAD_NUM;i++)
				{
					glCurrentOfflineIdx = i;
					iRet = RequestProcess(CMD_UPLOAD_DATA,NORMAL);
					if(iRet)
					{
						break;
					}
				}
			}while(iRet);
			glStartOfflineUploadMode = 0;
			glOfflineNum = 0;
			HidePromptWin();
			remove(OFFSET_TRAN_FILE);
			remove(OFFSET_NUM_FILE);
			XuiShowWindow(book_menu_win(),XUI_SHOW,0);
		}
		if (XuiHasKey()) {
			key=XuiGetKey();
		} else {
			if (OsTimerCheck(&timer)==0)
			{
				 OsSysSleepEx(1);
				 OsTimerSet(&timer,book_menu_exit_delay_ms());
				
			}
			continue;
		}
		OsTimerSet(&timer,book_menu_exit_delay_ms());
		

		{//check whether select function
			if (key==key_code_back()) {
				void *node=node_get_father_node(book_menu_get_cur_page_node());
				if (node) {
					book_menu_set_cur_page_node(node);
					return page_code_to_father_page();
				}
			}
			if (key==key_code_ensure()) {
				return page_code_ensure();
			}

			if(key == KEYFUNC)
			{
				return page_code_func();
			}
		}

		{//check whether select item
			void *node=NULL;
			if (key_code_is_touch_button(key)) {
				int node_index=key-book_page_btn_key_base();
				node=family_tree_get_node(node_index);
			}
			if (key_code_is_num(key)) {
				int seq=key-key_code_num_base();
				node=node_get_seq_son_node(book_menu_get_cur_page_node(),seq);
			}
			if (node) {
				if (node_get_first_son_node(node)) {
					book_menu_set_cur_page_node(node);
					return page_code_to_son_page();
				} else if (node) {        //added by jeff_xiehuan20170520
					set_select_product_node(node);
					PaxLog(LOG_INFO,"Varprice=%d,F=%s:Line:%d",product_node_get_ifVariable_price(node),
									__FUNCTION__,__LINE__);
					if(product_node_get_ifVariable_price(node))
					{
						 double amount = 0.0;
			
						 XuiShowWindow(book_menu_win(),XUI_HIDE,0);
						 GetFormantAmount(&amount);
						 XuiShowWindow(book_menu_win(),XUI_SHOW,0);
						 if((int)amount == 0)
						 {
						 	glSelVarPriceFlag = 1;
						 	PaxLog(LOG_INFO,"getAmount=%d,F:%s,Line:%d",(int)amount,__FUNCTION__,__LINE__);
						 }
						 else 
						 {
							 
							 product_node_set_price(node,(int)amount);
							 prouduct_node_set_vat(node,(double)((int)amount) / 5);
							 PaxLog(LOG_INFO,"getAmount=%d,F:%s,Line:%d",(int)amount,__FUNCTION__,__LINE__);
						 }
					}
					return page_code_select_product();
				}
			}
		}
	}
	
	return page_code_none();
}
#undef PAGE_AREA

#endif//ifdef _PROLIN2_4_

