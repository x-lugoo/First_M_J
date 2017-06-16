/*------------------------------------------------------------
* FileName: HttpPacket.h
* Author: Jeff
* Date: 2017-04-25
------------------------------------------------------------*/
#ifndef HTTPPACKET_H
#define HTTPPACKET_H

#define HTTPPACKET_OK					0
#define	HTTPPACKET_ERR					-1
#define HTTPPACKET_INVALIDPARAM			-2


//define http code
#define HTTP_OK			200
#define HTTP_ERR		400
#define HTTP_AUTH_ERR	401

#define CMD_NO_UPLOAD_GET -201

typedef enum _paymentCmd {
	CMD_GET_TXNINFO,
	CMD_UPLOAD_DATA,
} REQUESTCMD;



int ResultProcess(long lRetcode, char *szRetJsonData,REQUESTCMD ePackCmd,int iUpdateJsonFlag);
int GetRequestJson(char *pszJsonData, int iBufSize,REQUESTCMD ePackCmd);
int GetTimeStampUTCString(char *pszTimeStamp, int iBufSize);
int CalculateHMAC(const unsigned char *pucContent, int iContentLen,
		const unsigned char *pucapiKey, int iApiKeyLen, char *pszEncodeData, int iBufSize);
int Base64Encode(const char *szStr, int iStrLen, char *pszEncode, int iEncodeLen);
int RequestProcess(REQUESTCMD ePackCmd,int iUpdateJsonFlag);
#endif
