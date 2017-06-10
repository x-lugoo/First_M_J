#include "sm_all.h"
#ifdef __PROLIN__

#include <xui.h>
#include <stdlib.h>
#include "pax_log.h"

#define REF_AREA
XuiColor create_xui_color(int argb_hex);
#undef REF_AREA


int keypad_locate_style_a() {return 1;}
int keypad_locate_style_b() {return 2;}
#endif//__PROLIN__