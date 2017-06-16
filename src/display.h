/*
 * display.h
 *
 *  Created on: 2017-2-22
 *      Author: zhouhong
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "httppacket.h"
extern int glStartSelProductThread;


typedef struct _tagKeyMap {
    int KeyValue;
    char *Table;
} KeyMap;

typedef enum _msgType {
	MSGTYPE_NORMAL,
	MSGTYPE_LOADING,
	MSGTYPE_SUCCESS,
	MSGTYPE_WARNING,
	MSGTYPE_FAILURE,
	MSGTYPE_PRINT,
	MSGTYPE_UPLOADING,
	MSGTYPE_INIT,
}MSGTYPE;

typedef enum _menuIndex {
	MENUTYPE_CANCEL=0,
	MENUTYPE_PAY=1,
	MENUTYPE_SETTING=2
}MENUINDEX;

typedef enum _inputboxtype{
    INPUT_INVOICENUM,
    INPUT_MIX,
    INPUT_AMOUNT,
    INPUT_APIKEY,
}INPUTBOXTYPE;

typedef enum _isPaid{
    NOT_PAID,
    IS_PAID
}IFISPAID;

typedef enum _confirmStatus{
	COMFIRM_DONE,
	CANCEL_DONE
}CONFIRMSTATUS;

typedef struct{
	int iPosX;
	int iPosY;
	int iWidth;
	int iHeight;
    XuiFont *stText_font;
    int iText_height;
    XuiColor stText_color;
    char szText[1024];
    int iStrLen;
    INPUTBOXTYPE eType;
} INPUT_BOX;

typedef struct{
	char szItemName[128];
	char szItemValue[64];
}PAYMENT_INFO_ITEM;


//functions
int GetFormantAmount(double* amount);
int InitDisplay(void);
void DestroyDisplay(void);
void DisplayPrompt(const char *psztitle, const char *pszStr, MSGTYPE eMsgType, int iNeedConfirm);
void BeepPrompt(char cFlag);
void Display_Error_Code(int iErrCode);
void HidePromptWin(void);


#endif /* DISPLAY_H_ */
