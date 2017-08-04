/*------------------------------------------------------------
* FileName: dataExchange.h
* Author: Jeff
* Date: 2017-04-25
------------------------------------------------------------*/
#ifndef DATAEXCHANGE_H
#define DATAEXCHANGE_H
#include "Sm_all.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define FILE_MAINAPP_MINIPOS	"/tmp/MAINAPP-MiniPOS.req"
#define FILE_MINIPOS_MAINAPP	"/tmp/miniPOS-MAINAPP.req"
#define ORDER_AllPRODUCT_FILE 	"./data/allProduct.dat"
#define OFFSET_NUM_FILE				"./data/offsetNum.dat"
#define OFFSET_TRAN_FILE			"./data/offsetTranFile.dat"
#define CURRENT_UPLOAD_JSON_FILE   "./data/currentuploadjsonfile.dat"

extern struct miniposToMainapp glMiniPosData;
extern struct mainappToMinipos glMainAppData;
extern int glTotalCount;
extern double glTotalPrice;
extern double glTotalVat;
extern char glReverFlag;
extern char *glCurrencyName;
extern uchar glOfflineNum;
extern int glStartOfflineUploadMode;
extern int glCurrentOfflineIdx;

extern int GetDataFromNodeStruc(void);
extern int SaveFile(const char *pszFileName,const void *psData,int iDataLen);
extern int ReadFile(const char *pszFileName,const void *psData,int iDataLen);

extern int UpdateTerminalProduct(void);
extern int LoadTerminalProduct(void);
extern int ReadJason(unsigned char *pszStr);
extern int ProcesssStartUp(void);
extern int SaveMainAppDataForTest(void);
extern int menu_exec(MENU_SELECTION menuStat);
extern int SaveOffLineFile(const char *pszFileName,long lOffset,const void *psData,int iDataLen);
extern int LoadCurrenUploadJsonData(char *pszJsonData);
extern int ReadOffLineFile(const char *pszFileName,long lOffset,const void *psData,int iDataLen);



enum
{
	SAVE_JSON = 0,
	UPDATE_JSON,
	NORMAL,
};

enum miniPos2MAINAPPerrors {

	M2M_SUCCESS = 0,
	M2M_INVALID_PARAMS,
	M2M_FILE_NOT_FOUND,
	M2M_FILE_ACCESS_DENIED,
	M2M_READ_ERROR,
	M2M_INVALID_CRC,
	M2M_WRITE_ERROR
};

/******************************************************
wakeup reason 
 	'0' Startup
 	'1' Payment result
 	'2' exit from menu
Payment result
	'0' Payment denied
	'1' Payment approved
	'2' cancelled
*********************************************************
*/

enum
{
	MAIN_WAKEUP_STARTUP 	= '0',
	MAIN_WAKEUP_PAYRESULT   = '2',
	MAIN_WAKEUP_EXIT_FROM_MENU  = '4',
};

typedef enum
{
	MAIN_PAYRESULT_DENIED  		= '0',
	MAIN_PAYRESULT_APPROVED 	= '1',
	MAIN_PAYRESULT_CANCELLED    = '2',
}PAYMENT_RESULT;

typedef enum
{
	M2M_PAYMENT_TYPE_CARD       =  '0',
 	M2M_PAYMENT_TYPE_MOBILEPAY  =  '1',
	M2M_PAYMENT_TYPE_CASH       =  '2',
	M2M_PAYMENT_TYPE_VOUCHER    =  '3',
}PAYMENT_TYPE;


enum
{
	MINI_WAKEUP_STARTPAY = '1',
	MINI_WAKEUP_SHOWMENU = '3',
	MINI_WAKEUP_DONOTHING = 0,
};



struct mainappToMinipos
{
	char wakeUpReason;
	char paymentType;
	char paymentResult;
	char currency[3+1];  	// currency code, NULL terminated e.g. "DKK"
	char precision;      	// number of decimal digits e.g. 2
	char terminalID[8+1];	// mandatory terminal ID
	char orderID[64+1];		// mandatory order ID
	char waiterID[32+1];	// optional waiter ID
	char ticketLine1[24+1]; // optional ticket line 1
 	char ticketLine2[24+1]; // optional ticket line 2
 	char ticketPrintEnabled;// ticket print flag '0' do not print, '1' print
};

/****************************************************
wakeup reason
	'0' start payment
	'1' show menu
Payment type
	'0' card Payment
	'1' mobile pay
	'2' cash Payment
Amount "000000123400"


*******************************************************
*/
struct miniposToMainapp
{
	char wakeUpReason;
	char amount[12];
	
};

#ifdef __cplusplus
};
#endif


#endif
