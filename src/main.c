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
	
#ifdef JEFF_DEBUG
    //SaveMainAppDataForTest();  //for test to write MAINAPP sharing data
#endif
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
		while(1) //fetch and check json data from file
		{
			if (XuiHasKey() && XuiGetKey() == XUI_KEYCANCEL)
			{
				return USER_CANCEL;
			}
			iRet = ReadJason(jsonBuff); //The main function is to check whether existed json file in local file.
			if(iRet)
			{
				Pax_Log(LOG_INFO,"ReadJason,iRet = %d		fun:%s,line:%d",iRet,__FUNCTION__,__LINE__);
				iRet = LoadTerminalProduct(); //If success,save json file in local file.
				Pax_Log(LOG_INFO,"load terminal product,iRet = %d		fun:%s,line:%d",iRet,__FUNCTION__,__LINE__);
				if(iRet)
				{
					return iRet;
				}
			}
			else
			{
				break;
			}
		}
		 switch(glMainAppData.wakeUpReason)
		 {
		 	case MAIN_WAKEUP_STARTUP:
		 		Pax_Log(LOG_INFO, "%s - %d MiniPos_STARTUP", __FUNCTION__, __LINE__);
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
#ifdef JEFF_DEBUG
				break;
#else
				break; 
#endif
		 	case MAIN_WAKEUP_PAYRESULT:
		 		//glMainAppData.paymentResult = MAIN_PAYRESULT_CANCELLED;//for test only
		 		Pax_Log(LOG_INFO,"start to process payment result 	fun:%s,line:%d",__FUNCTION__,__LINE__);
		 		iRet = ProcessPaymentResult(glMainAppData.paymentResult);
		 		if(iRet)
		 		{
		 			return iRet;
		 		}
		 		break;
		 	case MAIN_WAKEUP_EXIT_FROM_MENU:
		 		Display_Prompt("EXIT", "App Exit", MSGTYPE_SUCCESS, 0);
		 		HidePromptWin();
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
#ifdef JEFF_DEBUG
	//Display_Prompt("SUCCESS", "Test success", MSGTYPE_SUCCESS, 0);
#endif
	Destroy_Display();
	return NORMAL_EXIT;
}
