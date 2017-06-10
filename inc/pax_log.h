#ifndef PAX_LOG_H
#define PAX_LOG_H

#include <string.h>
#include <unistd.h>
#include <xui.h>
#include <osal.h>

//muti-platforom configuration
#define MP_GETKEY() XuiGetKey()
#define MP_PRINTF(fmt,args...) do {\
	OsLogSetTag("chad");\
	OsLog(LOG_ERROR,fmt,##args);\
} while (0)
#define MP_ENTER_KEYVAL (XUI_KEYENTER)
#define MP_CANEL_KEYVAL (XUI_KEYCANCEL)
//-muti-platforom configuration


//selection
#define FORMAT_SHOW_FILE_ON 0
#define SAY_SEL \
	"aaaaaaaaaa"\
	"xm_default_para_init"\
//-selection

//debug printer
#if FORMAT_SHOW_FILE_ON
#define MP_FORMAT_PRINTF(fmt,args...) do {\
	OsLogSetTag("log");\
	char mp_fmt[128]={0};\
	sprintf(mp_fmt,"%s%s%s","[%s][%s][%d]:",fmt,"\n");\
	OsLog(LOG_ERROR,mp_fmt,__FILE__,__FUNCTION__,__LINE__,##args);\
} while (0)
#else
#define MP_FORMAT_PRINTF(fmt,args...) do {\
	OsLogSetTag("log");\
	char mp_fmt[128]={0};\
	sprintf(mp_fmt,"%s%s%s","[%s][%d]:",fmt,"\n");\
	OsLog(LOG_ERROR,mp_fmt,__FUNCTION__,__LINE__,##args);\
} while (0)
#endif//#if FORMAT_SHOW_FILE_ON

#define SAY(fmt,args...) do {\
	if (!strstr(SAY_SEL,__func__)) break;\
	MP_FORMAT_PRINTF(fmt,##args);\
} while (0)
#define ERR(fmt,args...) do {\
	MP_FORMAT_PRINTF(fmt,##args);\
	OsSleep(10);\
} while (0)

//-debug printer
extern ST_TIMER _MACRO_TIMER_;
#define TIMER_START \
	OsTimerSet(&_MACRO_TIMER_, 60*1000);\
	ERR("[TIMER_START]=%d",OsTimerCheck(&_MACRO_TIMER_));

#define TIMER_START_DEF \
	ST_TIMER _MACRO_TIMER_;\
	OsTimerSet(&_MACRO_TIMER_, 60*1000);\
	ERR("[TIMER_START]=%d",OsTimerCheck(&_MACRO_TIMER_));


#define TIMER_CHECK \
	ERR("[TIMER_CHECK]=%d",OsTimerCheck(&_MACRO_TIMER_));

#define VBLK(key_value) do {\
	int mp_keyvalue=0;\
	ERR("wait for key[%d]",key_value);\
	while ((mp_keyvalue=MP_GETKEY())!=key_value) {ERR("getkey[%d]!=%d",mp_keyvalue,key_value);}\
	ERR("get target_key, exit");\
} while (0)
#define EBLK() VBLK(MP_ENTER_KEYVAL);
#define CBLK() VBLK(MP_CANEL_KEYVAL);
//-block

#endif//PAX_LOG_H

