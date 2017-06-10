/*
 * display.c
 *
 *  Created on: 2017-2-22
 *      Author: zhouhong
 */

#include "global.h"
#include <pthread.h>
int glStartSelProductThread = 0;

pthread_t selProductThread;

static const KeyMap sgDefKeyMap[16] = {
{ XUI_KEY1, "1QZ.qz" }, { XUI_KEY2, "2ABCabc" }, { XUI_KEY3, "3DEFdef" }, { XUI_KEYDOWN, "v" },
{ XUI_KEY4, "4GHIghi" }, { XUI_KEY5, "5JKLjkl" }, { XUI_KEY6, "6MNOmno" }, { XUI_KEYALPHA, "." },
{ XUI_KEY7, "7PRSprs" }, { XUI_KEY8, "8TUVtuv" }, { XUI_KEY9, "9WXYwxy" }, { -1, "" },
{ XUI_KEYCANCEL, "" }, { XUI_KEY0, "0,*# ~`!@$%^&-+=(){}[]<>_|\\:;\"\'?/" }, { XUI_KEYCLEAR, "" }, { XUI_KEYENTER, "" },
};

//XuiImg *g_ptMpLogo=NULL;
XuiFont *g_ptXuiFont=NULL;
XuiFont *g_ptContentFont=NULL;
XuiFont *g_ptStBarFont=NULL;
XuiWindow *g_ptGifLoader=NULL;
XuiImg *imgOrdLogo = NULL;

static int sg_iClockX=0;
static int sg_iClockY=0;
static int sg_iClockWidth[3]={0};
static int sg_iClockHeight=0;
static ST_TIME sg_tOldTime={0};
static int sg_iGprsOpened=0;
static int sg_iWifiOpened=0;
static int sg_iTitleSpaceHeight=0;
static pthread_mutex_t selectProdInitMutex = PTHREAD_MUTEX_INITIALIZER;


static int GuiInit(int iSBHeight)
{
	char szValue[32]={0};
	char szRotateStr[32]={0};
	char szSBStr[32]={0};
	int iRet=0;
	char *xui_argv[10];
	int  xui_argc=0;

	iRet = OsRegGetValue("ro.fac.lcd.rotate", szValue);
	if (iRet > 0) {
		snprintf(szRotateStr, sizeof(szRotateStr), "ROTATE=%s", szValue);
	}
	else {
		strcpy(szRotateStr, "ROTATE=0");
	}

	if (iSBHeight > 0) {
		snprintf(szSBStr, sizeof(szSBStr), "STATUSBAR=%d", iSBHeight);
	}
	else {
		strcpy(szSBStr, "STATUSBAR=0");
	}

	xui_argv[0] = szRotateStr;
	xui_argv[1] = szSBStr;
	xui_argv[2] = NULL;
	xui_argc = 2;

	iRet = XuiOpen(xui_argc, xui_argv);
	if (iRet == XUI_RET_OK) {
		return OT_OK;
	}
	else {
		return OT_ERR;
	}
}

static void GuiDeinit(void)
{
	XuiClose();
}

static void UpdateStatusbar(void)
{
	static int s_icount = 0;
	char szCurESSID[64]={0};
	int iCurRSSI=0;
	int iRet=0;
	int iBatteryLevel=0;
	ST_TIME tNewTime;

	XuiColor colorClock;
	char buf[16]={0};
	char *batteryc[] = {
			"./res/battery0c.png",
			"./res/battery25c.png",
			"./res/battery50c.png",
			"./res/battery75c.png",
			"./res/battery100c.png"
	};
	char *batterys[] = {
			"./res/battery0.png",
			"./res/battery25.png",
			"./res/battery50.png",
			"./res/battery75.png",
			"./res/battery100.png",
			"./res/nobattery.png"
	};
	char *wifisignals[] = {
			"./res/wifi0.png",
			"./res/wifi50.png",
			"./res/wifi100.png"
	};
	char *netsignals[] = {
			"./res/empty.png",
			"./res/mobile1.png",
			"./res/mobile2.png",
			"./res/mobile3.png",
			"./res/mobile4.png",
			"./res/mobile5.png"
	};

	colorClock.r = 0xb8;
	colorClock.g = 0xb8;
	colorClock.b = 0xb8;
	colorClock.a = 0xff;

	OsGetTime(&tNewTime);
	if (memcmp(&sg_tOldTime, &tNewTime, sizeof(sg_tOldTime)) != 0) {
		if ( (sg_tOldTime.Hour == tNewTime.Hour) && (sg_tOldTime.Minute == tNewTime.Minute) ) {
			if ( tNewTime.Second % 2 ) {
				XuiCanvasDrawText(XuiStatusbarCanvas(), sg_iClockX + sg_iClockWidth[1], sg_iClockY, sg_iClockHeight, g_ptStBarFont, XUI_TEXT_NORMAL, colorClock, ":");
			} else {
				XuiClearArea(XuiStatusbarCanvas(), sg_iClockX + sg_iClockWidth[1], sg_iClockY, sg_iClockWidth[2], sg_iClockHeight);
			}
		} else {
			snprintf(buf, sizeof(buf), "%02d:%02d", tNewTime.Hour, tNewTime.Minute);
			XuiClearArea(XuiStatusbarCanvas(), sg_iClockX, 0, sg_iClockWidth[0], sg_iClockHeight+2);
			XuiCanvasDrawText(XuiStatusbarCanvas(), sg_iClockX, sg_iClockY, sg_iClockHeight, g_ptStBarFont,
					XUI_TEXT_NORMAL, colorClock, buf);
		}
		sg_tOldTime = tNewTime;

//		if ( gl_ucConnDevice == 'W' ) {
		if ( sg_iWifiOpened ) {
			iRet = OsWifiCheck(szCurESSID, NULL, &iCurRSSI);
			if ( !iRet ) {
				if (iCurRSSI > -33) {
					XuiSetStatusbarIcon(3, wifisignals[2]);
				} else if (iCurRSSI > -66) {
					XuiSetStatusbarIcon(3, wifisignals[1]);
				} else {
					XuiSetStatusbarIcon(3, wifisignals[0]);
				}
			} else {
				XuiSetStatusbarIcon(3, wifisignals[0]);
			}
		}
//			}
//		} else if ( gl_ucConnDevice == 'G' ) {
			if ( sg_iGprsOpened ) {
				iRet = OsWlGetSignal();
				if ( iRet >= 0 ) {
					XuiSetStatusbarIcon(0, netsignals[iRet]);
				}
			}
//		}

		//battery
		iBatteryLevel = OsCheckBattery();
		switch( iBatteryLevel ) {
		case BATTERY_LEVEL_0:
			XuiSetStatusbarIcon(7, batterys[0]);
			break;
		case BATTERY_LEVEL_1:
			XuiSetStatusbarIcon(7, batterys[1]);
			break;
		case BATTERY_LEVEL_2:
			XuiSetStatusbarIcon(7, batterys[2]);
			break;
		case BATTERY_LEVEL_3:
			XuiSetStatusbarIcon(7, batterys[3]);
			break;
		case BATTERY_LEVEL_4:
			XuiSetStatusbarIcon(7, batterys[4]);
			break;
		case BATTERY_LEVEL_CHARGE:
			XuiSetStatusbarIcon(7, batteryc[s_icount]);
			s_icount++;
			if (s_icount >= sizeof(batteryc)/sizeof(batteryc[0])) {
				s_icount = 0;
			}
			break;
		case BATTERY_LEVEL_COMPLETE:
			XuiSetStatusbarIcon(7, batteryc[4]);
			break;
		default:
			break;
		}
	}
}

static void *thread_StatusBarRefresh(void)
{
	while(1) {
		OsSleep(500);
		UpdateStatusbar();
	}
}


void *SettleProductSel(void)
{
    
	pthread_mutex_lock(&selectProdInitMutex);
	Pax_Log(LOG_INFO,"start thread to select product");
	glStartSelProductThread = 1;
	settle_page_refresh();
	settle_title_refresh();
    settle_keypad_refresh();
	pthread_mutex_unlock(&selectProdInitMutex);
	return (void*)0;
}

static char* GetKeyTableByValue(int iKeyValue)
{
	int ii;
	for(ii = 0; ii < sizeof(sgDefKeyMap)/sizeof(KeyMap); ++ii){
		if(iKeyValue == sgDefKeyMap[ii].KeyValue){
			return sgDefKeyMap[ii].Table;
		}
	}

	return NULL;
}

static void MapChar2Index(uchar ch, int *piRow, int *piCol) {
    int i;
    char *p;

    for (i = 0; i < sizeof(sgDefKeyMap)/sizeof(KeyMap); ++i)
    {
    	char *start = (char*)GetKeyTableByValue(sgDefKeyMap[i].KeyValue);
        if(!start)
        	continue;

    	for (p = start; *p; ++p) {
            if (*p == toupper(ch)) {
                *piRow = i;
                *piCol = p - start;
                break;
            }
        }
    }
}

static int MapKeyIndex(int iKey) {
    int i;
    for (i = 0; i < sizeof(sgDefKeyMap)/sizeof(KeyMap); ++i)
    {
        if (sgDefKeyMap[i].KeyValue == iKey)
            return i;
    }
    return -1;
}

int Init_Display(void)
{
	int iRet=0;
	XuiColor tColorTitleBg;
	pthread_t thread;
	//pthread_t selProductThread;
	char szCurESSID[64]={0};
	int iCurRSSI=0;
	XuiWindow *ptRootCanvas=NULL;

	GuiInit(18);
	ptRootCanvas = XuiRootCanvas();
	g_ptXuiFont = XuiCreateFont("./res/PTS75F.ttf", 0, 0);
	g_ptContentFont = XuiCreateFont("./res/PTS55F.ttf", 0, 0);
	g_ptStBarFont = XuiCreateFont("./res/ProFontWindows.ttf", 0, 0);
	tColorTitleBg.r = 0xff;
	tColorTitleBg.g = 0xff;
	tColorTitleBg.b = 0xff;
	tColorTitleBg.a = 0xff;
	XuiCanvasSetBackground(ptRootCanvas, XUI_BG_CENTER, NULL, tColorTitleBg);

	sg_iClockHeight = 14;
	sg_iClockWidth[0] = XuiTextWidth(g_ptStBarFont, sg_iClockHeight, "88:88");
	sg_iClockWidth[1] = XuiTextWidth(g_ptStBarFont, sg_iClockHeight, "88");
	sg_iClockWidth[2] = XuiTextWidth(g_ptStBarFont, sg_iClockHeight, ":");

	sg_iClockX = XUI_CENTER_X(0, XuiStatusbarCanvas()->width, sg_iClockWidth[0]);
	sg_iClockY = XUI_CENTER_Y(0, XuiStatusbarCanvas()->height, sg_iClockHeight);

	iRet = OsWlCheck();
	Pax_Log(LOG_INFO, "OsWlCheck ret = %d", iRet);
	if ( RET_OK == iRet ) {
		sg_iGprsOpened = 1;
	}
	iRet = OsWifiCheck(szCurESSID, NULL, &iCurRSSI);
	Pax_Log(LOG_INFO, "OsWifiCheck ret = %d", iRet);
	if ( RET_OK == iRet ) {
		sg_iWifiOpened = 1;
		Pax_Log(LOG_INFO, "szCurESSID = %s", szCurESSID);
	}
	UpdateStatusbar();
	pthread_create(&thread, NULL, thread_StatusBarRefresh, NULL);
	
	return OT_OK;
}

void Destroy_Display(void)
{
	if ( imgOrdLogo )
		XuiImgFree(imgOrdLogo);
	if ( g_ptXuiFont )
		XuiDestroyFont(g_ptXuiFont);
	if ( g_ptContentFont )
		XuiDestroyFont(g_ptContentFont);
	if ( g_ptStBarFont )
		XuiDestroyFont(g_ptStBarFont);
	GuiDeinit();
}

void Beep_Prompt(char cFlag)
{
	if ( cFlag == 0 ) {
		OsBeep(5, 100);
	}
	else {
		OsBeep(1, 100);
		OsSleep(1 * 100);
		OsBeep(1, 100);
		OsSleep(1 * 100);
		OsBeep(1, 100);
	}
}

static void Display_Title(const char *psztitle)
{
	char* szOrdLogoFile = "./res/ordrestyring_logo.png";
//	XuiImg *imgOrdLogo = NULL;
	int iTitleHeight=0;
	int iStrWidth=0;
	int iStrX=0, iStrY=0;
	int iGreyAreaHeight=0;
	int iIconX, iIconY, iIconWidth;
	XuiColor tColorLightGrey = {245, 245,245,255};
	XuiColor tColorBlack = {0, 0, 0, 255};

	if ( g_ptGifLoader != NULL ) {
		XuiDestroyWindow(g_ptGifLoader);
		g_ptGifLoader = NULL;
	}

	if ( XuiRootCanvas()->width == 320) {
		iGreyAreaHeight = 31;
		iIconWidth = 24;
	} else {
		iGreyAreaHeight = 41;
		iIconWidth = 32;
	}

	iTitleHeight = 16;
	XuiClearArea(XuiRootCanvas(), 0, 0, XuiRootCanvas()->width, XuiRootCanvas()->height);

	//auto font size
	while(1) {
		iStrWidth = XuiTextWidth(g_ptXuiFont, iTitleHeight, psztitle);
		if ( iStrWidth >= XuiRootCanvas()->width ) {
			iTitleHeight--;
		} else {
			break;
		}
	}

	XuiCanvasDrawRect(XuiRootCanvas(), 0, 0, XuiRootCanvas()->width, iGreyAreaHeight, tColorLightGrey, 0, 1);
	XuiCanvasDrawRect(XuiRootCanvas(), 0, iGreyAreaHeight, XuiRootCanvas()->width, 1, tColorBlack, 0, 1);

	sg_iTitleSpaceHeight = iGreyAreaHeight + 1;

	iStrWidth = XuiTextWidth(g_ptXuiFont, iTitleHeight, psztitle);
	iIconX = XUI_CENTER_X(0, XuiRootCanvas()->width, iStrWidth);
	iIconY = XUI_CENTER_Y(0, iGreyAreaHeight, iIconWidth);
	//iStrX = iIconX + iIconWidth + 5;//delete ICON
	iStrX = XUI_CENTER_X(0, XuiRootCanvas()->width, iStrWidth);
	iStrY = XUI_CENTER_Y(0, iGreyAreaHeight, iTitleHeight);
	XuiCanvasDrawText(XuiRootCanvas(), iStrX, iStrY, iTitleHeight, g_ptXuiFont, 0, tColorBlack, psztitle);
#if 0  //for drawing ICON
	//draw Ordrestyring Icon
	if(!imgOrdLogo){
		imgOrdLogo = XuiImgLoadFromFile(szOrdLogoFile);
	}
	XuiCanvasDrawImg(XuiRootCanvas(), iIconX, iIconY, iIconWidth, iIconWidth,XUI_BG_CENTER, imgOrdLogo);
#endif	
}

//display string on screen
int DrawText(INPUT_BOX* pInputBox)
{
	int iTextWidth = 0;
	int iTextX = 0;
	int iTextY = 0;
	char* pTmpTxt = NULL;
	char *pPoint = NULL;
	int iDspStrLen = 0;
	char szDisplayAmount[40] = {0};
	int iAmountTextWidth = 0;
    XuiColor tColorBlack = {0, 0, 0, 255};
	if(!pInputBox){
		return -1;
	}

	iTextWidth = XuiTextWidth(pInputBox->stText_font, pInputBox->iText_height, pInputBox->szText);
	if(iTextWidth > pInputBox->iWidth-4)
	{
		pTmpTxt = &(pInputBox->szText[pInputBox->iStrLen-1]);
		while(XuiTextWidth(pInputBox->stText_font, pInputBox->iText_height, pTmpTxt) <= pInputBox->iWidth-4)
		{
			iDspStrLen++;
			pTmpTxt--;
		}
		iDspStrLen--;
		pTmpTxt++;
	}
	else{
		pTmpTxt = pInputBox->szText;
	}

	if(INPUT_INVOICENUM == pInputBox->eType && iTextWidth < pInputBox->iWidth-4){
		iTextX = XUI_CENTER_X(0, XuiRootCanvas()->width, iTextWidth);
	}
	else{
		iTextX = pInputBox->iPosX+2;
	}
	iTextY = pInputBox->iPosY;
	
	PubConvAmount((uchar*)"",pTmpTxt,2,0,szDisplayAmount,GA_SEPARATOR);
	pPoint = strchr(szDisplayAmount, '.');
	if(pPoint)
	{
		*pPoint = ',';
	}
	iAmountTextWidth = XuiTextWidth(pInputBox->stText_font, pInputBox->iText_height, szDisplayAmount);
	iTextX = XUI_CENTER_X(0, XuiRootCanvas()->width, iAmountTextWidth);
	XuiClearArea(XuiRootCanvas(), pInputBox->iPosX+2, pInputBox->iPosY, pInputBox->iWidth, pInputBox->iHeight);
	XuiCanvasDrawText(XuiRootCanvas(), iTextX, iTextY, pInputBox->iText_height, pInputBox->stText_font,
			XUI_TEXT_NORMAL, pInputBox->stText_color,szDisplayAmount );
	XuiCanvasDrawText(XuiRootCanvas(), XuiRootCanvas()->width - 40, iTextY + 5, 16,  pInputBox->stText_font,
			XUI_TEXT_NORMAL, tColorBlack,glCurrencyName);

	return 0;
}

static int GetInput(INPUT_BOX* pstInputBox)
{
	int iKey = -1;
    int iIndex = 0, iLastKey = -1;
    int iVaildKeyCount = 0;

    //init the inputbox
    if (pstInputBox->iStrLen > 0) {
        MapChar2Index(pstInputBox->szText[pstInputBox->iStrLen - 1], &iLastKey, &iIndex);
    }
    DrawText(pstInputBox);

	while(1)
	{
		if (!XuiHasKey()){
			continue;
		}
		iKey = XuiGetKey();

		if (iKey == XUI_KEYCANCEL)
		{
			break;
		}
		else if (iKey == XUI_KEYENTER){
			break;
		}
		if (iKey == XUI_KEYCLEAR)
		{
            if (pstInputBox->iStrLen <= 0) {
                OsBeep(7, 50);
                continue;
            }
			iVaildKeyCount--;
            pstInputBox->szText[--(pstInputBox->iStrLen)] = 0;
		    if (pstInputBox->iStrLen > 0) {
		        MapChar2Index(pstInputBox->szText[pstInputBox->iStrLen - 1], &iLastKey, &iIndex);
		    }
		    else {
		        iLastKey = -1;
		        iIndex = 0;
		    }

		    DrawText(pstInputBox);
		}
		else if (iKey >= XUI_KEY1 && iKey <= XUI_KEY0)
		{
			if(pstInputBox->iStrLen >= sizeof(pstInputBox->szText)-1)
			{
				continue ;
			}
			if(iVaildKeyCount >= 8)
			{
				OsBeep(7, 50);
                continue;
			}
			iVaildKeyCount++;
			Pax_Log(LOG_INFO, "iVaildKeyCount=%d",iVaildKeyCount);
            iLastKey = MapKeyIndex(iKey);
            iIndex = 0;

            pstInputBox->szText[(pstInputBox->iStrLen)++] = GetKeyTableByValue(sgDefKeyMap[iLastKey].KeyValue)[0];
            pstInputBox->szText[pstInputBox->iStrLen] = 0;

            DrawText(pstInputBox);
		}
//		else if (iKey == XUI_KEYALPHA || iKey == XUI_KEYF2)
		else if (iKey == XUI_KEYALPHA)
		{
            if (iLastKey < 0 || pstInputBox->iStrLen < 1) {
                continue;
            }

            if (INPUT_APIKEY == pstInputBox->eType)
            {
                iIndex = (iIndex + 1) % strlen(GetKeyTableByValue(sgDefKeyMap[iLastKey].KeyValue));
                pstInputBox->szText[pstInputBox->iStrLen - 1] = GetKeyTableByValue(sgDefKeyMap[iLastKey].KeyValue)[iIndex];
                DrawText(pstInputBox);
            }
            else
            {
            	continue;
            }
		}

		Pax_Log(LOG_INFO, "key value %u", iKey);
	}

	return iKey;
}

int GetFormantAmount(double* amount)
{
	int iRetKey;
	XuiColor tColorBlack = {0, 0, 0, 255};
	XuiColor tColorGreen = {116,179,67,255};
	XuiColor tColorWhite = {255, 255, 255, 255};
	char szTopText[64] = " Amount received from customer.";
	char szConfirmText[10] = "Confirm";
//	char szDashLine[128] = {0};
	int iTextHeight = 16;
	XuiImg *imgDottedline = NULL;
	char* szEnterImgFile = "./res/ga_enter_ico.png";
	XuiWindow* pNextBnt = NULL;
	XuiButtonStat stStatNormal;
	XuiButtonStat stSatPressed;
	char szAmount[20] = {0};
	char *pszAmount;
	int iInputLen = 0;

	pszAmount = szAmount;
	INPUT_BOX stVoiceNumInputBox;

	int iTopTextWidth;
	int iTopTextX;
	int iTopTextY;

	int iDashLineX;
	int iDashLineY;

	int iConfirmAreaX;
	int iConfirmAreaY;

	int iNextBntX;
	int iNextBntY;
	int iNextBntWidth;
	int iNextBntHeight;

	//draw title
	Display_Title("TYPE IN AMOUNT");

	//show "Indtast faktura number"
	iTopTextWidth = XuiTextWidth(g_ptXuiFont, iTextHeight, szTopText);
	iTopTextX = XUI_CENTER_X(0, XuiRootCanvas()->width, iTopTextWidth);
	iTopTextY = sg_iTitleSpaceHeight + 4;
	XuiCanvasDrawText(XuiRootCanvas(), iTopTextX, iTopTextY, iTextHeight, g_ptContentFont, XUI_TEXT_NORMAL, tColorBlack, szTopText);

	//inputbox area
	memset(&stVoiceNumInputBox, 0, sizeof(INPUT_BOX));
	stVoiceNumInputBox.iPosX = 0;
	if ( XuiRootCanvas()->width == 320) {
		stVoiceNumInputBox.iPosY = iTopTextY + iTextHeight + 10;
	} else {
		stVoiceNumInputBox.iPosY = iTopTextY + iTextHeight + 100;
	}
	stVoiceNumInputBox.iWidth = XuiRootCanvas()->width;
	stVoiceNumInputBox.iHeight = 36;
	stVoiceNumInputBox.stText_font = g_ptXuiFont;
	stVoiceNumInputBox.iText_height = 30;
	stVoiceNumInputBox.stText_color = tColorGreen;
	stVoiceNumInputBox.iStrLen = 0;
	stVoiceNumInputBox.eType = INPUT_AMOUNT;

	//draw  "----------------------------"
	iDashLineX = 19;
	iDashLineY = stVoiceNumInputBox.iPosY+stVoiceNumInputBox.iHeight;
	imgDottedline = XuiImgLoadFromFile(Add_Prefix_Res_Path("ga_dottedlinebl.png"));
	XuiCanvasDrawImg(XuiRootCanvas(), iDashLineX, iDashLineY, XuiRootCanvas()->width - 38, 1,XUI_BG_CENTER, imgDottedline);

	iConfirmAreaX = XuiRootCanvas()->width - (XuiRootCanvas()->width / 3) -20;
	iConfirmAreaY = XuiRootCanvas()->height - (XuiRootCanvas()->height - iDashLineY) / 2 - iTextHeight / 2;
	iNextBntX =  iConfirmAreaX;
	iNextBntY =	iConfirmAreaY - 2;
	iNextBntHeight = iTextHeight + 10;
	iNextBntWidth = XuiRootCanvas()->width - iNextBntX;
	pNextBnt = XuiCreateButton(XuiRootCanvas(), iNextBntX, iNextBntY, iNextBntWidth, iNextBntHeight);
	memset(&stStatNormal, 0, sizeof(stStatNormal));
	stStatNormal.btn_bg = tColorWhite;
	stStatNormal.btn_round = 0;
	stStatNormal.img_style = XUI_BG_NORMAL;
	stStatNormal.text = szConfirmText;
	stStatNormal.text_x = 0;
	stStatNormal.text_y= 0;
	stStatNormal.text_height = iTextHeight;
	stStatNormal.text_font = g_ptContentFont;
	stStatNormal.text_fg = tColorBlack;
	stStatNormal.img_x = XuiTextWidth(g_ptContentFont, iTextHeight, stStatNormal.text) + 2;
	stStatNormal.img_y = 0;
	stStatNormal.img = XuiImgLoadFromFile(szEnterImgFile);
	stSatPressed = stStatNormal;

	XuiButtonSetStat(pNextBnt, XUI_BTN_NORMAL, &stStatNormal);
	XuiButtonSetStat(pNextBnt, XUI_BTN_PRESSED, &stSatPressed);
	XuiButtonSetKey(pNextBnt, XUI_KEYENTER);
	XuiShowWindow(pNextBnt, 1, 0);

	iRetKey = GetInput(&stVoiceNumInputBox);
	if(iRetKey == XUI_KEYCANCEL){
		//do cleaning
		//XuiClearArea(XuiRootCanvas(), 0, 0, XuiRootCanvas()->width, XuiRootCanvas()->height);
		//XuiShowWindow(pNextBnt  , 0, 0);
		//XuiDestroyWindow(pNextBnt);
		//XuiImgFree(imgDottedline);
		*amount = 0;
		goto clean;
	}
	if(iRetKey == XUI_KEYENTER)
	{
		Pax_Log(LOG_INFO, "szText=%s", stVoiceNumInputBox.szText);
		iInputLen = strlen(stVoiceNumInputBox.szText);
		if(iInputLen >= 3)
		{
			memcpy(pszAmount, stVoiceNumInputBox.szText, iInputLen - 2);
			pszAmount += iInputLen - 2;
			*pszAmount = '.';
			pszAmount += 1;
			memcpy(pszAmount, stVoiceNumInputBox.szText + iInputLen - 2,2);
			*amount = atof(szAmount);
		 }
		 else if(iInputLen == 0)
		 {
		 	*amount = 0;
		 }
		 else if(iInputLen == 1)
		 {
		 	memcpy(pszAmount,"0.0",3);
		 	pszAmount += 3;
		 	*pszAmount = stVoiceNumInputBox.szText[0];
		 	*amount = atof(szAmount);
		 }
		 else if(iInputLen == 2)
		 {
		 	memcpy(pszAmount,"0.",2);
		 	pszAmount += 2;
		 	memcpy(pszAmount,stVoiceNumInputBox.szText,2);
		 	*amount = atof(szAmount);
		 }
		Pax_Log(LOG_INFO, "szText=%s,pszAmount=%s,*amount=%f", stVoiceNumInputBox.szText,szAmount,*amount);
	}
	//do cleaning
clean:	
	XuiClearArea(XuiRootCanvas(), 0, 0, XuiRootCanvas()->width, XuiRootCanvas()->height);
	XuiImgFree(imgDottedline);
	XuiShowWindow(pNextBnt  , 0, 0);
	XuiDestroyWindow(pNextBnt);
	return COMFIRM_DONE;
}


int AmountToNordicNum(long int iSrcAmount, char* pszDest, int iMaxLen)
{
	char szTmpbuf[64] = {0};
	int iTmpBufLen = 0;
	int iDestLen = 0;
	char* pTmp = szTmpbuf;
	char* pTmpDest = pszDest;

	if((pszDest == NULL) || iMaxLen < 0){
		return OT_ERR_INVALID_PARAM;
	}

	snprintf(szTmpbuf, sizeof(szTmpbuf) - 1, "%ld", iSrcAmount);
	iTmpBufLen = strlen(szTmpbuf);
	if(iTmpBufLen > iMaxLen){
		return OT_ERR_INVALID_PARAM;
	}

	if(iSrcAmount < 100){
		pTmpDest[0] = '0';
		pTmpDest[1] = ',';
		pTmpDest += 2;
		if(iSrcAmount < 10)
		{
			*pTmpDest = '0';
			pTmpDest++;
		}
		memcpy(pTmpDest, pTmp, iTmpBufLen);
		pTmpDest += iTmpBufLen;
		*pTmpDest = '\0';

		return OT_OK;
	}

	if((iTmpBufLen-2)%3){
		iDestLen = (iTmpBufLen-2)%3;
		memcpy(pTmpDest, pTmp, iDestLen);
		pTmpDest[iDestLen] = '.';
		pTmp += iDestLen;
		iDestLen++;
		pTmpDest += iDestLen;
	}

	while((strlen(pTmp) -2)/3 > 1 || (strlen(pTmp) -2)/3 == 1)
	{
		memcpy(pTmpDest, pTmp, 3);
		pTmpDest[3] = '.';
		iDestLen += 4;
		pTmpDest += 4;
		pTmp += 3;
	}

	memcpy(pTmpDest, pTmp, 2);
	*strrchr(pszDest, '.') = ',';

	return OT_OK;
}

XuiWindow *gen_prompt_win_addr()
{
	static XuiWindow *s_win;
	static int first=1;
	if (first) {
		
		s_win=XuiCreateCanvas(XuiRootCanvas(),0,0,XuiRootCanvas()->width,XuiRootCanvas()->height);
		first=0;
	}
	return s_win;
}

XuiWindow *prompt_win_addr()
{
	static int first=1;
	if (first) {
		XuiColor tColorPromot = {255, 255,255,255};
		XuiCanvasSetBackground(gen_prompt_win_addr(),0,0,tColorPromot);
		first=0;
	}
	return gen_prompt_win_addr();
}

void HidePromptWin(void)
{
	if(g_ptGifLoader)
	{
		XuiShowWindow(g_ptGifLoader,XUI_HIDE, 0);
	}
	XuiShowWindow(prompt_win_addr(),XUI_HIDE, 0);
}

void Display_Prompt(const char *psztitle, const char *pszStr, MSGTYPE eMsgType, int iNeedConfirm)
{
	int iPromptHeight=0;
	int iConfirmHeight=0;
	int iStrWidth=0;
	int iStrX=0, iStrY=0;
	int iPosX=0, iPosY=0;
	int iGreyAreaHeight=0;
	int iGifSize=0;
	char *pszWaitPrompt="Please wait.";
	char szConfirmPrompt[24]={0};
	XuiColor tColorLightGrey = {245, 245,245,255};
	XuiColor tColorBlack = {0, 0, 0, 255};
	ST_TIMER tTimer;
	int iCountDown=0;
	int iSleepFlag = 0;

	if ( g_ptGifLoader != NULL ) {
		XuiDestroyWindow(g_ptGifLoader);
		g_ptGifLoader = NULL;
	}

	if ( XuiRootCanvas()->width == 320) {
		iGifSize = 110;
		iGreyAreaHeight = 31;
	} else {
		iGifSize = 150;
		iGreyAreaHeight = 41;
	}

	iPromptHeight = 16;
	iConfirmHeight = 16;
	XuiClearArea(prompt_win_addr(), 0, 0, XuiRootCanvas()->width, XuiRootCanvas()->height);
	while(1) {
		iStrWidth = XuiTextWidth(g_ptXuiFont, iPromptHeight, pszStr);
		if ( iStrWidth >= XuiRootCanvas()->width ) {
			iPromptHeight--;
		} else {
			break;
		}
	}

	if ( eMsgType == MSGTYPE_NORMAL) {
		iStrX = XUI_CENTER_X(0, XuiRootCanvas()->width, iStrWidth);
		iStrY = XUI_CENTER_Y(0, XuiRootCanvas()->height, iPromptHeight);
		XuiCanvasDrawText(prompt_win_addr(), iStrX, iStrY, iPromptHeight, g_ptXuiFont, 0, tColorBlack, pszStr);
		XuiShowWindow(prompt_win_addr(),XUI_SHOW, 0);
	} else{
		XuiCanvasDrawRect(prompt_win_addr(), 0, 0, XuiRootCanvas()->width, iGreyAreaHeight, tColorLightGrey, 0, 1);
		XuiCanvasDrawRect(prompt_win_addr(), 0, iGreyAreaHeight, XuiRootCanvas()->width, 1, tColorBlack, 0, 1);

		iStrWidth = XuiTextWidth(g_ptXuiFont, iPromptHeight, psztitle);
		iStrX = XUI_CENTER_X(0, XuiRootCanvas()->width, iStrWidth);
		iStrY = XUI_CENTER_Y(0, iGreyAreaHeight, iPromptHeight);
		XuiCanvasDrawText(prompt_win_addr(), iStrX, iStrY, iPromptHeight, g_ptXuiFont, 0, tColorBlack, psztitle);

		iPosX = XUI_CENTER_X(0, XuiRootCanvas()->width, iGifSize);
		iPosY = XUI_CENTER_Y(0, XuiRootCanvas()->height, iGifSize);

		if ( eMsgType == MSGTYPE_LOADING || eMsgType == MSGTYPE_UPLOADING) {
			g_ptGifLoader = XuiCreateGif(prompt_win_addr(), iPosX, iPosY, iGifSize, iGifSize, Add_Prefix_Res_Path("loader_loading.gif"));
		} else if ( eMsgType == MSGTYPE_SUCCESS || MSGTYPE_PRINT == eMsgType) {
			g_ptGifLoader = XuiCreateGif(prompt_win_addr(), iPosX, iPosY, iGifSize, iGifSize, Add_Prefix_Res_Path("loader_success.gif"));
			Beep_Prompt(0);
		} else if ( eMsgType == MSGTYPE_WARNING ) {
			g_ptGifLoader = XuiCreateGif(prompt_win_addr(), iPosX, iPosY, iGifSize, iGifSize, Add_Prefix_Res_Path("loader_warning.gif"));
			Beep_Prompt(1);
		} else {
			g_ptGifLoader = XuiCreateGif(prompt_win_addr(), iPosX, iPosY, iGifSize, iGifSize, Add_Prefix_Res_Path("loader_fail.gif"));
			Beep_Prompt(1);
		}

		//do not check needconfirm flag
		
		iStrWidth = XuiTextWidth(g_ptXuiFont, iConfirmHeight, pszStr);
		//auto fone size
		while(1) {
			iStrWidth = XuiTextWidth(g_ptXuiFont, iConfirmHeight, pszStr);
			if ( iStrWidth >= XuiRootCanvas()->width ) {
				iConfirmHeight--;
			} else {
				break;
			}
		}
		iStrX = XUI_CENTER_X(0, XuiRootCanvas()->width, iStrWidth);
		XuiCanvasDrawText(prompt_win_addr(), iStrX, XuiRootCanvas()->height - 2 * iConfirmHeight - 2 - 20, iConfirmHeight, g_ptXuiFont, 0, tColorBlack, pszStr);

		if ( eMsgType == MSGTYPE_LOADING || MSGTYPE_PRINT == eMsgType || eMsgType == MSGTYPE_UPLOADING)
		{		
			//do not handle at present
		} 
		else if(eMsgType == MSGTYPE_INIT)
		{
			iStrWidth = XuiTextWidth(g_ptXuiFont, iConfirmHeight,pszWaitPrompt);
			iStrX = XUI_CENTER_X(0, XuiRootCanvas()->width, iStrWidth);
			XuiCanvasDrawText(prompt_win_addr(), iStrX, XuiRootCanvas()->height - iConfirmHeight - 20, iConfirmHeight, g_ptXuiFont, 0, tColorBlack, pszWaitPrompt);
		}
		else {
			iSleepFlag = 1;
			
		}
		XuiShowWindow(prompt_win_addr(),XUI_SHOW, 0);
		if(g_ptGifLoader)
		{
			XuiShowWindow(g_ptGifLoader,XUI_SHOW, 0);
		}
		if(iSleepFlag == 1)
		{
			
			OsSleep(3 * 1000);
		}
	}
}

void Display_Error_Code(int iErrCode)
{

}
