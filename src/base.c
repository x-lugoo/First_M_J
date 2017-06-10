/*------------------------------------------------------------
* FileName: base.c
* Author: Jeff
* Date: 2017-04-25
------------------------------------------------------------*/

#include "global.h"

long Filesize(const char *pszFilename)
{
	struct stat buf;
	int iRet=0;

	iRet = stat(pszFilename, &buf);
	if ( iRet < 0 ) {
		Pax_Log(LOG_ERROR, "stat return %d, errno=%d,%s, P1=%s", iRet, errno, strerror(errno), pszFilename);
		return OT_ERR;
	}
	return (int)buf.st_size;
}

char *Strrpl(char *pszStrIn, char *pszStrOut, int iOutlen, const char *pszSrc, char *pszDst)
{
    char *p = pszStrIn;
    unsigned int  uiLen = iOutlen - 1;

    if((NULL == pszSrc) || (NULL == pszDst) || (NULL == pszStrIn) || (NULL == pszStrOut)) {
        return NULL;
    }
    if(iOutlen <= 0) {
        return NULL;
    }

    while((*p != '\0') && (uiLen > 0)) {
        if( strncmp(p, pszSrc, strlen(pszSrc)) != 0 ) {
            int n = strlen(pszStrOut);

            pszStrOut[n] = *p;
            pszStrOut[n + 1] = '\0';

            p++;
            uiLen--;
        }
        else {
            strcat(pszStrOut, pszDst);	//TODO check size
            p += strlen(pszSrc);
            uiLen -= strlen(pszDst);
        }
    }
    return pszStrOut;
}

void DeleChar(char *pszStr, char ch)
{
	char *pTmp = pszStr;

	while (*pszStr != '\0')
	{
		if (*pszStr != ch)
		{
			*pTmp++ = *pszStr;
		}
		++pszStr;
	}
	*pTmp = '\0';
}

int PaxGetSN ( char *pszSerialNo, int iMaxLen )
{
	char szTmp[128]={0};
	int iSNLen = 0;

	memset(szTmp, 0, sizeof(szTmp));
	iSNLen = OsRegGetValue("ro.fac.sn", szTmp);

	if (iSNLen)
	{
		if (iSNLen > iMaxLen)
			iSNLen = iMaxLen;
		strncpy(pszSerialNo, szTmp, iSNLen);
	}
	else
	{
		strcpy(pszSerialNo, "00000000");
		return OT_ERR;
	}
	return OT_OK;
}

static int PaxSNdigitToPosUnitIddigits (char cDigit, char *pacDigits)
{
	int iConv=0;

	if (!pacDigits)
		return OT_ERR_INVALID_PARAM;

	if ((cDigit >= '0') && (cDigit <= '9'))
	{
			pacDigits[0] = '0';
			pacDigits[1] = cDigit;
	}
	else if ((cDigit >= 'A') && (cDigit <= 'Z'))
	{
		iConv = (cDigit - 'A') + 10;
		pacDigits[0] = (char)((iConv / 10) + 0x30);
		pacDigits[1] = (char)((iConv % 10) + 0x30);
	}
	else if ((cDigit >= 'a') && (cDigit <= 'z'))
	{
		iConv = (cDigit - 'a') + 36;
		pacDigits[0] = (char)((iConv / 10) + 0x30);
		pacDigits[1] = (char)((iConv % 10) + 0x30);
	}
	else
	{
		return OT_ERR;
	}

	return OT_OK;
}

int PaxSNtoPosUnitId(char *pszSerno, char *pszPosUnitID)
{
	int iRet=0, i=0, iOutIdx = 0, iInIdx = 0;
	int iSernoLen=0, iNumAlpha=0;

	memset(pszPosUnitID, 0x30, 15);

	iSernoLen = strlen(pszSerno);

	if (iSernoLen == 8)
	{
		iNumAlpha = 2;
	}
	else if (iSernoLen == 10)
	{
		iNumAlpha = 3;
	}
	else
		return OT_ERR;

	pszPosUnitID[iOutIdx++] = '7';
	pszPosUnitID[iOutIdx++] = iNumAlpha + 0x30;

	for (i = 0; i < 3; i++)
	{
		if (i < iNumAlpha)
		{
			iRet = PaxSNdigitToPosUnitIddigits(pszSerno[iInIdx], &pszPosUnitID[iOutIdx]);
			iInIdx ++;
		}
		else
			iRet = PaxSNdigitToPosUnitIddigits('0', &pszPosUnitID[iOutIdx]);

		if (iRet)
			return OT_ERR;

		iOutIdx += 2;
	}

	memcpy(&pszPosUnitID[iOutIdx], &pszSerno[iInIdx], (iSernoLen - iNumAlpha));
	if (iSernoLen == 8)
		pszPosUnitID[14] = 0x30;
	pszPosUnitID[15] = 0;

	return OT_OK;
}

int PaxGenUuid(char *pszPosUnitID, int iPosUnitIDLen, char *pszBeaconUuid)
{
	int i=0, iLen=0;
	char szTempPosUnitId[16+1]={0};
	uchar uszTemp[17]={0};
	uchar uszPosUnitID[10]={0};

	if ( (pszPosUnitID == NULL) || (pszBeaconUuid == NULL) ) {
		return OT_ERR_INVALID_PARAM;
	}

	memcpy(szTempPosUnitId, pszPosUnitID, 15);
	szTempPosUnitId[15] = '0';
	szTempPosUnitId[16] = 0;

	memset(uszTemp, 0, sizeof(uszTemp));
	memset(uszPosUnitID, 0, sizeof(uszPosUnitID));
	iLen = PaxCharsToHex(uszPosUnitID, szTempPosUnitId, iPosUnitIDLen);
	if ( iLen < 0 ) {
		Pax_Log(LOG_ERROR, "%s - %d", __FUNCTION__, __LINE__);
		return OT_ERR;
	}
	memset(uszTemp, 0x00, sizeof(uszTemp));
	memcpy(uszTemp, "PAX", 3);
	memcpy(uszTemp + 3, uszPosUnitID, iLen);
	memcpy(pszBeaconUuid, uszTemp, 11);

	//debug
	for (i=0; i < 16; i++) {
		Pax_Log(LOG_INFO, "%s - %d pszBeaconUuid=%02x", __FUNCTION__, __LINE__, pszBeaconUuid[i]);
	}

	return OT_OK;
}

unsigned char PaxCharToHex ( char c )
{
	if ((c >= '0') && (c <= '9'))
		return (c - '0');

	if ((c >= 'A') && (c < 'G'))
		return ((c - 'A') + 0x0a);

	if ((c >= 'a') && (c < 'g'))
		return ((c - 'a') + 0x0a);

	return 0xff;
}

int PaxCharsToHex ( unsigned char *pucBhex, char *pszBasc, unsigned int nc )
{
	unsigned int i=0;

	// Check Parameters
	if ((pucBhex == NULL) || (pszBasc == NULL))
	{
		return OT_ERR_INVALID_PARAM;
	}
	// convert two characters at time
	for (i = 0; i < (nc - (nc % 2)); i += 2)
	{
		pucBhex[(i + 1) / 2] =  (PaxCharToHex (pszBasc[i]) << 4) + PaxCharToHex (pszBasc[i + 1]);
	}
	// if odd number of character , the last hex value converted will be 0xX0 ,
	if ( (nc % 2) != 0 )
	{
		pucBhex[nc / 2] = PaxCharToHex (pszBasc[nc - 1]) << 4;
	}

	return (nc / 2 + (nc % 2));
}

static char *sg_pszResPath=NULL;
void Set_Prefix_Res_Path(const char *pszStr)
{
    if (pszStr == NULL)
    {
        return;
    }
    if (sg_pszResPath != NULL)
    {
        free(sg_pszResPath);
    }
    sg_pszResPath = (char*)malloc(strlen(pszStr) + 1);
    memset(sg_pszResPath, 0, strlen(pszStr) + 1);
    memcpy(sg_pszResPath, pszStr, strlen(pszStr));
}
//if call this function without setPrefixResPath, it will return incoming string.
const char *Add_Prefix_Res_Path(const char *pszStr)
{
    int iAddedPathLen=0;
    static char s_szAddedPath[48]={0};
    char *p=NULL;

    if (pszStr == NULL)
    {
    	return NULL;
    }
    if (sg_pszResPath == NULL)
    {
        return pszStr;
    }
    iAddedPathLen = strlen(sg_pszResPath) + strlen(pszStr);
    if (iAddedPathLen >= sizeof(s_szAddedPath))
    {
        return NULL;
    }
    memset(s_szAddedPath, 0, sizeof(s_szAddedPath));
    memcpy(s_szAddedPath, sg_pszResPath, strlen(sg_pszResPath));
    memcpy(s_szAddedPath + strlen(sg_pszResPath), pszStr, strlen(pszStr));
    p = s_szAddedPath;

    return p;
}

// Pad leading charactes till specific length.
void PubAddHeadChars( uchar *pszStringInOut, uint uiTargetLen, uchar ucAddChar )
{
	uint	uiLen;

	if (pszStringInOut == NULL)
	{
		return;
	}

	uiLen = (uint)strlen((char *)pszStringInOut);
	if( uiLen>=uiTargetLen )
	{
		return;
	}

	memmove(pszStringInOut+uiTargetLen-uiLen, pszStringInOut, uiLen+1);
	memset(pszStringInOut, ucAddChar, uiTargetLen-uiLen);
}

// delete the specified continuous characters on the left of the string
void PubTrimHeadChars(uchar *pszStringInOut, uchar ucRemoveChar)
{
	uchar	*p;

	if( !pszStringInOut || !*pszStringInOut )
    {
        return;
    }

	for(p=pszStringInOut; *p && *p==ucRemoveChar; p++);
	if( p!=pszStringInOut )
	{
		while( (*pszStringInOut++ = *p++) );
	}
}

// convert a string number with amount format
void PubConvAmount(const uchar *pszPrefix, const uchar *pszIn, uchar ucDeciPos, uchar ucIgnoreDigit, uchar *pszOut, uchar ucMisc)
{
	int		ii, iInLen, iIntegerLen;
	uchar	bNegative, szBuff[40+1], szTemp[40];
	uchar	*pRead, *pWr;


	if ((pszIn == NULL) || (pszOut == NULL) || (strlen((char*)pszIn) > 40))
	{
		return;
	}

	sprintf((char *)szBuff, "%.40s", pszIn);
	PubTrimHeadChars(szBuff, '0');
	iInLen = strlen((char *)szBuff);			//get input amount length
	if (ucIgnoreDigit && iInLen>ucIgnoreDigit)	// trim ignored digits.
	{
		iInLen -= ucIgnoreDigit;
		szBuff[iInLen] = 0;
	}
	pRead  = szBuff;
	pWr = pszOut;

	bNegative  = ucMisc & GA_NEGATIVE;		//whether display negative amount

	if (pszPrefix!=NULL && *pszPrefix!=0)
	{
		pWr += sprintf((char *)pWr, "%.3s ", pszPrefix);
		//		pWr += MIN(strlen((char *)pszPrefix), 4);
	}
	if (bNegative)
	{
		*(pWr++) = '-';
	}

	// before decimal point
	if (iInLen>ucDeciPos)	// if value>1
	{
		iIntegerLen = iInLen - ucDeciPos;
		ii = iIntegerLen;
		while (ii--)
		{
			*(pWr++) = *(pRead++);
			if ((ii%3==0) && ii)
			{
				*(pWr++) = ',';
			}
		}
	}
	else
	{
		*(pWr++) = '0';
	}

	// decimal point and afterwards
	if (ucDeciPos!=0)
	{
		sprintf((char *)szTemp, "%.*s", ucDeciPos, pRead);
		PubAddHeadChars(szTemp, ucDeciPos, '0');
		sprintf((char *)pWr, ".%s", szTemp);
		//		sprintf((char *)pWr, ".%0*s", ucDeciPos, pRead);
		pWr += (ucDeciPos+1);
	}
	else
	{
		*(pWr++) = 0;
	}

	// Process suffix (if any)
	//...
}


