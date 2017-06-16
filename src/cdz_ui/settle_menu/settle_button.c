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



#define BTN_CONFIG
int settle_keypad_btn_text_height() {return keypad_btn_text_height();}
int settle_keypad_btn_text_gap() {return keypad_btn_text_gap();}
XuiFont *settle_keypad_btn_text_font() {return keypad_btn_text_font();}
int settle_keypad_btn_text_color() {return keypad_btn_text_color();}
int settle_keypad_btn_back_bg_color() {return keypad_btn_bg_color();}
int settle_keypad_btn_ensure_bg_color() {return keypad_btn_bg_color();}
int settle_keypad_btn_delete_bg_color() {return settle_page_btn_bg_color_default();}
int settle_keypad_btn_add_bg_color() {return settle_page_btn_bg_color_default();}

#undef BTN_CONFIG



#define KEYPAD_BTN_AREA
static int s_settle_keypad_btn_count;
XuiWindow *settle_keypad_btn_index(int i)
{
	static XuiWindow *s_btn_list[64]={0};
	static int first=1;
	if (first) {
		int i;
		int settle_keypad_max_btn_count=settle_keypad_btn_count_x_max()*settle_keypad_btn_count_y_max();
		bf_set_width(settle_keypad_btn_width());
		bf_set_height(settle_keypad_btn_height());
		bf_set_bg_color(settle_page_btn_bg_color_default());
		bf_set_bg_color_p(settle_page_btn_p_bg_color_default());
		bf_set_parent(settle_keypad_win_addr());
		for (i=0;i!=settle_keypad_max_btn_count;++i) {
			int x_seq=i%settle_keypad_btn_count_x_max();
			int y_seq=i/settle_keypad_btn_count_x_max();
			int btn_x=x_seq*settle_keypad_btn_width() + (x_seq+1)*settle_keypad_btn_x_gap();
			int btn_y=y_seq*settle_keypad_btn_height() + (y_seq+1)*settle_keypad_btn_y_gap();
            if(i == 1)
            {
                btn_x += 20;
            	bf_set_width(settle_keypad_btn_width() + 20);
            }
            else if(i == 2)
            {
            	bf_set_width(0);
            }
            else if(i == 3)
            {
            	btn_x -= 24;
            	bf_set_width(settle_keypad_btn_width() + 34);
            }
			//PaxLog(LOG_INFO,"btn[%d]<x_seq:%d, y_seq:%d> <x:%d,y:%d,w:%d,h:%d>",i,x_seq,y_seq,btn_x,btn_y,settle_keypad_btn_width(),settle_keypad_btn_height());
			bf_set_x(btn_x);
			bf_set_y(btn_y);
			s_btn_list[i]=create_xui_button();
		}		
		s_settle_keypad_btn_count=settle_keypad_max_btn_count;
		first=0;
	}
	return s_btn_list[i];
}
int settle_keypad_btn_count()
{
	settle_keypad_btn_index(0);
	return s_settle_keypad_btn_count;
}
XuiWindow *settle_keypad_btn_delete()
{
	static XuiWindow *s_btn=NULL;
	if (!s_btn) {
		s_btn=settle_keypad_btn_index(1);
	}
	return s_btn;
}
#undef KEYPAD_BTN_AREA

#define BTN_ATTR
int settle_keypad_btn_back_locate_style() {return keypad_locate_style_a();}
int settle_keypad_btn_back_key() {return key_code_back();}
int settle_keypad_btn_back_btn_list_index() {return 0;}
int settle_keypad_btn_back_text_gap() {return settle_keypad_btn_text_gap();}
XuiFont *settle_keypad_btn_back_text_font() {return settle_keypad_btn_text_font();}
char *settle_keypad_btn_back_text() {return "Back";}
int settle_keypad_btn_back_text_color() {return  settle_keypad_btn_text_color();}
int settle_keypad_btn_back_text_height() {return settle_keypad_btn_text_height();}
int settle_keypad_btn_back_text_width() {return XuiTextWidth(settle_keypad_btn_back_text_font(),settle_keypad_btn_back_text_height(),settle_keypad_btn_back_text());}
char *settle_keypad_btn_back_img_path() {return keypad_btn_back_img_path();}
char *settle_keypad_btn_update_img_path() {return keypad_btn_update_img_path();}

XuiImg *settle_keypad_btn_back_img() {static XuiImg *s=NULL;return (!s) ? (s=XuiImgLoadFromFile(settle_keypad_btn_back_img_path())) : (s);}
XuiImg *settle_keypad_btn_update_img() {static XuiImg *s=NULL;return (!s) ? (s=XuiImgLoadFromFile(settle_keypad_btn_update_img_path())) : (s);}

int settle_keypad_btn_back_img_width() {XuiImg *img=settle_keypad_btn_back_img();return (img) ? (img->width) : (0);}
int settle_keypad_btn_back_img_height() {XuiImg *img=settle_keypad_btn_back_img();return (img) ? (img->height) : (0);}
int settle_keypad_btn_back_img_x()
{
	if (settle_keypad_btn_back_locate_style()==keypad_locate_style_b()) {
		return settle_keypad_btn_width()-settle_keypad_btn_back_text_gap()-settle_keypad_btn_back_img_width();
	}
	return settle_keypad_btn_back_text_gap();
}
int settle_keypad_btn_back_img_y() {return (settle_keypad_btn_height()-settle_keypad_btn_back_img_height())/2;}
int settle_keypad_btn_back_text_x()
{
	if (settle_keypad_btn_back_locate_style()==keypad_locate_style_b()) {
		return settle_keypad_btn_width()-2*settle_keypad_btn_back_text_gap()-settle_keypad_btn_back_img_width()-settle_keypad_btn_back_text_width();
	}
	return settle_keypad_btn_back_img_x()+settle_keypad_btn_back_img_width()+settle_keypad_btn_back_text_gap();
}
int settle_keypad_btn_back_text_y() {return (settle_keypad_btn_height()-settle_keypad_btn_back_text_height())/2;}
int settle_keypad_btn_back_refresh()
{
	XuiWindow *btn=settle_keypad_btn_index(settle_keypad_btn_back_btn_list_index());
	XuiButtonSetKey(btn,settle_keypad_btn_back_key());
	//set btn view
	XuiButtonStat stat;memset(&stat,0,sizeof(stat));
	stat.btn_bg=create_xui_color(settle_keypad_btn_back_bg_color());			
	stat.img=settle_keypad_btn_back_img();
	stat.img_x=settle_keypad_btn_back_img_x();
	stat.img_y=settle_keypad_btn_back_img_y();

	
	stat.text=settle_keypad_btn_back_text();
	stat.text_height=settle_keypad_btn_back_text_height();
	stat.text_x=settle_keypad_btn_back_text_x();
	stat.text_y=settle_keypad_btn_back_text_y();
	stat.text_font=settle_keypad_btn_back_text_font();
	stat.text_fg=create_xui_color(settle_keypad_btn_back_text_color());
	XuiButtonSetStat(btn,XUI_BTN_NORMAL,&stat);
	XuiButtonSetStat(btn,XUI_BTN_PRESSED,&stat);

	return 0;
}
#undef BTN_ATTR

#define BTN_ATTR
int settle_keypad_btn_ensure_locate_style() {return keypad_locate_style_b();}
int settle_keypad_btn_ensure_key() {return key_code_ensure();}
int settle_keypad_btn_ensure_btn_list_index() {return 3;}
int settle_keypad_btn_ensure_text_gap() {return settle_keypad_btn_text_gap();}
XuiFont *settle_keypad_btn_ensure_text_font() {return settle_keypad_btn_text_font();}
char *settle_keypad_btn_ensure_text() {return (settle_menu_get_cur_page()!=settle_menu_get_page_count()-1) ? ("Next") : ("Payment");}


int settle_keypad_btn_ensure_text_color() {return  settle_keypad_btn_text_color();}
int settle_keypad_btn_ensure_text_height() {return settle_keypad_btn_text_height();}
int settle_keypad_btn_ensure_text_width() {return XuiTextWidth(settle_keypad_btn_ensure_text_font(),settle_keypad_btn_ensure_text_height(),settle_keypad_btn_ensure_text());}
char *settle_keypad_btn_ensure_img_path() {return keypad_btn_ensure_img_path();}
XuiImg *settle_keypad_btn_ensure_img() {static XuiImg *s=NULL;return (!s) ? (s=XuiImgLoadFromFile(settle_keypad_btn_ensure_img_path())) : (s);}
int settle_keypad_btn_ensure_img_width() {XuiImg *img=settle_keypad_btn_ensure_img();return (img) ? (img->width) : (0);}
int settle_keypad_btn_ensure_img_height() {XuiImg *img=settle_keypad_btn_ensure_img();return (img) ? (img->height) : (0);}
int settle_keypad_btn_ensure_img_x()
{
	if (settle_keypad_btn_ensure_locate_style()==keypad_locate_style_b()) {
		return settle_keypad_btn_width()-settle_keypad_btn_ensure_text_gap()-settle_keypad_btn_ensure_img_width();
	}
	return settle_keypad_btn_ensure_text_gap();
}
int settle_keypad_btn_ensure_img_y() {return (settle_keypad_btn_height()-settle_keypad_btn_ensure_img_height())/2;}
int settle_keypad_btn_ensure_text_x()
{
	if (settle_keypad_btn_ensure_locate_style()==keypad_locate_style_b()) {
		return settle_keypad_btn_width()-2*settle_keypad_btn_ensure_text_gap()-settle_keypad_btn_ensure_img_width()-settle_keypad_btn_ensure_text_width();
	}
	return settle_keypad_btn_ensure_img_x()+settle_keypad_btn_ensure_img_width()+settle_keypad_btn_ensure_text_gap();
}
int settle_keypad_btn_ensure_text_y() {return (settle_keypad_btn_height()-settle_keypad_btn_ensure_text_height())/2;}
int settle_keypad_btn_ensure_refresh()
{
	XuiWindow *btn=settle_keypad_btn_index(settle_keypad_btn_ensure_btn_list_index());
	if(settle_product_count() == 0)
	{
		XuiShowWindow(btn,XUI_HIDE,0);
	}
	else
	{
		XuiShowWindow(btn,XUI_SHOW,0);
	}
	XuiButtonSetKey(btn,settle_keypad_btn_ensure_key());
	//set btn view
	XuiButtonStat stat;memset(&stat,0,sizeof(stat));
	stat.btn_bg=create_xui_color(settle_keypad_btn_ensure_bg_color());			
	stat.img=settle_keypad_btn_ensure_img();
	stat.img_x = settle_keypad_btn_width() + 9;
	stat.img_y=settle_keypad_btn_ensure_img_y();
	
	stat.text=settle_keypad_btn_ensure_text();
	stat.text_height=settle_keypad_btn_ensure_text_height();
	stat.text_x=0;
	if(strcmp(stat.text,"Next") == 0)
	{
		stat.text_x = 27;
	}
	stat.text_y=settle_keypad_btn_ensure_text_y();
	stat.text_font=settle_keypad_btn_ensure_text_font();
	stat.text_fg=create_xui_color(settle_keypad_btn_ensure_text_color());
	XuiButtonSetStat(btn,XUI_BTN_NORMAL,&stat);
	XuiButtonSetStat(btn,XUI_BTN_PRESSED,&stat);

	return 0;
}
#undef BTN_ATTR

#define BTN_ATTR
int settle_keypad_btn_delete_locate_style() {return keypad_locate_style_b();}
int settle_keypad_btn_delete_key() {return key_code_delete();}
int settle_keypad_btn_add_key() {return key_code_add();}

int settle_keypad_btn_delete_btn_list_index() {return 1;}
int settle_keypad_btn_delete_text_gap() {return settle_keypad_btn_text_gap();}
XuiFont *settle_keypad_btn_delete_text_font() {return settle_keypad_btn_text_font();}
char *settle_keypad_btn_delete_text() {return "Delete";}
int settle_keypad_btn_delete_text_color() {return  settle_keypad_btn_text_color();}
int settle_keypad_btn_delete_text_height() {return settle_keypad_btn_text_height();}
int settle_keypad_btn_delete_text_width() {return XuiTextWidth(settle_keypad_btn_delete_text_font(),settle_keypad_btn_delete_text_height(),settle_keypad_btn_delete_text());}
char *settle_keypad_btn_delete_img_path() {return keypad_btn_delete_img_path();}
XuiImg *settle_keypad_btn_delete_img() {static XuiImg *s=NULL;return (!s) ? (s=XuiImgLoadFromFile(settle_keypad_btn_delete_img_path())) : (s);}
int settle_keypad_btn_delete_img_width() {XuiImg *img=settle_keypad_btn_delete_img();return (img) ? (img->width) : (0);}
int settle_keypad_btn_delete_img_height() {XuiImg *img=settle_keypad_btn_delete_img();return (img) ? (img->height) : (0);}
int settle_keypad_btn_delete_img_x()
{
	if (settle_keypad_btn_delete_locate_style()==keypad_locate_style_b()) {
		return settle_keypad_btn_width()-settle_keypad_btn_delete_text_gap()-settle_keypad_btn_delete_img_width();
	}
	return settle_keypad_btn_delete_text_gap();
}
int settle_keypad_btn_delete_img_y() {return (settle_keypad_btn_height()-settle_keypad_btn_delete_img_height())/2;}
int settle_keypad_btn_delete_text_x()
{
	if (settle_keypad_btn_delete_locate_style()==keypad_locate_style_b()) {
		return settle_keypad_btn_width()-2*settle_keypad_btn_delete_text_gap()-settle_keypad_btn_delete_img_width()-settle_keypad_btn_delete_text_width();
	}
	return settle_keypad_btn_delete_img_x()+settle_keypad_btn_delete_img_width()+settle_keypad_btn_delete_text_gap();
}
int settle_keypad_btn_delete_text_y() {return (settle_keypad_btn_height()-settle_keypad_btn_delete_text_height())/2;}
int settle_keypad_btn_delete_refresh()
{
	XuiWindow *btn=settle_keypad_btn_index(settle_keypad_btn_delete_btn_list_index());
	XuiButtonSetKey(btn,settle_keypad_btn_delete_key());
	//set btn view
	XuiButtonStat stat;memset(&stat,0,sizeof(stat));
	stat.btn_bg=create_xui_color(settle_keypad_btn_delete_bg_color());			
	stat.img = NULL;//remove delete icon
	stat.img_y=settle_keypad_btn_delete_img_y();
	stat.img_x=50;
	stat.text=settle_keypad_btn_delete_text();
	stat.text_height=settle_keypad_btn_delete_text_height();
	stat.text_x=settle_keypad_btn_delete_text_x();
	stat.text_x=0;
	stat.text_y=settle_keypad_btn_delete_text_y();
	stat.text_font=settle_keypad_btn_delete_text_font();
	stat.text_fg=create_xui_color(settle_keypad_btn_delete_text_color());
	XuiButtonSetStat(btn,XUI_BTN_NORMAL,&stat);
	XuiButtonSetStat(btn,XUI_BTN_PRESSED,&stat);

	return 0;
}

#undef BTN_ATTR
//add by jeff
#define BTN_ATTR
int settle_keypad_btn_add_locate_style() {return keypad_locate_style_b();}
int settle_keypad_btn_add_btn_list_index() {return 2;}
int settle_keypad_btn_add_text_gap() {return settle_keypad_btn_text_gap();}
XuiFont *settle_keypad_btn_add_text_font() {return settle_keypad_btn_text_font();}
char *settle_keypad_btn_add_text() {return "";}
int settle_keypad_btn_add_text_color() {return  settle_keypad_btn_text_color();}
int settle_keypad_btn_add_text_height() {return settle_keypad_btn_text_height();}
int settle_keypad_btn_add_text_width() {return XuiTextWidth(settle_keypad_btn_add_text_font(),settle_keypad_btn_add_text_height(),settle_keypad_btn_add_text());}
char *settle_keypad_btn_add_img_path() {return keypad_btn_add_img_path();}
char *settle_keypad_btn_sub_img_path() {return keypad_btn_sub_img_path();}
char *settle_keypad_btn_plus_img_path() {return keypad_btn_plus_img_path();}


XuiImg *settle_keypad_btn_add_img() {static XuiImg *s=NULL;return (!s) ? (s=XuiImgLoadFromFile(settle_keypad_btn_add_img_path())) : (s);}

XuiImg *settle_keypad_btn_sub_img() {static XuiImg *s=NULL;return (!s) ? (s=XuiImgLoadFromFile(settle_keypad_btn_sub_img_path())) : (s);}

XuiImg *settle_keypad_btn_plus_img() {static XuiImg *s=NULL;return (!s) ? (s=XuiImgLoadFromFile(settle_keypad_btn_plus_img_path())) : (s);}


int settle_keypad_btn_add_img_width() {XuiImg *img=settle_keypad_btn_add_img();return (img) ? (img->width) : (0);}
int settle_keypad_btn_add_img_height() {XuiImg *img=settle_keypad_btn_add_img();return (img) ? (img->height) : (0);}
int settle_keypad_btn_add_img_x()
{
	if (settle_keypad_btn_add_locate_style()==keypad_locate_style_b()) {
		return settle_keypad_btn_width()-settle_keypad_btn_add_text_gap()-settle_keypad_btn_add_img_width();
	}
	return settle_keypad_btn_add_text_gap();
}
int settle_keypad_btn_add_img_y() {return (settle_keypad_btn_height()-settle_keypad_btn_add_img_height())/2;}
int settle_keypad_btn_add_text_x()
{
	if (settle_keypad_btn_add_locate_style()==keypad_locate_style_b()) {
		return settle_keypad_btn_width()-2*settle_keypad_btn_add_text_gap()-settle_keypad_btn_add_img_width()-settle_keypad_btn_add_text_width();
	}
	return settle_keypad_btn_add_img_x()+settle_keypad_btn_add_img_width()+settle_keypad_btn_add_text_gap();
}
int settle_keypad_btn_add_text_y() {return (settle_keypad_btn_height()-settle_keypad_btn_add_text_height())/2;}
int settle_keypad_btn_add_refresh()
{
	XuiWindow *btn=settle_keypad_btn_index(settle_keypad_btn_add_btn_list_index());
	XuiButtonSetKey(btn,settle_keypad_btn_add_key());
	//set btn view
	XuiButtonStat stat;memset(&stat,0,sizeof(stat));
	stat.btn_bg=create_xui_color(settle_keypad_btn_add_bg_color());			
	stat.img=settle_keypad_btn_add_img();
	stat.img_y=settle_keypad_btn_add_img_y();
	stat.img_x=10;
	stat.text=settle_keypad_btn_add_text();
	stat.text_height=settle_keypad_btn_add_text_height();
	stat.text_x=settle_keypad_btn_add_text_x();
	stat.text_y=settle_keypad_btn_add_text_y();
	stat.text_font=settle_keypad_btn_add_text_font();
	stat.text_fg=create_xui_color(settle_keypad_btn_add_text_color());
	XuiButtonSetStat(btn,XUI_BTN_NORMAL,&stat);
	XuiButtonSetStat(btn,XUI_BTN_PRESSED,&stat);

	return 0;
}
#undef BTN_ATTR


#endif//__PROLIN__
