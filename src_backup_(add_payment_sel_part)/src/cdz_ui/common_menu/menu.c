#include "sm_all.h"
#ifdef __PROLIN__

#define MENU_STAT_AREA
int menu_stat_null() {return 0;}
int menu_stat_book() {return 1;}
int menu_stat_settle() {return 2;}
static int s_menu_stat=0;
int get_menu_stat() {return s_menu_stat;}
int set_menu_stat(int menu_stat) {s_menu_stat=menu_stat;return 0;}
#undef MENU_STAT_AREA

#endif
