#include "sm_all.h"
#ifdef __PROLIN__




#include <xui.h>
#include <osal.h>
#include <stdlib.h>
#include <string.h>
#include "pax_log.h"

#define REF_AREA
XuiColor create_xui_color(int argb_hex);
#undef REF_AREA



int page_code_none() {return 0;}
int page_code_to_next_page() {return 1;}
int page_code_to_prev_page() {return 2;}
int page_code_to_father_page() {return 3;}
int page_code_user_exit() {return 4;}
int page_code_error_exit() {return 5;}
int page_code_timeout_eixt() {return 6;}
int page_code_switch_to_menu() {return 7;}
int page_code_to_son_page() {return 8;}
int page_code_select_product() {return 9;}
int page_code_ensure() {return 10;}
int page_code_back() {return 11;}
int page_code_delete() {return 12;}
int page_code_select_varPrice() { return 14;}//added by jeff_xiehuan20170522
int page_code_exit() {return 13;}
int page_code_func() {return 15;}//added by jeff_xiehuan20170606



#endif//__PROLIN__
