/*------------------------------------------------------------
* FileName: print.c
* Author: lugoo
* Date: 2017-05-02
------------------------------------------------------------*/
#include "global.h"

//#define PRN_FONT_PATH "simsun.ttc"
#define PRN_FONT_PATH "profontwindows.ttf"




/*
*
*  convert 43.200000 -> "4320"
***********************************************
*/
void FormatFloat(double price,uchar *pszOutput)
{	
	int iLen;
	char *p;
	uchar ucTempAmount[30];

	memset(ucTempAmount,0,sizeof(ucTempAmount));
	sprintf(ucTempAmount,"%f",price);
	p = NULL;
	iLen = strlen(ucTempAmount);
	p = strchr(ucTempAmount,'.');
	if(p)
	{
		*p = *(p +1);          //remove'.'
		*(p+1) = *(p+2);
		if( (iLen + (int)ucTempAmount - (int)p) > 2)
		{
			*(p + 2) = '\0';
		}
	}
	strcpy(pszOutput,ucTempAmount);
}

int PrintReceipt(struct _orderLine *orderLine,int orderLineNum)
{
	int iRet;
	int i;
	int iLen;
	int iKey;
	char *p;
	char *pPoint;
    uchar ucDispAmount[30];
    uchar ucTempAmount[30];
    uchar ucTotalAmount[30];
    char  printBuff[30];
    char  printAmtBuff[30];
    char  printShowAmtBuff[30];
    char  printVatBuff[30];
    char  printShowVatBuff[30];
    ST_TIME curTime;

    memset(&curTime,0,sizeof(ST_TIME));
	memset(ucTotalAmount,0,sizeof(ucTotalAmount));
	memset(printVatBuff,0,sizeof(printVatBuff));
	memset(printShowVatBuff,0,sizeof(printShowVatBuff));
	//format total Amount
	FormatFloat(glOrderAllProduct.totalPrice,ucTempAmount);
	Pax_Log(LOG_INFO,"ucTempAmount=%s,fun:%s,line:%d",ucTempAmount,__FUNCTION__,__LINE__);
	PubConvAmount((uchar*)"",ucTempAmount,2,0,ucTotalAmount,GA_SEPARATOR);
	pPoint = NULL;
	pPoint = strchr(ucTotalAmount, '.');
	if(pPoint)
	{
		*pPoint = ',';
	}

     //format vat
	FormatFloat(glOrderAllProduct.totalVat,printVatBuff);
	Pax_Log(LOG_INFO,"printVatBuff=%s,fun:%s,line:%d",printVatBuff,__FUNCTION__,__LINE__);
	PubConvAmount((uchar*)"",printVatBuff,2,0,printShowVatBuff,GA_SEPARATOR);
	pPoint = NULL;
	pPoint = strchr(printShowVatBuff, '.');
	if(pPoint)
	{
		*pPoint = ',';
	}
	
	Pax_Log(LOG_INFO,"start to print receipt");
	OsGetTime(&curTime);
	iRet = OsPrnOpen(PRN_REAL,NULL);
	if(iRet)
	{
		//Init_Display();
		Display_Prompt("ERROR", "PRINT FAIL", MSGTYPE_WARNING, 0);
		HidePromptWin();
		Pax_Log(LOG_INFO,"prnOpen,iRet=%d",iRet);
		return iRet;
	}
	OsPrnReset();
	iRet = OsPrnSetFont(PRN_FONT_PATH);
	Pax_Log(LOG_INFO,"OsPrnSetFont,iRet=%d",iRet);
    
    OsPrnSetSpace(1, 0);
    OsPrnSelectFontSize(15, 48, 15, 48);
    OsPrnPrintf("%s","     SALE RECEIPT\n");
    OsPrnSelectFontSize(15, 34, 15, 34);
    OsPrnPrintf("ORDER ID:%s\n",glMainAppData.orderID);
    OsPrnPrintf("TM ID:%s\n",glMainAppData.terminalID);
	OsPrnPrintf("DATE:%02d-%02d-%02d TIME:%02d:%02d\n",
		curTime.Year-2000,curTime.Month,curTime.Day,curTime.Hour,curTime.Minute);
	OsPrnPrintf("CURRENCY:%s\n",glCurrencyName);
	OsPrnPrintf("%s","----------------------\n");
	for(i = 0;i < orderLineNum;i++)
	{
		memset(ucDispAmount,0,sizeof(ucDispAmount));
		memset(printAmtBuff,0,sizeof(printAmtBuff));
		memset(printShowAmtBuff,0,sizeof(printShowAmtBuff));
		
		FormatFloat(orderLine[i].price,ucTempAmount);
		PubConvAmount((uchar*)glCurrencyName,ucTempAmount,2,0,ucDispAmount,GA_SEPARATOR);
		snprintf(printBuff,17,"%dX%s",orderLine[i].quantity,orderLine[i].productName);
		OsPrnPrintf("%s",printBuff);
		FormatFloat(orderLine[i].price * orderLine[i].quantity,printAmtBuff);
		PubConvAmount((uchar*)"",printAmtBuff,2,0,printShowAmtBuff,GA_SEPARATOR);
		pPoint = NULL;
		pPoint = strchr(printShowAmtBuff, '.');
		if(pPoint)
		{
			*pPoint = ',';
		}
		snprintf(printBuff,24,"%*.*s\n",24-strlen(printBuff),24-strlen(printBuff),printShowAmtBuff);
	    OsPrnPrintf("%s",printBuff);
	}
	OsPrnPrintf("%s","----------------------\n");
	// print total amount
	OsPrnPrintf("TOTAL:");
	snprintf(printBuff,24,"%*.*s\n",24-strlen("TOTAL:"),24-strlen("TOTAL:"),ucTotalAmount);
	OsPrnPrintf("%s",printBuff);


	//print payment type
	switch(glMainAppData.paymentType)
	{
		case M2M_PAYMENT_TYPE_CARD:
			strcpy(printBuff,"CREDIT CARD:");
			break;
		case M2M_PAYMENT_TYPE_MOBILEPAY:
			strcpy(printBuff,"MOBILEPAY:");
			break;
		case M2M_PAYMENT_TYPE_CASH:
			strcpy(printBuff,"CASH:");
			break;
		case M2M_PAYMENT_TYPE_VOUCHER:
			strcpy(printBuff,"VOUCHER CARD:");
			default:
			strcpy(printBuff,"NO PAYMENT TYPE");
			break;
	}
	if(strcmp(printBuff,"NO PAYMENT TYPE"))
	{
		OsPrnPrintf(printBuff);
		snprintf(printBuff,24,"%*.*s\n",24-strlen(printBuff),24-strlen(printBuff),ucTotalAmount);
		OsPrnPrintf("%s",printBuff);
	}
	
	//print total vat
	OsPrnPrintf("HERAF MOMS:");
    snprintf(printBuff,24,"%*.*s\n",24-strlen("HERAF MOMS:"),24-strlen("HERAF MOMS:"),printShowVatBuff);
	OsPrnPrintf("%s%s",printBuff,"\n\n\n\n\n");
	Display_Prompt("PLEASE WAIT...", "Processing request", MSGTYPE_PRINT, 0);
	iRet = OsPrnStart();
	HidePromptWin();
	if(iRet)
	{
		iKey = -1;
		if(iRet == ERR_PRN_PAPEROUT)
		{
			do{
				//Init_Display();
				Display_Prompt("NO PAPER", "PLEASE PUT PAPER", MSGTYPE_WARNING, 0);
				if(XuiHasKey())
				{
					iKey = XuiGetKey();
					if(iKey == XUI_KEYCANCEL)
					{
						HidePromptWin();
						break;
					}
					else if(iKey == XUI_KEYENTER)
					{
						iRet = OsPrnStart();
						HidePromptWin();
					}
				}
			}while(iRet == ERR_PRN_PAPEROUT);
         }
		else
		{
			//Init_Display();
			Display_Prompt("ERROR", "PRINT FAIL", MSGTYPE_FAILURE, 0);
			HidePromptWin();
		}
		Pax_Log(LOG_INFO,"OsPrnStart,iKey = %d,iRet=%d",iKey,iRet);
	}

	OsPrnClose();
	return 0;
}


