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





#define OUTPUT_AREA
XuiFont *get_text_font()
{
	static XuiFont *s_font=NULL;
	static int first=1;
	if (first) {
		s_font=XuiCreateFont("./res/PTS75F.ttf",0,0);
		first=0;
	}
	return s_font;
}

XuiFont *get_xui_text_font()
{
	static XuiFont *s_font=NULL;
	static int first=1;
	if (first) {
		s_font=XuiCreateFont("./res/PTS55F.ttf",0,0);
		first=0;
	}
	return s_font;
}

#define PAGE_BUTTON_AREA


#define OUTPUT_AREA

#define CONFIG_SETTING_MENU_ID_ATTR
XuiImg *sm_get_num_icon(int num)
{
	char *num_icon_path[]={
		NULL,
		"./res/newui/seq/480X800/pr1.png",
		"./res/newui/seq/480X800/pr2.png",
		"./res/newui/seq/480X800/pr3.png",
		"./res/newui/seq/480X800/pr4.png",
		"./res/newui/seq/480X800/pr5.png",
		"./res/newui/seq/480X800/pr6.png",
		"./res/newui/seq/480X800/pr7.png",
		"./res/newui/seq/480X800/pr8.png",
		"./res/newui/seq/480X800/pr9.png",
		"./res/newui/seq/480X800/pr10.png",
	};
	int num_icon_count=sizeof(num_icon_path)/sizeof(char *);
	if (num>=num_icon_count) {return NULL;}
	
	static XuiImg *s_img_list[64];
	static int first=1;
	if (first) {
		int i=0;
		for (i!=0;i!=num_icon_count;++i) {
			char *path=num_icon_path[i];
			if (path) {
				s_img_list[i]=XuiImgLoadFromFile(path);
			}
		}
		first=0;
	}
	return s_img_list[num];
}
#undef CONFIG_SETTING_MENU_ID_ATTR

int get_screen_width() {return XuiRootCanvas()->width;}
int get_screen_height() {return (XuiStatusbarCanvas()) ? (XuiRootCanvas()->height-XuiStatusbarCanvas()->height) : (XuiRootCanvas()->height);}

int sort_int_array(int *arr,int count)
{
	int i;
	for (i=0;i!=count;++i) {
		int j;
		for (j=i;j!=count;++j) {
			if (arr[i]>arr[j]) {
				int temp=arr[i];
				arr[i]=arr[j];
				arr[j]=temp;
			}
		}
	}
	return 0;
}

int pu()
{
	static int s_pu=1;
	static int first=1;
	if (first) {
		if (get_screen_width()==480) {
			s_pu=2;
		} else {
			s_pu=1;
		}
		first=0;
	}
	return s_pu;

}

void *cmalloc(int size)
{
	void *ptr=malloc(size);
	if (ptr==NULL) {
		ERR("malloc fail");
		return NULL;
	}
	memset(ptr,0,size);
	return ptr;
}
#undef OUTPUT_AREA









#endif//ifdef _PROLIN2_4_

