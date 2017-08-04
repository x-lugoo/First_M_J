/*------------------------------------------------------------
* FileName: main.c
* Author: zhouhong
* Date: 2017-02-22
------------------------------------------------------------*/

#include "global.h"
#include "ui_menu.h"


int main(void)
{
	int iRet;
	int iKey;
	int i;
	char szTerminalType[20];
	char jsonBuff[MAX_JASON_DATA];

	Init_Display();
	Pax_LogInit("log");
	
	glMiniPosData.wakeUpReason = MINI_WAKEUP_SHOWMENU; //init Miniposdata
    SaveMiniDataForMain(&glMiniPosData);
	memset(szTerminalType,0,sizeof(szTerminalType));
	memset(jsonBuff,0,sizeof(jsonBuff));
	OsRegGetValue("ro.fac.mach", szTerminalType);	//check device type
	if(0 == strcmp(szTerminalType, "d200"))
	{
		Set_Prefix_Res_Path("./res/320/");
	}
	else if(0 == strcmp(szTerminalType, "s920") || (0 == strcmp(szTerminalType, "s900")) )
	{
		Set_Prefix_Res_Path("./res/240/");
	}
	iRet = LoadMainDataForMini(&glMainAppData);//read data for MAINAPP sharing file.
	if(iRet == 0)
	{
		 switch(glMainAppData.wakeUpReason)
		 {
		 	case MAIN_WAKEUP_STARTUP:
		 	case MAIN_WAKEUP_EXIT_FROM_MENU:
		 		Pax_Log(LOG_INFO, "%s - %d MiniPos_STARTUP", __FUNCTION__, __LINE__);
		 		if(glMainAppData.wakeUpReason == MAIN_WAKEUP_STARTUP)
		 		{
		 			iRet = LoadTerminalProduct(); //If success,save json file in local file.
					Pax_Log(LOG_INFO,"load terminal product,iRet = %d		fun:%s,line:%d",iRet,__FUNCTION__,__LINE__);
					if(iRet)
					{
						Display_Prompt("ERROR", "Update product Fail", MSGTYPE_FAILURE, 0);
		 				HidePromptWin();
		 				Destroy_Display();
						return iRet;
					}
		 		}
		 		iRet = ProcessStartUp();
		 		if(iRet == START_SHOW_MAINAPP_MENU)
		 		{
		 			glMiniPosData.wakeUpReason = MINI_WAKEUP_SHOWMENU; //init Miniposdata
    				SaveMiniDataForMain(&glMiniPosData);
		 		}
		 		else if(iRet)
		 		{
					Display_Prompt("ERROR", "startup error", MSGTYPE_SUCCESS, 0);
					return ABNORMAL_EXIT;
				}
				break;
		 	case MAIN_WAKEUP_PAYRESULT:
		 		Pax_Log(LOG_INFO,"start to process payment result 	fun:%s,line:%d",__FUNCTION__,__LINE__);
		 		iRet = ProcessPaymentResult(glMainAppData.paymentResult);
		 		if(iRet == START_SHOW_MAINAPP_MENU)
		 		{
		 			glMiniPosData.wakeUpReason = MINI_WAKEUP_SHOWMENU; //init Miniposdata
    				SaveMiniDataForMain(&glMiniPosData);
		 		}
		 		else if(iRet)
		 		{
		 			return iRet;
		 		}
		 		break;
		 	default:
		 		Display_Prompt("ERROR", "Unknown Command.", MSGTYPE_FAILURE, 0);
		 		HidePromptWin();
		 		Destroy_Display();
		 		return ABNORMAL_EXIT;
		  }
	}
	else
	{
		Display_Prompt("ERROR", "File no exist.", MSGTYPE_FAILURE, 0);
		HidePromptWin();
		Pax_Log(LOG_DEBUG,"iRet=%d,fun:%s,Line:%d",iRet,__FUNCTION__,__LINE__);
		return ABNORMAL_EXIT;
	}
	Destroy_Display();
	return NORMAL_EXIT;
}
