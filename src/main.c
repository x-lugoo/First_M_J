/*------------------------------------------------------------
* FileName: main.c
* Author: JEFF
* Date: 2017-05-22
------------------------------------------------------------*/

#include "global.h"
#include "ui_menu.h"


int main(void)
{
	int iRet;
	char szTerminalType[20];
	char jsonBuff[MAX_JASON_DATA];

	InitDisplay();
	PaxLogInit("log");
	glMiniPosData.wakeUpReason = MINI_WAKEUP_SHOWMENU; //init Miniposdata
    SaveMiniDataForMain(&glMiniPosData);
	memset(szTerminalType,0,sizeof(szTerminalType));
	memset(jsonBuff,0,sizeof(jsonBuff));
	OsRegGetValue("ro.fac.mach", szTerminalType);	//check device type
	if(0 == strcmp(szTerminalType, "d200"))
	{
		SetPrefixResPath("./res/320/");
	}
	else if(0 == strcmp(szTerminalType, "s920") || (0 == strcmp(szTerminalType, "s900")) )
	{
		SetPrefixResPath("./res/240/");
	}
	iRet = LoadMainDataForMini(&glMainAppData);//read data for MAINAPP sharing file.
	if(iRet == 0)
	{
		 PaxLog(LOG_INFO, "%s - %d wakeupReason=%d", __FUNCTION__, __LINE__,glMainAppData.wakeUpReason);
		 switch(glMainAppData.wakeUpReason)
		 {
		   case MAIN_WAKEUP_PAYRESULT:
		 		PaxLog(LOG_INFO,"start to process payment result 	fun:%s,line:%d",__FUNCTION__,__LINE__);
		 		iRet = ProcessPaymentResult(glMainAppData.paymentResult);
		 		if(iRet == START_SHOW_MAINAPP_MENU)
		 		{
		 			PaxLog(LOG_INFO,"---Checked the function key---fun:%s,line:%d",__FUNCTION__,__LINE__);
		 			memset(&glMiniPosData, 0, sizeof(struct miniposToMainapp));
		 			glMiniPosData.wakeUpReason = MINI_WAKEUP_SHOWMENU;
    				SaveMiniDataForMain(&glMiniPosData);
    				PaxLog(LOG_INFO,"---Have sent Messages to Mainapp,prepare to exit(),---fun:%s,line:%d",__FUNCTION__,__LINE__);
		 		}
		 		else if(iRet)
		 		{
		 			DestroyDisplay();
		 			return iRet;
		 		}
		 		break;
			default:
		 		if(glMainAppData.wakeUpReason == MAIN_WAKEUP_STARTUP)
		 		{
		 			iRet = LoadTerminalProduct(); //If success,save json file in local file.
					PaxLog(LOG_INFO,"load terminal product,iRet = %d		fun:%s,line:%d",iRet,__FUNCTION__,__LINE__);
					if(iRet)
					{
						DisplayPrompt("ERROR", "Update product Fail", MSGTYPE_FAILURE, 0);
		 				HidePromptWin();
		 				DestroyDisplay();
						return iRet;
					}
		 		}
		 		iRet = ProcessStartUp();
		 		if(iRet == START_SHOW_MAINAPP_MENU) //press func key
		 		{
		 			PaxLog(LOG_INFO,"---Checked the function key---fun:%s,line:%d",__FUNCTION__,__LINE__);
		 			memset(&glMiniPosData, 0, sizeof(struct miniposToMainapp));
		 			glMiniPosData.wakeUpReason = MINI_WAKEUP_SHOWMENU;
    				SaveMiniDataForMain(&glMiniPosData);
    				PaxLog(LOG_INFO,"---Have sent Messages to Mainapp,prepare to exit(),---fun:%s,line:%d",__FUNCTION__,__LINE__);
		 		}
		 		else if(iRet)
		 		{
					DisplayPrompt("ERROR", "startup error", MSGTYPE_SUCCESS, 0);
					HidePromptWin();
		 			DestroyDisplay();
					return ABNORMAL_EXIT;
				}
				break;
		    }
	}
	else
	{
		DisplayPrompt("ERROR", "File no exist.", MSGTYPE_FAILURE, 0);
		HidePromptWin();
		PaxLog(LOG_DEBUG,"iRet=%d,fun:%s,Line:%d",iRet,__FUNCTION__,__LINE__);
		return ABNORMAL_EXIT;
	}
	DestroyDisplay();
	return NORMAL_EXIT;
}
