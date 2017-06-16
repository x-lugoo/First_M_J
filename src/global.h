/*------------------------------------------------------------
* FileName: global.h
* Author: Jeff
* Date: 2017-04-25
------------------------------------------------------------*/
#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <osal.h>
#include "xui.h"
#include <signal.h>
#include <openssl/sha.h>
#include "curl.h"
#include "jansson.h"
#include "handleJson.h"
#include <qrencode.h>
#include "log.h"
#include "base.h"
#include "simple_parse.h"
#include "httppacket.h"
#include "display.h"
#include "dataExchange.h"
#include "sm_all.h"

#define MINIPOS_VERSION	"V0.01"


#define APIKEY_SIZE     128
#define JEFF_DEBUG   //for debug
#define DEBUG_LOG_OPEN   //control log messages output
#define USER_CANCEL 	-2
#define ABNORMAL_EXIT 	-1
#define NORMAL_EXIT 	0
#define START_SHOW_MAINAPP_MENU (9001) //when in book_page,pressing function key,minipos should exit

#define MAX_JASON_DATA 4096

#endif
