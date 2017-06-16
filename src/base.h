/*------------------------------------------------------------
* FileName: base.h
* Author: Jeff
* Date: 2017-04-25
------------------------------------------------------------*/
#ifndef BASE_H
#define BASE_H

//functions

int FileExist(const char *pszFilename);
long Filesize(const char *pszFilename);
char *Strrpl(char *pszStrIn, char *pszStrOut, int iOutlen, const char *pszSrc, char *pszDst);
void DeleChar(char *pszStr, char ch);
int PaxGetSN (char *pszSerialNo, int iMaxLen);
int PaxSNtoPosUnitId(char *pszSerno, char *pszPosUnitID);
int PaxGenUuid(char *pszPosUnitID, int iPosUnitIDLen, char *pszBeaconUuid);
unsigned char PaxCharToHex (char c);
int PaxCharsToHex (unsigned char *pucBhex, char *pszBasc, unsigned int nc);
void SetPrefixResPath(const char *pszStr);
const char *AddPrefixResPath(const char *pszStr);
int PaxLogInit(const char* pszTag);
int PaxLog(LOG_T Prio, const char *fmt, ...);
void PubConvAmount(const uchar *pszPrefix, const uchar *pszIn, uchar ucDeciPos,
		uchar ucIgnoreDigit, uchar *pszOut, uchar ucMisc);

#define GA_SEPARATOR	0x80
#define GA_NEGATIVE		0x40
#endif
