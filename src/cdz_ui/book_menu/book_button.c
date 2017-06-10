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


#define BOOK_KEYPAD_BUTTON_AREA

#define KEYPAD_BTN_LIST
static int s_keypad_btn_count;
XuiWindow *book_keypad_btn_index(int i)
{
	static XuiWindow *s_btn_list[64]={0};
	static int first=1;
	if (first) {
		int i;
		int keypad_max_btn_count=book_keypad_btn_count_max();
		bf_set_width(book_keypad_btn_width());
		bf_set_height(book_keypad_btn_height());
		bf_set_parent(book_keypad_win_addr());
		for (i=0;i!=keypad_max_btn_count;++i) {
			int x_seq=i%book_keypad_btn_count_x_max();
			int y_seq=i/book_keypad_btn_count_x_max();
			int btn_x=x_seq*book_keypad_btn_width() + (x_seq+1)*book_keypad_btn_x_gap();
			int btn_y=y_seq*book_keypad_btn_height() + (y_seq+1)*book_keypad_btn_y_gap();
			//Pax_Log(LOG_INFO,"btn_x=%d,btn_y=%d",btn_x,btn_y);
			bf_set_x(btn_x);
			bf_set_y(btn_y);
			s_btn_list[i]=create_xui_button();
		}		
		s_keypad_btn_count=keypad_max_btn_count;
		first=0;
	}
	return s_btn_list[i];
}
int keypad_btn_count()
{
	book_keypad_btn_index(0);
	return s_keypad_btn_count;
}
#undef KEYPAD_BTN_LIST

#define BOOK_KEYPAD_BTN_COMMON_CONFIG
int book_keypad_btn_text_height() {return keypad_btn_text_height();}
int book_keypad_btn_text_gap() {return keypad_btn_text_gap();}
XuiFont *book_keypad_btn_text_font() {return keypad_btn_text_font();}
int book_keypad_btn_text_color() {return keypad_btn_text_color();}
int book_keypad_btn_back_bg_color() {return keypad_btn_bg_color();}
int book_keypad_btn_ensure_bg_color() {return keypad_btn_bg_color();}
#undef BOOK_KEYPAD_BTN_COMMON_CONFIG

#define BOOK_KEYPAD_BTN_BACK_ATTR
int book_keypad_btn_back_locate_style() {return keypad_locate_style_a();}
int book_keypad_btn_back_key() {return key_code_back();}
int book_keypad_btn_back_btn_list_index() {return 0;}
int book_keypad_btn_back_text_gap() {return book_keypad_btn_text_gap();}
XuiFont *book_keypad_btn_back_text_font() {return book_keypad_btn_text_font();}
char *book_keypad_btn_back_text() {return (book_menu_get_cur_page_node()!=book_menu_first_node()) ? ("Back") : ("Load");}
int book_keypad_btn_back_text_color() {return  book_keypad_btn_text_color();}
int book_keypad_btn_back_text_height() {return book_keypad_btn_text_height();}
int book_keypad_btn_back_text_width() {return XuiTextWidth(book_keypad_btn_back_text_font(),book_keypad_btn_back_text_height(),book_keypad_btn_back_text());}
char *book_keypad_btn_back_img_path() {return keypad_btn_back_img_path();}
char *book_keypad_btn_update_img_path() {return keypad_btn_update_img_path();}

XuiImg *book_keypad_btn_back_img() {static XuiImg *s=NULL;return (!s) ? (s=XuiImgLoadFromFile(book_keypad_btn_back_img_path())) : (s);}
XuiImg *book_keypad_btn_update_img() {static XuiImg *s=NULL;return (!s) ? (s=XuiImgLoadFromFile(book_keypad_btn_update_img_path())) : (s);}

int book_keypad_btn_back_img_width() {XuiImg *img=book_keypad_btn_back_img();return (img) ? (img->width) : (0);}
int book_keypad_btn_back_img_height() {XuiImg *img=book_keypad_btn_back_img();return (img) ? (img->height) : (0);}
int book_keypad_btn_back_img_x()
{
	if (book_keypad_btn_back_locate_style()==keypad_locate_style_b()) {
		return book_keypad_btn_width()-book_keypad_btn_back_text_gap()-book_keypad_btn_back_img_width();
	}
	return book_keypad_btn_back_text_gap();
}
int book_keypad_btn_back_img_y() {return (book_keypad_btn_height()-book_keypad_btn_back_img_height())/2;}
int book_keypad_btn_back_text_x()
{
	if (book_keypad_btn_back_locate_style()==keypad_locate_style_b()) {
		return book_keypad_btn_width()-2*book_keypad_btn_back_text_gap()-book_keypad_btn_back_img_width()-book_keypad_btn_back_text_width();
	}
	return book_keypad_btn_back_img_x()+book_keypad_btn_back_img_width()+book_keypad_btn_back_text_gap();
}
int book_keypad_btn_back_text_y() {return (book_keypad_btn_height()-book_keypad_btn_back_text_height())/2;}
int book_keypad_btn_back_refresh()
{
	XuiWindow *btn=book_keypad_btn_index(book_keypad_btn_back_btn_list_index());
	XuiButtonSetKey(btn,book_keypad_btn_back_key());
	//set btn view
	XuiButtonStat stat;memset(&stat,0,sizeof(stat));
	stat.btn_bg=create_xui_color(book_keypad_btn_back_bg_color());			
	
	
	stat.text=book_keypad_btn_back_text();  //added by jeff_xiehuan 20170519
	if(strcmp(stat.text,"Back") == 0)
	{
		stat.img=book_keypad_btn_back_img();
		stat.img_x=book_keypad_btn_back_img_x();
		XuiShowWindow(btn,XUI_SHOW,0);
	}
	else if(strcmp(stat.text,"Load") == 0)
	{
		stat.img=book_keypad_btn_update_img();//remove "Load" button temporary
		stat.img_x= 0;
		XuiShowWindow(btn,XUI_HIDE,0);
		
	}
	
	stat.img_y=book_keypad_btn_back_img_y();
	stat.text_height=book_keypad_btn_back_text_height();
	stat.text_x=book_keypad_btn_back_text_x();
	stat.text_y=book_keypad_btn_back_text_y();
	stat.text_font=book_keypad_btn_back_text_font();
	stat.text_fg=create_xui_color(book_keypad_btn_back_text_color());
	XuiButtonSetStat(btn,XUI_BTN_NORMAL,&stat);
	XuiButtonSetStat(btn,XUI_BTN_PRESSED,&stat);

	return 0;
}
#undef BOOK_KEYPAD_BTN_BACK_ATTR

#define BOOK_KEYPAD_BTN_ENSURE_ATTR
int book_keypad_btn_ensure_locate_style() {return keypad_locate_style_b();}
int book_keypad_btn_ensure_key() {return key_code_ensure();}
int book_keypad_btn_ensure_btn_list_index() {return 3;}
int book_keypad_btn_ensure_text_gap() {return book_keypad_btn_text_gap();}
XuiFont *book_keypad_btn_ensure_text_font() {return book_keypad_btn_text_font();}
char *book_keypad_btn_ensure_text() {return "Next";}
int book_keypad_btn_ensure_text_color() {return  book_keypad_btn_text_color();}
int book_keypad_btn_ensure_text_height() {return book_keypad_btn_text_height();}
int book_keypad_btn_ensure_text_width() {return XuiTextWidth(book_keypad_btn_ensure_text_font(),book_keypad_btn_ensure_text_height(),book_keypad_btn_ensure_text());}
char *book_keypad_btn_ensure_img_path() {return keypad_btn_ensure_img_path();}
XuiImg *book_keypad_btn_ensure_img() {static XuiImg *s=NULL;return (!s) ? (s=XuiImgLoadFromFile(book_keypad_btn_ensure_img_path())) : (s);}
int book_keypad_btn_ensure_img_width() {XuiImg *img=book_keypad_btn_ensure_img();return (img) ? (img->width) : (0);}
int book_keypad_btn_ensure_img_height() {XuiImg *img=book_keypad_btn_ensure_img();return (img) ? (img->height) : (0);}
int book_keypad_btn_ensure_img_x()
{
	if (book_keypad_btn_ensure_locate_style()==keypad_locate_style_b()) {
		return book_keypad_btn_width()-book_keypad_btn_ensure_text_gap()-book_keypad_btn_ensure_img_width();
	}
	return book_keypad_btn_ensure_text_gap();
}
int book_keypad_btn_ensure_img_y() {return (book_keypad_btn_height()-book_keypad_btn_ensure_img_height())/2;}
int book_keypad_btn_ensure_text_x()
{
	if (book_keypad_btn_ensure_locate_style()==keypad_locate_style_b()) {
		return book_keypad_btn_width()-2*book_keypad_btn_ensure_text_gap()-book_keypad_btn_ensure_img_width()-book_keypad_btn_ensure_text_width();
	}
	return book_keypad_btn_ensure_img_x()+book_keypad_btn_ensure_img_width()+book_keypad_btn_ensure_text_gap();
}
int book_keypad_btn_ensure_text_y() {return (book_keypad_btn_height()-book_keypad_btn_ensure_text_height())/2;}
int book_keypad_btn_ensure_refresh()
{
	XuiWindow *btn=book_keypad_btn_index(book_keypad_btn_ensure_btn_list_index());
	XuiButtonSetKey(btn,book_keypad_btn_ensure_key());
	//set btn view
	XuiButtonStat stat;memset(&stat,0,sizeof(stat));
	stat.btn_bg=create_xui_color(book_keypad_btn_ensure_bg_color());			
	stat.img=book_keypad_btn_ensure_img();
	stat.img_x=book_keypad_btn_ensure_img_x();
	stat.img_y=book_keypad_btn_ensure_img_y();
	
	stat.text=book_keypad_btn_ensure_text();
	stat.text_height=book_keypad_btn_ensure_text_height();
	stat.text_x=book_keypad_btn_ensure_text_x();
	stat.text_y=book_keypad_btn_ensure_text_y();
	stat.text_font=book_keypad_btn_ensure_text_font();
	stat.text_fg=create_xui_color(book_keypad_btn_ensure_text_color());
	XuiButtonSetStat(btn,XUI_BTN_NORMAL,&stat);
	XuiButtonSetStat(btn,XUI_BTN_PRESSED,&stat);

	return 0;
}
#undef BOOK_KEYPAD_BTN_ENSURE_ATTR

#undef BOOK_KEYPAD_BUTTON_AREA



#endif//__PROLIN__

