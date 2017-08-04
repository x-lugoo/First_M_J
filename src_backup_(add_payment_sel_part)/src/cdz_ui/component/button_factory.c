#include "sm_all.h"
#ifdef __PROLIN__

#include <xui.h>
#include <stdlib.h>

#define COMMON_XUI_REF_AREA
XuiColor create_xui_color(int argb_hex)
{
	XuiColor xui_color={0};
	xui_color.a=(argb_hex&0xff000000)>>3*8;
	xui_color.r=(argb_hex&0x00ff0000)>>2*8;
	xui_color.g=(argb_hex&0x0000ff00)>>1*8;
	xui_color.b=(argb_hex&0x000000ff)>>0*8;
	return xui_color;
}
int create_argb_color(XuiColor xui_color)
{
	int argb=0x00000000;
	argb|=(xui_color.a)>>3*8;

	argb|=(xui_color.a)>>3*8;
	argb|=(xui_color.r)>>2*8;
	argb|=(xui_color.g)>>1*8;
	argb|=(xui_color.b)>>0*8;
}
#undef COMMON_XUI_REF_AREA

#define INNER_MEMBER_AREA
typedef struct button_config_t {
	XuiWindow *parent;
	int x;
	int y;
	int width;
	int height;

	int bg_color;
	int bg_color_p;

	XuiFont *text_font;
	char *text;
	int text_color;
	int text_color_p;
	int text_x;
	int text_y;
	int text_height;

	int key;

	XuiImg *img;
	int img_x;
	int img_y;
	XuiImg *img_p;
	int img_x_p;
	int img_y_p;
} BC_T;

static BC_T s_bf_raw;
static BC_T *s_bf=&s_bf_raw;
#undef INNER_MEMBER_AREA

#define OUTPUT_AREA

int bf_to_default()
{
	memset(s_bf,0,sizeof(BC_T));
	s_bf->bg_color=0xff000000;
	return 0;
}

XuiWindow *create_xui_button()
{
	XuiWindow *canvas=XuiCreateButton(s_bf->parent,s_bf->x,s_bf->y,s_bf->width,s_bf->height);
	XuiButtonStat stat;
	memset(&stat,0,sizeof(XuiButtonStat));
	stat.btn_bg=create_xui_color(s_bf->bg_color);
	stat.text=s_bf->text;
	stat.text_font=s_bf->text_font;
	stat.text_fg=create_xui_color(s_bf->text_color);
	stat.text_height=s_bf->text_height;
	stat.text_y=s_bf->text_y;
	stat.text_x=s_bf->text_x;
	stat.img=s_bf->img;
	stat.img_x=s_bf->img_x;
	stat.img_y=s_bf->img_y;
	XuiButtonSetStat(canvas,XUI_BTN_NORMAL,&stat);

	stat.text_fg=create_xui_color(s_bf->text_color_p);
	stat.btn_bg=create_xui_color(s_bf->bg_color_p);
	stat.img=s_bf->img_p;
	stat.img_x=s_bf->img_x_p;
	stat.img_y=s_bf->img_y_p;
	XuiButtonSetStat(canvas,XUI_BTN_PRESSED,&stat);
	
	XuiButtonSetKey(canvas,s_bf->key);
	XuiShowWindow(canvas,XUI_SHOW,0);

	return canvas;
}


int bf_set_text(char *text) {s_bf->text=text;return 0;}
int bf_set_x(int x) {s_bf->x=x;return 0;}
int bf_set_y(int y) {s_bf->y=y;return 0;}
int bf_set_width(int width) {s_bf->width=width;return 0;}
int bf_set_height(int height) {s_bf->height=height;return 0;}
int bf_set_parent(XuiWindow *parent) {s_bf->parent=parent;return 0;}
int bf_set_text_font(XuiFont *text_font) {s_bf->text_font=text_font;return 0;}
int bf_set_text_height(int text_height) {s_bf->text_height=text_height;return 0;}
int bf_set_text_y(int text_y) {s_bf->text_y=text_y;return 0;}
int bf_set_text_x(int text_x) {s_bf->text_x=text_x;return 0;}
int bf_set_key(int key) {s_bf->key=key;return 0;}
int bf_set_text_color(int argb) {s_bf->text_color=argb;return 0;}
int bf_set_bg_color(int argb) {s_bf->bg_color=argb;return 0;}
int bf_set_text_color_p(int argb) {s_bf->text_color_p=argb;return 0;}
int bf_set_bg_color_p(int argb) {s_bf->bg_color_p=argb;return 0;}
#undef OUTPUT_AREA




#define EXTERN_OUTPUT_AREA
int bf_set_img(XuiImg *img) {s_bf->img=img;return 0;}
int bf_set_img_p(XuiImg *img_p) {s_bf->img_p=img_p;return 0;}
int bf_set_img_x(int x) {s_bf->img_x=x;return 0;}
int bf_set_img_y(int y) {s_bf->img_y=y;return 0;}
int bf_set_img_x_p(int x_p) {s_bf->img_x_p=x_p;return 0;}
int bf_set_img_y_p(int y_p) {s_bf->img_y_p=y_p;return 0;}
#undef EXTERN_OUTPUT_AREA


#endif//ifdef _PROLIN2_4_

