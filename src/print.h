/*------------------------------------------------------------
* FileName: print.h
* Author: lugoo
* Date: 2017-05-02
------------------------------------------------------------*/
#ifndef PRINT_H
#define PRINT_H


void FormatFloat(double price,uchar *pszOutput);

int PrintReceipt(struct _orderLine *orderLine,int orderLineNum,char ifPrintFlag);
#endif
