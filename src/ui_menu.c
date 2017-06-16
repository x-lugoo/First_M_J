/*------------------------------------------------------------
* FileName: ui_menu.c
* Author: Jeff
* Date: 2017-05-06
------------------------------------------------------------*/
#include<xui.h>
#include<string.h>
#include "global.h"

void DrawTitle_Back(void)
{
	XuiColor colorTitleBg;
	XuiColor colorTitleFg;
	char *bgfilename;
	XuiImg *imgBg;
	int titleWidth;
	int titleHeight = 24;
	int titleX;
	int titleY;
	int imgX;
	int imgY;
	char *title = "MiniPos";
	XuiFont *font;

	font = XuiCreateFont("./res/ProFontWindows.ttf", 0, 0);
	bgfilename = "./res/bg_240x320.png";
	imgBg = XuiImgLoadFromFile(bgfilename);
	/* set background */
		colorTitleBg.r = 0x10;
		colorTitleBg.g = 0x00;
		colorTitleBg.b = 0xfe;
		colorTitleBg.a = 0xff;
		XuiCanvasSetBackground(XuiRootCanvas(), XUI_BG_CENTER, imgBg, colorTitleBg);
		/* after SetBackground, the imgBg can free it */
		XuiImgFree(imgBg);
		imgBg = NULL;

	// draw title
		colorTitleBg.r = 0x10;
		colorTitleBg.g = 0x00;
		colorTitleBg.b = 0xfe;
		colorTitleBg.a = 0x70;
		XuiCanvasDrawRect(XuiRootCanvas(), 0, 0, XuiRootCanvas()->width, 40, colorTitleBg, 0, 1);
		titleWidth = XuiTextWidth(font, titleHeight, title);
		titleX = XUI_CENTER_X(0, XuiRootCanvas()->width, titleWidth);
		titleY = XUI_CENTER_Y(0, 40, titleHeight);

		colorTitleFg.r = 0xef;
		colorTitleFg.g = 0xef;
		colorTitleFg.b = 0xef;
		colorTitleFg.a = 0xff;
		XuiCanvasDrawText(XuiRootCanvas(), titleX, titleY, titleHeight, font, XUI_TEXT_NORMAL,
		colorTitleFg, title);
}


XuiWindow *GenButton(int x,int y,int color,char *text)
{
		XuiFont *font;
		XuiColor colorTitleFg;
		XuiColor colorMsgFg;
		XuiColor colorBtnBg;
		XuiWindow *btnCapture;
		XuiButtonStat statNormal;
		XuiButtonStat statPressed;

		font = XuiCreateFont("./res/PTS75F.ttf", 0, 0);
		colorTitleFg.r = 0x00;
		colorTitleFg.g = 0x00;
		colorTitleFg.b = 0x00;
		colorTitleFg.a = 0xff;
		/* create button */
		btnCapture = XuiCreateButton(XuiRootCanvas(), x, y, 64, 64);
		memset(&statNormal, 0, sizeof(statNormal));
		colorBtnBg.r = ((color >> 24)& 0xff);
		colorBtnBg.g = ((color >> 16)& 0xff);
		colorBtnBg.b = ((color >> 8)& 0xff);
		colorBtnBg.a = ((color >> 0)& 0xff);
		statNormal.btn_bg = colorBtnBg;
		statNormal.btn_round = 0;
		statNormal.img_style = XUI_BG_NORMAL;
		statNormal.text_fg = colorTitleFg;
		statNormal.text_font = font;
		statNormal.text_height = 12;
		statNormal.text_x = XUI_CENTER_X(0, 64, XuiTextWidth(statNormal.text_font, statNormal.text_height, text));
		statNormal.text_y = 30;
		//PaxLog(LOG_INFO,"(statNormal.text_x)=%d,y=%d",statNormal.text_x,statNormal.text_y);
		colorBtnBg.a = 0xa0;
		statNormal.text = text;
		XuiButtonSetStat(btnCapture, XUI_BTN_NORMAL, &statNormal);
		statPressed = statNormal;
		statPressed.btn_bg = colorBtnBg;
		XuiButtonSetStat(btnCapture, XUI_BTN_PRESSED, &statPressed);
	return (XuiWindow*)btnCapture;
}
