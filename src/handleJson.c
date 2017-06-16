/*------------------------------------------------------------
* FileName: parseJason.c
* Author: Jeff
* Date: 2017-04-20
------------------------------------------------------------*/


#include "global.h"

struct _orderAllProduct glOrderAllProduct;

void InitOrderHead(struct _orderHead *orderHead)
{
	memset(orderHead,0,sizeof(struct _orderHead));
	strcpy(orderHead->exIdValue,glMainAppData.orderID);
	orderHead->terminalValue = atoi(glMainAppData.terminalID);
	strcpy(orderHead->waiterIdValue,glMainAppData.waiterID);
}


int PackOrderApiJson(char *pszOutJsonData)

{
	json_t *pObjectmsg=NULL;
	json_t *pObjectOrderLine = NULL;
	char *pResult = NULL;
	json_t *pArray = json_array();
	int i;
	int iRet;

    PaxLog(LOG_INFO,"start to pack order api Jason,MAX_ORDER_PRODUCT =%d",MAX_ORDER_PRODUCT);
	iRet = ReadFile(ORDER_AllPRODUCT_FILE,&glOrderAllProduct,sizeof(struct _orderAllProduct));
	if(iRet)
	{
		return iRet;
	}
	if(glOrderAllProduct.orderQuantity <= 0)
	{
		return ERROR_PRODUCT_AMOUNT;
	}
	InitOrderHead(&glOrderAllProduct.orderHead);
	pObjectmsg = json_object();
	json_object_set_new(pObjectmsg,"external_id",json_string(glOrderAllProduct.orderHead.exIdValue));
	json_object_set_new(pObjectmsg,"terminal_id",json_integer(glOrderAllProduct.orderHead.terminalValue));
	json_object_set_new(pObjectmsg,"waiter_id",json_string(glOrderAllProduct.orderHead.waiterIdValue));

	for(i = 0; i < glOrderAllProduct.orderQuantity;i++)
	{
		pObjectOrderLine = json_object();
		json_object_set_new(pObjectOrderLine,"quantity",json_integer(glOrderAllProduct.orderLine[i].quantity));
		json_object_set_new(pObjectOrderLine,"price",json_integer((int)glOrderAllProduct.orderLine[i].price));
		//if(glOrderAllProduct.orderLine[i].product_id == 20)
		//{
			//json_object_set_new(pObjectOrderLine,"vat",json_integer(0));
		//}
		//else
		//{
			json_object_set_new(pObjectOrderLine,"vat",json_real(glOrderAllProduct.orderLine[i].vat));
		//}
		json_object_set_new(pObjectOrderLine,"product_id",json_integer(glOrderAllProduct.orderLine[i].product_id));
		json_array_insert(pArray,i,pObjectOrderLine);
	}
	json_object_set_new(pObjectmsg,"order_lines",pArray);
	pResult = json_dumps(pObjectmsg,JSON_PRESERVE_ORDER);
	if(!pResult)
	{
		return DUMP_JASON_ERROR;
	}
	strcpy(pszOutJsonData,pResult);
	free(pResult);
	json_decref(pObjectmsg);
	return 0;
}

int HandleReturnOrderJson(char *pszInJsonData)
{
	json_t *jsRoot = NULL;
	json_t *value;
	json_error_t error;
	const char *key;

	PaxLog(LOG_INFO,"start to print Ret json log");
	PaxLog(LOG_INFO,"pszInJsonData=%s					fun:%s,Line:%d",
			pszInJsonData,__FUNCTION__,__LINE__);
	jsRoot = json_loads(pszInJsonData,0,&error);
	if(jsRoot == NULL)
	{
		json_decref(jsRoot);
		return JSON_FORMAT_ERROR;
	}
	if(JSON_OBJECT  == json_typeof(jsRoot))
	{
		json_object_foreach(jsRoot, key, value)
		{
			PaxLog(LOG_INFO,"key=%s",key);
			if(JSON_TRUE == json_typeof(value))
			{
				json_decref(jsRoot);
				PaxLog(LOG_INFO,"value = true");
				return UpdateTerminalProduct();
			}
			else if (JSON_FALSE == json_typeof(value))
			{
				json_decref(jsRoot);
				PaxLog(LOG_INFO,"value = false");
			}
			else
			{
				json_decref(jsRoot);
			    PaxLog(LOG_INFO,"error formant");
			    return JSON_FORMAT_ERROR;
			}
		}
	}
	else
	{
		json_decref(jsRoot);
		PaxLog(LOG_INFO,"error formant");
		return JSON_FORMAT_ERROR;
	}
	return 0;
}

