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


//int key_code_exit() {return XUI_KEYCANCEL;}
int key_code_to_next() {return XUI_KEYALPHA;}
int key_code_to_prev() {return XUI_KEYFUNC;}
int key_code_is_num(int key) {return (key>=XUI_KEY1 && key<=XUI_KEY9) ? (1) : (0);}
int key_code_num_base() {return XUI_KEY1;}
int key_code_is_touch_button(int key) {return (key>=book_page_btn_key_base() && key < 20000) ? (1) : (0);}
int key_code_back() {return XUI_KEYCANCEL;}
int key_code_ensure() {return XUI_KEYENTER;}
int key_code_delete() {return XUI_KEYCLEAR;}
int key_code_add() {return XUI_KEYALPHA;}

#define KEYPAD_BTN_AREA
int keypad_btn_text_height() {return 16;}

int keypad_btn_text_gap() {return 2*pu();}
XuiFont *keypad_btn_text_font() {return get_text_font();}
int keypad_btn_text_color() {return  0xff000000;}
int keypad_btn_bg_color() {return minipos_page_background_color();}
char *keypad_btn_back_img_path() {return "./res/keypad/back.png";}
char *keypad_btn_update_img_path() {return "./res/keypad/update.png";}


char *keypad_btn_ensure_img_path() {return "./res/keypad/ensure.png";}
char *keypad_btn_delete_img_path() {return "./res/keypad/back.png";}
char *keypad_btn_add_img_path() {return "./res/keypad/add.png";}
char *keypad_btn_plus_img_path() {return "./res/keypad/plus.png";}
char *keypad_btn_sub_img_path() {return "./res/keypad/sub.png";}






#undef KEYPAD_BTN_AREA

#endif//__PROLIN__
