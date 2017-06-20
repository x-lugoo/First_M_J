/*------------------------------------------------------------
* FileName: parseJason.h
* Author: Jeff
* Date: 2017-04-20
------------------------------------------------------------*/
#ifndef PARSEJASON_H
#define PARSEJASON_H


#define FILE_WRITE_OK 				0
#define FILE_ERR					-1
#define FILE_ERR_INVLIDE_PARAM		-2
#define FILE_ERR_NOT_EXIST			-3
#define FILE_ERR_INVLIDE_DATA		-4
#define FILE_ERR_OPEN_FAIL			-5
#define FILE_ERR_WRITE_FAIL			-6


#define OT_OK						0
#define OT_ERR						-1
#define OT_ERR_INVALID_PARAM		-2
#define OT_ERR_TIMEOUT				-3
#define	OT_ERR_NETWORK				-4
#define OT_ERR_FILE_NOT_EXIST		-5
#define OT_ERR_FILE_INVLID			-6
#define OT_ERR_FILE_OPER			-7
#define OT_ERR_FILE_CHECKSUM		-8
#define PARSE_OK					 0
#define PARSE_OVER_LIMIT 			-2

#define ERROR_PRODUCT_AMOUNT 		-100
#define DUMP_JASON_ERROR			-101
#define JSON_FORMAT_ERROR 			-102

#define MAX_ORDER_PRODUCT 100
extern struct _orderAllProduct glOrderAllProduct;
struct productList
{
	double price;
	int  productId;
	char varPriceFlag;
	char *productName;
};

struct _orderHead
{
	char exIdValue[64+1];
	int   terminalValue;
	char waiterIdValue[32+1];
};

struct _orderLine
{
	char productName[40];
	int product_id;
	int quantity;
	double price;
	double vat;
};

struct _orderAllProduct
{
	struct _orderLine orderLine[MAX_ORDER_PRODUCT];
	struct _orderHead orderHead;
	int orderQuantity;
	double totalPrice;
	double totalVat;
};

struct currentJsonData
{
	char jsonData[4096];
};

extern void InitOrderHead(struct _orderHead *orderHead);
extern int PackOrderApiJson(char *pszOutJsonData);
extern int HandleReturnOrderJson(char *pszInJsonData);

#endif
