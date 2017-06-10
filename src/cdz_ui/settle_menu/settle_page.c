#include "sm_all.h"
#ifdef __PROLIN__

#include <xui.h>
#include <stdlib.h>
#include "pax_log.h"
extern char* glCurrencyName;
extern int glStartSelProduct;
extern int glStartSelProductThread;

#define REF_AREA
XuiColor create_xui_color(int argb_hex);
#undef REF_AREA
static void *sNode = NULL;
static int sNodeIndex = 0;
extern pthread_t selProductThread;
extern void *SettleProductSel(void);



#define SETTLE_PAGE_CONFIG
int settle_page_win_width() {return book_page_win_width();}
int settle_page_win_height() {return book_page_win_height();}
int settle_page_win_x() {return book_page_win_x();}
int settle_page_win_y() {return book_page_win_y();}
int settle_page_win_color() {return book_page_win_color();}
int settle_page_btn_count_x_max() {return 1;}
int settle_page_btn_count_y_max() {return 5;}
int settle_page_btn_count_max() {return settle_page_btn_count_x_max()*settle_page_btn_count_y_max();}
int settle_page_btn_x_gap() {return title_gap();}
int settle_page_btn_y_gap() {return 5*pu();}
int settle_page_btn_width() {return (settle_page_win_width()-(settle_page_btn_count_x_max()+1)*settle_page_btn_x_gap())/settle_page_btn_count_x_max();}
int settle_page_btn_height() {return (settle_page_win_height()-(settle_page_btn_count_y_max()+1)*settle_page_btn_y_gap())/settle_page_btn_count_y_max();}
int settle_page_btn_text_is_existed() {return 1;}
//int settle_page_btn_text_height() {return settle_page_btn_height()*1/3;}
int settle_page_btn_text_height() {return 14;}

int settle_page_btn_text_x() {return 0;}
int settle_page_btn_text_y() {return (settle_page_btn_height()-settle_page_btn_text_height())/2;}
int settle_page_btn_virtual_key_base() {return 10000;}
int settle_page_btn_virtual_Text_key_base() {return 200000;}
int settle_page_btn_virtual_Plus_key_base() {return 300000;}
int settle_page_btn_virtual_Sub_key_base() {return 400000;}

int settle_page_btn_bg_color_default() {return minipos_page_background_color();}
int settle_page_btn_p_bg_color_default() {return minipos_page_background_color();}
#undef SETTLE_PAGE_CONFIG


#define SETTLE_PAGE_WIN_AREA
static XuiWindow *settle_page_win_addr()
{
	static XuiWindow *s_win=NULL;
	if (!s_win) {
		s_win=XuiCreateCanvas(settle_menu_win_addr(),book_page_win_x(),book_page_win_y(),book_page_win_width(),book_page_win_height());
	}
	return s_win;
}
XuiWindow *settle_page_win()
{
	static int first=1;
	if (first) {
		XuiCanvasSetBackground(settle_page_win_addr(),0,0,create_xui_color(book_page_win_color()));
		page_btn_index(0);
		first=0;
	}
	return settle_page_win_addr();
}
#undef SETTLE_PAGE_WIN_AREA

#define SETTLE_PAGE_BUTTON_AREA
 int settle_page_btn_x(int i)
{
	int x_seq=i%settle_page_btn_count_x_max();
	return x_seq*settle_page_btn_width() + (x_seq+1)*settle_page_btn_x_gap();
}
 int settle_page_btn_y(int i)
{
	int y_seq=i/settle_page_btn_count_x_max();
	return y_seq*settle_page_btn_height() + (y_seq+1)*settle_page_btn_y_gap();
}
XuiWindow *settle_page_btn(int i)
{
	static XuiWindow *s_list[64]={0};
	static int first=1;
	if (first) {
		int i;
		int page_max_btn_count=settle_page_btn_count_max();
		bf_set_width(settle_page_btn_width());
		bf_set_height(settle_page_btn_height());
		bf_set_bg_color(settle_page_btn_bg_color_default());
		bf_set_bg_color_p(settle_page_btn_p_bg_color_default());
		bf_set_parent(settle_page_win());
		for (i=0;i!=page_max_btn_count;++i) { 
				bf_set_x(settle_page_btn_x(i));
				bf_set_y(settle_page_btn_y(i));
				s_list[i]=create_xui_button();
		}
		first=0;
	}
	return s_list[i];
}

XuiWindow *settle_page_select_btn(int i,int pointX,int pointY)
{
	XuiWindow *pageSelectBtn;
	if(i == 0)
	{
		bf_set_width(140);
	}
	else if(i == 1)
	{
		bf_set_width(40);
	}
	else if(i == 2)
	{
		bf_set_width(40);
	}
	bf_set_height(settle_page_btn_height());
	bf_set_bg_color(settle_page_btn_bg_color_default());
	bf_set_bg_color_p(settle_page_btn_p_bg_color_default());
	bf_set_parent(settle_page_win());
	bf_set_x(pointX);
	bf_set_y(pointY);
	pageSelectBtn=create_xui_button();
	return pageSelectBtn;
}


int settle_page_btn_count() {return settle_page_btn_count_max();}
#undef SETTLE_PAGE_BUTTON_AREA



#define SETTLE_PAGE_REFRESH_AREA
static int settle_page_btn_right_gap() {return settle_page_btn_text_x();}
static int settle_page_btn_rest_width()
{
	return settle_page_btn_width()-settle_page_btn_text_x()-settle_page_btn_right_gap();
}
static int settle_page_btn_cal_text_width(char *text)
{
	return XuiTextWidth(get_text_font(),settle_page_btn_text_height(),text);
}

#define SETTLE_MENU_PRODUCT_SEPERATE_LINE
#define LINE_CONFIG_AREA
static int s_settle_product_seperate_line_btn_index=0;
int glSelectPageBtnFlag = 0; //选择相同产品进行加减不刷新加减按钮
static XuiWindow *selTxt = NULL;
static XuiWindow *selAdd = NULL;
static XuiWindow *selSub = NULL;
int cur_line_btn_set_index(int i) {s_settle_product_seperate_line_btn_index=i;return 0;}
int cur_line_btn_index() {return s_settle_product_seperate_line_btn_index;}
XuiWindow *cur_line_btn() {return settle_page_btn(cur_line_btn_index());}
int settle_product_seperate_line_width() {return cur_line_btn()->width;}
int settle_product_seperate_line_height() {return 2*pu();}
int settle_product_seperate_line_offset() {return (settle_page_btn_y_gap()-settle_product_seperate_line_height())/2;}
int settle_product_seperate_line_y_base() {return settle_page_btn_y(cur_line_btn_index())+cur_line_btn()->height;}
int settle_product_seperate_line_x() {return settle_page_btn_x(cur_line_btn_index());}
int settle_product_seperate_line_y() {return settle_product_seperate_line_y_base()+settle_product_seperate_line_offset();}
int settle_product_seperate_line_color() {return settle_title_win_color();}
static XuiImg *line_img()
{
	static XuiImg *s_img=NULL;
	if (!s_img) {
		int canvas_w=settle_page_btn_width();
		int canvas_h=settle_product_seperate_line_height();
		XuiWindow *canvas=XuiCreateCanvas(XuiRootCanvas(),0,0,canvas_w,canvas_h);
		XuiCanvasSetBackground(canvas,0,0,create_xui_color(settle_page_win_color()));

		int i;
		int step_count=30;
		int step_full_len=canvas_w/step_count;
		int step_len=step_full_len*1/2;
		for (i=0;step_full_len*i+step_len<canvas_w;++i) {
			XuiCanvasDrawRect(canvas,step_full_len*i,0,step_len,canvas_h,create_xui_color(settle_product_seperate_line_color()),0,1);
		}
		s_img=XuiCaptureCanvas(canvas,0,0,canvas_w,canvas_h);
		if (!s_img) {
			ERR("cap s_img error");
		}
		XuiDestroyWindow(canvas);
	}
	return s_img;
}
#undef LINE_CONFIG_AREA
 int settle_cur_page_draw_line()
{
	XuiImg *imgDottedline = XuiImgLoadFromFile(Add_Prefix_Res_Path("ga_dottedlinebl.png"));
	XuiCanvasDrawImg(settle_page_win(), settle_product_seperate_line_x(), settle_product_seperate_line_y(), XuiRootCanvas()->width - 38, 1,XUI_BG_CENTER, imgDottedline);	
	return 0;
}
 int payment_cur_page_draw_line()
{
	XuiImg *imgDottedline = XuiImgLoadFromFile(Add_Prefix_Res_Path("ga_dottedlinebl.png"));
	XuiCanvasDrawImg(payment_page_win(), settle_product_seperate_line_x(), settle_product_seperate_line_y(), XuiRootCanvas()->width - 38, 1,XUI_BG_CENTER, imgDottedline);	
	return 0;
}


static int settle_cur_page_clear_line()
{
	XuiClearArea(settle_page_win(),settle_product_seperate_line_x(),settle_product_seperate_line_y(),
		settle_product_seperate_line_width(),settle_product_seperate_line_height());
	return 0;
}
#undef SETTLE_MENU_PRODUCT_SEPERATE_LINE

static int settle_page_btn_blank_unit_size() {return settle_page_btn_cal_text_width(" ");}
char *settle_page_btn_text(void *node,char *outPutText)
{
	char s_text[256]={0};memset(s_text,0,sizeof(s_text));

	int product_count=product_node_get_count(node);
	char blank[128]={0};memset(blank,0,sizeof(blank));
	sprintf(s_text,"%d x %s %s %2d %s",product_count,node_get_name(node),"",product_node_get_price(node)*product_count,glCurrencyName);
	int base_width=settle_page_btn_cal_text_width(s_text);
	int unit_count=(settle_page_btn_rest_width()-base_width)/settle_page_btn_blank_unit_size();
	int i;
	if(unit_count >= 1)
	{
		unit_count -= 1;
	}
	for (i=0;i!=unit_count;++i) {blank[i]=' ';}
	sprintf(s_text,"%d x %s %s %2d %s",product_count,node_get_name(node),blank,product_node_get_price(node)*product_count,glCurrencyName);
	int new_width=settle_page_btn_cal_text_width(s_text);
	strcpy(outPutText,s_text);
	return NULL;
}

int DestorySelProductBut(int forceDestory)
{
	if(glSelectPageBtnFlag == 1 || forceDestory != 0)
	{
	  if(selTxt)
	  {
		XuiDestroyWindow(selTxt);
		selTxt = NULL;
	  }
	  if(selAdd)
	  {
		XuiDestroyWindow(selAdd);
		selAdd = NULL;
		
	   }
	   if(selSub)
	   {
	     XuiDestroyWindow(selSub);
		 selSub = NULL;
	   }
		
		glSelectPageBtnFlag = 0;
		Pax_Log(LOG_INFO,"destory page");
	}
	
	return 0;
}

int DestorySelButAndRefresh()  //强制清楚选择产品的按钮，并刷新界面
{
	Pax_Log(LOG_INFO,"hidehiset:%d",__LINE__);
	DestorySelProductBut(1);
	set_settle_page_key(settle_page_btn_virtual_Text_key_base());
	pthread_create(&selProductThread, NULL, SettleProductSel, NULL);
	WaitSelProThread(selProductThread);
	glSelectPageBtnFlag = 0;
    return 0;
}

int settle_page_refresh()
{
	
	//TIMER_START_DEF
	XuiShowWindow(settle_menu_win(),XUI_SHOW,0);
	int i;
	int j;
	int count=settle_page_btn_count();
	char szGetText[100];
	Pax_Log(LOG_INFO,"start to refresh");
	DestorySelProductBut(0);
	for (i=0;i!=count;++i) {
		XuiWindow *btn=settle_page_btn(i);
		int cur_page=settle_menu_get_cur_page();
		int node_index=i+cur_page*settle_page_btn_count();
		cur_line_btn_set_index(i);
		void *node=settle_product_list_node(node_index);
		if (node) {
		      Pax_Log(LOG_INFO,"line=%d",__LINE__);
			//set btn common stat
			XuiButtonStat stat_common;
			{
				memset(&stat_common,0,sizeof(XuiButtonStat));
				if (node_get_btn_text_flag(node)) {
				    memset(szGetText,0,sizeof(szGetText));
					settle_page_btn_text(node,szGetText);
					stat_common.text = szGetText;
					stat_common.text_x=settle_page_btn_text_x();
					stat_common.text_y=settle_page_btn_text_y();
					stat_common.text_height=settle_page_btn_text_height();
					stat_common.text_font=get_text_font();
				}
			}
			
			{//set btn normal stat
				XuiButtonStat stat;
				memcpy(&stat,&stat_common,sizeof(XuiButtonStat));
				//stat.text_fg=create_xui_color(node_get_btn_text_color(node));
				stat.text_fg=create_xui_color(0xff000000);
				
				stat.btn_bg=create_xui_color(node_get_btn_bg_color(node));
				XuiButtonSetStat(btn,XUI_BTN_NORMAL,&stat);
				 Pax_Log(LOG_INFO,"line=%d",__LINE__);
			}
			
			{//set btn pressed stat
				XuiButtonStat stat;
				memcpy(&stat,&stat_common,sizeof(XuiButtonStat));
				 Pax_Log(LOG_INFO,"line=%d",__LINE__);
				//stat.text_fg=create_xui_color(node_get_btn_text_color(node));
				stat.text_fg=create_xui_color(0xff000000);
				stat.btn_bg=create_xui_color(node_get_btn_bg_color_p(node));
				XuiButtonSetStat(btn,XUI_BTN_PRESSED,&stat);
			}
			XuiButtonSetKey(btn,node_index+settle_page_btn_virtual_key_base());
			XuiShowWindow(btn,XUI_SHOW,0);

			//画黑边
			settle_cur_page_draw_line();
			 //added by jeff 20170527
			//Pax_Log(LOG_INFO,"get_settle_page_key()=%d",get_settle_page_key());
		#if 1
		
		     Pax_Log(LOG_INFO,"line=%d,i=%d,get_settle_page_key()=%d",__LINE__,i,get_settle_page_key());
			if((get_settle_page_key() >=  settle_page_btn_virtual_Plus_key_base() && get_settle_page_key() < settle_page_btn_virtual_Sub_key_base() && (i == (get_settle_page_key() - settle_page_btn_virtual_Plus_key_base())%5) ) ||
			(get_settle_page_key() >= settle_page_btn_virtual_Sub_key_base() && (i == (get_settle_page_key() - settle_page_btn_virtual_Sub_key_base())%5) ) ||
			(get_settle_page_key() >= settle_page_btn_virtual_key_base() && get_settle_page_key() < 20000 && (i == (get_settle_page_key() - 10000)%5) ) )
			
			{
	  
				Pax_Log(LOG_INFO,"start to gen button Line:%d",__LINE__);
				XuiButtonStat selStat;
				XuiButtonStat selPressStat;
				XuiShowWindow(settle_page_btn(i),XUI_HIDE,0);
				for(j = 0;j < 3;j++)
				{
					memset(&selStat,0,sizeof(XuiButtonStat));
					if(j == 0)
					{
						if (node_get_btn_text_flag(node)) {
						    memset(szGetText,0,sizeof(szGetText));
							settle_page_btn_text(node,szGetText);
							selStat.text=szGetText;
							selStat.text_x=settle_page_btn_text_x();
							selStat.text_y=settle_page_btn_text_y();
							selStat.text_height=settle_page_btn_text_height();
							selStat.text_font=get_text_font();
						}
						selStat.text_fg=create_xui_color(node_get_btn_text_color(node));
						selStat.btn_bg=create_xui_color(node_get_btn_bg_color(node));
						if(selTxt == NULL)
						{
							selTxt = settle_page_select_btn(0,settle_page_btn_x(i),settle_page_btn_y(i));
						}
						XuiButtonSetStat(selTxt,XUI_BTN_NORMAL,&selStat);		
					}
			        else if(j == 1)
			        {
			        	if(selAdd == NULL)
			        	{
							selAdd = settle_page_select_btn(1,settle_page_btn_x(i) + 180,settle_page_btn_y(i));
						}
						selStat.img = settle_keypad_btn_plus_img();
						selStat.img_x = 12;
						selStat.img_y = 10;
						selStat.btn_bg=create_xui_color(node_get_btn_bg_color(node));
						XuiButtonSetStat(selAdd,XUI_BTN_NORMAL,&selStat);		
			        }
			        else if(j == 2)
			        {
			        	if(selSub == NULL)
						{	
							selSub = settle_page_select_btn(2,settle_page_btn_x(i) + 140,settle_page_btn_y(i));
						}
					    selStat.btn_bg=create_xui_color(node_get_btn_bg_color(node));
			        	
			        	selStat.img = settle_keypad_btn_sub_img();
						selStat.img_x = 12;
						
						selStat.img_y = 10;
						XuiButtonSetStat(selSub,XUI_BTN_NORMAL,&selStat);	
			        }
				
					
			//set btn pressed stat
				memset(&selPressStat,0,sizeof(XuiButtonStat));
				if(j == 0)
				{	memset(&selPressStat,0,sizeof(XuiButtonStat));
					selPressStat.text_fg=create_xui_color(0xff000000);
				
					//selPressStat.text_fg=create_xui_color(node_get_btn_text_color(node));
					XuiButtonSetKey(selTxt,settle_page_btn_virtual_Text_key_base());
					selPressStat.btn_bg=create_xui_color(node_get_btn_bg_color_p(node));
					XuiShowWindow(selTxt,XUI_SHOW,0);
					XuiButtonSetStat(selTxt,XUI_BTN_PRESSED,&selPressStat);
				}
				else if(j == 1)
				{
					selPressStat.img = settle_keypad_btn_plus_img();
					selPressStat.img_x = 12;
					selPressStat.img_y = 10;
					XuiButtonSetKey(selAdd,settle_page_btn_virtual_Plus_key_base() + node_index);
					XuiShowWindow(selAdd,XUI_SHOW,0);
					selPressStat.btn_bg=create_xui_color(node_get_btn_bg_color_p(node));
					XuiButtonSetStat(selAdd,XUI_BTN_PRESSED,&selPressStat);
				}
				else if(j == 2)
				{
					selPressStat.img = settle_keypad_btn_sub_img();
					selPressStat.img_x = 12;
					selPressStat.img_y = 10;
					selPressStat.btn_bg=create_xui_color(node_get_btn_bg_color_p(node));
					XuiButtonSetKey(selSub,settle_page_btn_virtual_Sub_key_base() + node_index);
					XuiShowWindow(selSub,XUI_SHOW,0);
					XuiButtonSetStat(selSub,XUI_BTN_PRESSED,&selPressStat);
			    }
			} //for
			glSelectPageBtnFlag = 0;
			}//if
	#endif
		} else {
			settle_cur_page_clear_line();
			XuiShowWindow(btn,XUI_HIDE,0);
			node=node_get_next_brother_node(node);
		}
	}
	//TIMER_CHECK
	return 0;
}

#undef SETTLE_PAGE_REFRESH_AREA

static int ssettlePageKey;

int get_settle_page_key()
{
	return ssettlePageKey;
}

int set_settle_page_key(int key)
{
	ssettlePageKey = key;
}


int settle_page_process()
{
	
	ST_TIMER timer={0};
	OsTimerSet(&timer,book_menu_exit_delay_ms());
	int node_index;
	while (1) {
		int key=0;
		if (XuiHasKey()) {
			key=XuiGetKey(); 
			set_settle_page_key(key);
			Pax_Log(LOG_INFO,"set key=%d",key);
			if(key >= settle_page_btn_virtual_key_base() && key < 20000)
			{
				glSelectPageBtnFlag = 1; 
				Pax_Log(LOG_INFO,"set glSelectPageBtnFlag=1");
			}

		} else {
			if (OsTimerCheck(&timer)==0) {return page_code_timeout_eixt();}
			continue;
		}
		OsTimerSet(&timer,book_menu_exit_delay_ms());

		{//check whether select function
			if (key==key_code_back()) {
				int cur_page=settle_menu_get_cur_page();
				if (cur_page>0) {
					settle_menu_set_cur_page(--cur_page);
					return page_code_to_prev_page();
				} else {
					settle_page_process_set_select_node(NULL);
					return page_code_back();
				}
			}
			if (key==key_code_ensure()) {
				int cur_page=settle_menu_get_cur_page();
				if (cur_page<settle_menu_get_page_count()-1) {
					settle_menu_set_cur_page(++cur_page);
					return page_code_to_next_page();
				} else {
					settle_page_process_set_select_node(NULL);
					return page_code_ensure();
				}
			}
			if (key==key_code_delete()) {
				return page_code_delete();
			}
			if (key==key_code_add()) {
				return key_code_add();
			}
			
		}
		if(key == settle_page_btn_virtual_Text_key_base())
		{
			Pax_Log(LOG_INFO,"key=text");
			continue;
			
		}
		else if(key >= settle_page_btn_virtual_Plus_key_base())
		{ 
		   if(glStartSelProductThread == 1)
		   { 
				Pax_Log(LOG_INFO,"wait thread to stop");
				continue;
			}
			else
			{
				Pax_Log(LOG_INFO,"dont need to wait thread");
				return key;
			}
		}

		{//check whether select item
			void *node=NULL;
			if (key_code_is_touch_button(key)) {
				node_index=key-settle_page_btn_virtual_key_base();
				node=settle_product_list_node(node_index);
			}
			if (key_code_is_num(key)) {
				int page_seq=key-key_code_num_base();
				int cur_page=settle_menu_get_cur_page();
				int node_index=cur_page*book_page_btn_count_max()+page_seq;
				node=settle_product_list_node(node_index);
			}
			if (node) {
				settle_page_process_set_select_node(node);
				sNodeIndex = node_index;
				sNode = node;
				return page_code_select_product();
			}
			
		}
	}
	
	return page_code_none();
}



#endif//__PROLIN__
