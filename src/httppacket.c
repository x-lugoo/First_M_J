/*
 * httppacket.c
 *
 *  Created on: 2017-5-1
 *      Author: Jeff
 */

#include "global.h"


#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include "jansson.h"
#include "jansson_config.h"
#include <string.h>
#include <osal.h>
#include <time.h>

static char *spRetJson;

int GetRequestJson(char *pszJsonData, int iBufSize,REQUESTCMD ePackCmd)
{
	int iRet;

	if(ePackCmd == CMD_GET_TXNINFO)
	{
		return 0;
	}
	else if(ePackCmd == CMD_UPLOAD_DATA && glReverFlag == '0')
	{
		iRet = PackOrderApiJson(pszJsonData);
		if(iRet)
		{
			return iRet;
		}
		if(strlen(pszJsonData) > iBufSize)
		{
			return -1;
		}
		iRet = SaveFile(FILE_REVERSAL,pszJsonData,strlen(pszJsonData));
		if(iRet)
		{
			return iRet;
		}
		return 0;
	}       //if the next upload is fail,will remove all the uploading data
	else if(ePackCmd == CMD_UPLOAD_DATA && glReverFlag == '1') 
	{
		glReverFlag = '0';
		iRet = SaveFile(FILE_REVERSALFlAG,&glReverFlag,1);
		if(iRet)
		{
			return iRet;
		}
		iRet = LoadReverdata(pszJsonData);
		if(iRet)
		{
			return iRet;
		}
		remove(FILE_REVERSAL);
	}
	
	return 0;
}
/********************************************************
lRetcode: the return value of get or post
szRetJsonData: return json data from back-end
ePackCmd:get or post
iUpdateJsonFlag:whether to update json data(when back-end return tag:update_categrade
				iUpdataJsonFlag will be 1.

*********************************************************/


int ResultProcess(long lRetcode, char *szRetJsonData,REQUESTCMD ePackCmd,int iUpdateJsonFlag)
{
	int iRet;

	PaxLog(LOG_INFO, "entrying	,lRetcode=%d		%s - %d",lRetcode, __FUNCTION__, __LINE__);
	if(lRetcode == HTTP_AUTH_ERR)
	{
		PaxLog(LOG_INFO, "%s - %d Http code = 401! Response", __FUNCTION__, __LINE__);
		return lRetcode;
	}
	else if(lRetcode != HTTP_OK && lRetcode != HTTP_ERR)
	{
		PaxLog(LOG_INFO, "%s - %d Http code = %d Response", __FUNCTION__, __LINE__,lRetcode);
		return lRetcode;
	}
	if(ePackCmd == CMD_UPLOAD_DATA)
	{
		if(szRetJsonData == NULL || strlen(szRetJsonData) == 0)
		{
			return 0;   //temporary return 0 to do
		}
		else
		{
			iRet = HandleReturnOrderJson(szRetJsonData);
			if(iRet)
			{
				return iRet;
			}
		}
	}
	else if(ePackCmd == CMD_GET_TXNINFO)
	{
	 PaxLog(LOG_ERROR,"iUpdateJsonFlag=%d %s,Line:%d",iUpdateJsonFlag,__FUNCTION__,__LINE__);
		SaveUpdateJsonData(szRetJsonData,iUpdateJsonFlag);
	}
	else
	{
		return CMD_NO_UPLOAD_GET;
	}
	return 0;
}

int Base64Encode(const char *szStr, int iStrLen, char *pszEncode, int iEncodeLen)
{
	int iLen=0;
    BIO *bmem=NULL,*b64=NULL;
    BUF_MEM *bptr=NULL;

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, szStr, iStrLen); //encode
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);
    if( bptr->length > iEncodeLen ) {
        PaxLog( LOG_INFO, "encode_len too small\n" );
        return HTTPPACKET_ERR;
    }
    memcpy(pszEncode, bptr->data, bptr->length);
    iLen = bptr->length;
    pszEncode[bptr->length] = '\0';
    BIO_free_all( b64 );

    return iLen;
}

/*
 * the HMAC_SHA256 transform looks like:
 * SHA256(K XOR opad, SHA256(K XOR ipad, text))
 * where K is an n byte key
 * ipad is the byte 0x36 repeated 64 times
 * opad is the byte 0x5c repeated 64 times
 * and text is the data being protected
 */
int CalculateHMAC(const unsigned char *pucContent, int iContentLen,
		const unsigned char *pucapiKey, int iApiKeyLen, char *pszEncodeData, int iBufSize)
{
	int i=0, iLen=0;
	unsigned char key_inner[65]={0};
	unsigned char key_outer[65]={0};
	unsigned char temp[SHA256_DIGEST_LENGTH]={0};
	unsigned char encode_inner[SHA256_DIGEST_LENGTH]={0};
	unsigned char encode_outer[SHA256_DIGEST_LENGTH]={0};
	unsigned char bufferIn[1024]={0};
	unsigned char bufferOut[1024]={0};

	/* if key is longer than 64 bytes reset it to key=sha256(key) */
	if ( iApiKeyLen > 64 ) {
		SHA256( pucapiKey, iApiKeyLen, temp );
		pucapiKey = temp;
		iApiKeyLen = SHA256_DIGEST_LENGTH;
	}
	/* start out by storing key in pads */
	memset(key_inner, 0, sizeof(key_inner));
	memset(key_outer, 0, sizeof(key_outer));
	memcpy(key_inner, pucapiKey, iApiKeyLen);
	memcpy(key_outer, pucapiKey, iApiKeyLen);
	/* XOR key with ipad and opad values */
	for ( i = 0; i < sizeof(key_inner)-1; i++ ) {
		key_inner[i] ^= 0x36;
		key_outer[i] ^= 0x5c;
	}
	/* perform inner SHA256*/
	memset(bufferIn, 0x00, sizeof(bufferIn));
	memcpy(bufferIn, key_inner, 64);
	memcpy(bufferIn + 64, pucContent, iContentLen);
	SHA256(bufferIn, 64 + iContentLen, encode_inner);
	/* perform outer SHA256*/
	memset(bufferOut, 0x00, sizeof(bufferOut));
	memcpy(bufferOut, key_outer, 64);
	memcpy(bufferOut + 64, encode_inner, SHA256_DIGEST_LENGTH);
	SHA256(bufferOut, 64 + SHA256_DIGEST_LENGTH, encode_outer);
	iLen = sizeof(encode_outer);
	if(iLen >= iBufSize) {
		return OT_ERR;
	}
	memcpy(pszEncodeData, encode_outer, iLen);

	return iLen;
}

int GetTimeStampUTCString(char *pszTimeStamp, int iBufSize)
{
	int iLen=0;
	time_t timep;
	struct tm *p=NULL;

	if ( (pszTimeStamp == NULL) || (iBufSize < 0) ) {
		return OT_ERR_INVALID_PARAM;
	}
	time(&timep);
	p=localtime(&timep);
	timep = mktime(p);
	iLen = snprintf((char *)pszTimeStamp, iBufSize, "%ld", timep);
	if ( (iLen < 0) || (iLen >= iBufSize) ) {
		PaxLog(LOG_INFO, "%s - %d, snprintf Failed!", __FUNCTION__, __LINE__);
		return OT_ERR;
	}

	return iLen;
}

int GetRequestUrl(char *szUrl, int iBufSize, REQUESTCMD ePackCmd)
{
	int iLen=0;

	if ( (szUrl == NULL) || (iBufSize < 0) ) {
		return OT_ERR_INVALID_PARAM;
	}
	if(CMD_GET_TXNINFO == ePackCmd)
	{
		iLen = snprintf(szUrl, iBufSize, "https://api.pos.goatms.com/api/v1/terminals/%s/product-catalogue",(glMainAppData.terminalID));
	}
	else if(CMD_UPLOAD_DATA== ePackCmd)
	{
	 	iLen = snprintf(szUrl, iBufSize, "https://api.pos.goatms.com/api/v1/orders");
	}
	if ( (iLen < 0) || (iLen >= iBufSize) ) {
		PaxLog(LOG_INFO, "%s - %d, snprintf Failed!", __FUNCTION__, __LINE__);
		return OT_ERR;
	}
	return iLen;
}
/*****************************************************************************
Below function must return size * nmemb,otherwise curl library will return 23,
param :data stands for the received data from curl library.
	   size stands for the size of package from curl library every times
	   nmemb stands for the number of the package,in common,it is 1
	   buffer_in stands for the outstream data.
here,I don't use buffer_in,because the function will be executed once curl gets any data
  or you can use Filestream on param buffer_in instead of spRetJson
********************************************************************************/
//once has received data, the function will be executed,maybe exectute twice
static int GetResponseJson(void *data, size_t size, size_t nmemb, void *buffer_in) //callback function
{
	
	PaxLog(LOG_INFO, "size =%d, nmemb=%d,%s--%d ",size,nmemb,__FUNCTION__, __LINE__);
	memcpy(spRetJson,data,size * nmemb);
	//PaxLog(LOG_INFO,"spRetJson=%s",spRetJson);
	spRetJson += size * nmemb;
    return size * nmemb;   //must return size * nmeme ,otherwise curl return 23
}

//static int Send_Recv_Process(char *pszUrl, char *pszAPIKey, char *pszJsonData, long *lRetcode, char *pszRetJsonData)
static int GetPutProcess(char *pszUrl, char *pszAPIKey, char *pszJsonData, long *lRetcode, char *pszRetJsonData, REQUESTCMD ePackCmd)
{
	CURL *pCurl=NULL;
	char szBuf[256] = {0};
	CURLcode eRes;
	struct curl_slist *ptHeaders=NULL;
    char szTemp[4096] = {0}; //unused
    
	if ( (pszUrl == NULL) || (pszAPIKey == NULL)) {
		return OT_ERR_INVALID_PARAM;
	}
    HidePromptWin();
    if(ePackCmd == CMD_GET_TXNINFO)
    {
    	DisplayPrompt("PLEASE WAIT", "Processing request...", MSGTYPE_UPLOADING, 0);
    }
    else if(ePackCmd == CMD_UPLOAD_DATA)
    {
    	 DisplayPrompt("PLEASE WAIT", "Processing request...", MSGTYPE_UPLOADING, 0);
    }	
   
	spRetJson = pszRetJsonData;
	PaxLog(LOG_INFO, "%s - %d pszUrl = %s", __FUNCTION__, __LINE__, pszUrl);
	PaxLog(LOG_INFO, "%s - %d pszAPIKey = %s", __FUNCTION__, __LINE__, pszAPIKey);
	if(CMD_UPLOAD_DATA == ePackCmd)
	{
		PaxLog(LOG_INFO, "%s - %d new upload pszJsonData = %s", __FUNCTION__, __LINE__, pszJsonData);
	}
	pCurl = curl_easy_init();
	if ( pCurl == NULL ) {
		curl_easy_cleanup(pCurl);
		return OT_ERR;
	}
	curl_easy_setopt(pCurl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	snprintf(szBuf, sizeof(szBuf) - 1, "%s%s", pszAPIKey, ":x");
	curl_easy_setopt(pCurl, CURLOPT_USERPWD, szBuf); //Your credentials goes here
	curl_easy_setopt(pCurl, CURLOPT_URL, pszUrl);

	if(ePackCmd == CMD_GET_TXNINFO)
	{
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, GetResponseJson);//callback fun
		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, szTemp);//szTemp no useud
		
	}

	else if(ePackCmd == CMD_UPLOAD_DATA)
	{
		//ptHeaders = curl_slist_append(ptHeaders, "Content-Type: application/json");
		//curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, ptHeaders);
		curl_easy_setopt(pCurl, CURLOPT_POST, 1);
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, GetResponseJson);//callback fun
		curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, pszJsonData);
		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, szTemp);//szTemp no useud
	}

	curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1);	//debug
	curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 20);

	//for https
	if ( strncmp(pszUrl, "https", 5) == 0 ) {
		curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 2L); //modified by Kevin Liu 20160707, from 1L to 2L
	}
	PaxLog(LOG_INFO, "new start to perfrom %s - %d", __FUNCTION__, __LINE__);
	eRes = curl_easy_perform(pCurl);
	
	//PaxLog(LOG_INFO, "pszRetJsonData=%s		%s - %d",pszRetJsonData, __FUNCTION__, __LINE__);
	PaxLog(LOG_INFO, "%s - %d eRes = %d", __FUNCTION__, __LINE__, eRes);
	if ( eRes != CURLE_OK ) {
		curl_easy_cleanup(pCurl);
		return eRes;
	}
	eRes = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE , lRetcode);
	PaxLog(LOG_INFO, "%s - %d eRes = %d,", __FUNCTION__, __LINE__, eRes);
	if ( eRes != CURLE_OK ) {
		curl_easy_cleanup(pCurl);
		return eRes;
	}
	curl_easy_cleanup(pCurl);
	
	PaxLog(LOG_INFO, "test end");
	return OT_OK;
}

int RequestProcess(REQUESTCMD ePackCmd,int iUpdateJsonFlag)
{

	int iTimeStampLen;
	int iJsonDataLen;
	int iUrlLen;
	long lRetcode;
	int iRet;
	int iSaveFileRet;
	char szUrl[200];
	char szJsonData[MAX_JASON_DATA];
	char szTimeStamp[24];
	char szRetJsonData[MAX_JASON_DATA];
	char szAPIKey[128];


	memset(szUrl,0,sizeof(szUrl));
	memset(szJsonData,0,sizeof(szJsonData));
	memset(szTimeStamp,0,sizeof(szTimeStamp));
	memset(szRetJsonData,0,sizeof(szRetJsonData));
	memset(szAPIKey,0,sizeof(szAPIKey));

	strcpy(szAPIKey,"ZDI2YmZiODZkOTNhYzE4MjU1OGFlYjE4"); //apiKey is fixed is this project
	iUrlLen = GetRequestUrl(szUrl,sizeof(szUrl),ePackCmd);  //get URL to connect to back_end host
	if(iUrlLen < 0)
	{
		return OT_ERR;
	}
	iJsonDataLen = GetRequestJson(szJsonData, sizeof(szJsonData),ePackCmd); //get request json data
	if(iJsonDataLen < 0)
	{
		return OT_ERR;
	}
	iTimeStampLen = GetTimeStampUTCString(szTimeStamp,sizeof(szTimeStamp));//get local time
	if(iTimeStampLen < 0)
	{
		return OT_ERR;
	}
	
	iRet = GetPutProcess(szUrl, szAPIKey, szJsonData, &lRetcode, szRetJsonData, ePackCmd);
	if(iRet)
	{
		if(ePackCmd == CMD_UPLOAD_DATA && access(FILE_REVERSAL,F_OK) >= 0)
		{
			glReverFlag = '1';
			iSaveFileRet = SaveFile(FILE_REVERSALFlAG,&glReverFlag,1);
			if(iSaveFileRet)
			{
				PaxLog(LOG_INFO,"iRet =%d			fun:%s-line:%d",iRet,__FUNCTION__,__LINE__);
				return iSaveFileRet;
			}
			
		}
		return iRet;
	}
	iRet = ResultProcess(lRetcode, szRetJsonData,ePackCmd,iUpdateJsonFlag);
	if(iRet)
	{
		PaxLog(LOG_INFO,"iRet =%d			fun:%s-line:%d",iRet,__FUNCTION__,__LINE__);
		return iRet;
	}
	PaxLog(LOG_INFO,"removingFILE_REVERSAL	fun:%s-line:%d",__FUNCTION__,__LINE__);
	remove(FILE_REVERSAL);//process success,remove reversal data
	return 0;
}



