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

	InitDisplay();
	PaxLogInit("log"); //init log tag
	glMiniPosData.wakeUpReason = MINI_WAKEUP_SHOWMENU; //init Miniposdata
    SaveMiniDataForMain(&glMiniPosData);
	memset(szTerminalType,0,sizeof(szTerminalType));
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
		 		else if(iRet && glUploadFailToMainScreenFlag == 0)
		 		{
		 			DestroyDisplay();
		 			return iRet;
		 		}
		 		else if(iRet && glUploadFailToMainScreenFlag == 1)//if upload fail,will back to main screen and save it.
		 		{
		 			glUploadFailToMainScreenFlag = 0;
		 		      //for implement upload offline product
		 			if(access(OFFSET_NUM_FILE,F_OK) < 0)
					{
						glOfflineNum = 0;
					}
					else
					{
						iRet = ReadFile(OFFSET_NUM_FILE,&glOfflineNum,1);
						if(iRet)
						{
							DestroyDisplay();
							return iRet;
						}
					}
		 			iRet = SaveOffLineFile(OFFSET_TRAN_FILE,sizeof(struct currentJsonData) * glOfflineNum,
		 			 &glCurUploadJsonData,sizeof(struct currentJsonData));
		 			glOfflineNum += 1;
		 			remove(OFFSET_NUM_FILE);
		 			PaxLog(LOG_DEBUG,"glOfflineNum=%d,fun:%s_line:%d",
											glOfflineNum,__FUNCTION__,__LINE__);
		 			iRet = SaveFile(OFFSET_NUM_FILE,&glOfflineNum,1);
	 				if(iRet)
	 				{
	 					DestroyDisplay();
	 					return iRet;
	 				}
		 			iRet = PrintReceipt(glOrderAllProduct.orderLine,glOrderAllProduct.orderQuantity,glMainAppData.ticketPrintEnabled);
					if(iRet)
					{	
						PaxLog(LOG_DEBUG,"PrintReceipt,iRet=%d,fun:%s_line:%d",
											iRet,__FUNCTION__,__LINE__);
						return iRet;
					}
		 			iRet = ProcessStartUp(); //back to main screen/selecting products
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
		 		iRet = ProcessStartUp();//enter page of selecting products
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
