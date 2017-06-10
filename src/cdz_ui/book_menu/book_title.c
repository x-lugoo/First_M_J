#include "sm_all.h"
#ifdef __PROLIN__

#include <osal.h>
#include <xui.h>
#include <stdlib.h>
#include <string.h>
#include "pax_log.h"

#define REF_AREA
XuiColor create_xui_color(int argb_hex);
#undef REF_AREA

#define TITLE_CONFIG
int settle_title_win_width() {return settle_menu_win_width();}
int settle_title_win_height() {return settle_menu_win_height()*1/8;}
int settle_title_win_x() {return 0;}
int settle_title_win_y() {return 0;}
#undef TITLE_CONFIG

XuiWindow *book_title_win_addr()
{
	static XuiWindow *s_win;
	static int first=1;
	if (first) {
		s_win=XuiCreateCanvas(book_menu_win_addr(),settle_title_win_x(),settle_title_win_y(),book_title_win_width(),book_title_win_height());
		first=0;
	}
	return s_win;
}
XuiWindow *book_title_win()
{
	static int first=1;
	if (first) {
		XuiCanvasSetBackground(book_title_win_addr(),0,0,create_xui_color(book_title_win_color()));
		first=0;
	}
	return book_title_win_addr();
}


#define TITLE_SQUARE_CONFIG
int book_title_square_gap() {return 3*pu();}
int book_title_square_size() {return 2*pu();}
int book_title_square_width() {return 2*book_title_square_size()+2*book_title_square_gap()+title_product_count_text_width();}
int book_title_square_height() {return 2*book_title_square_size()+2*book_title_square_gap()+title_product_count_text_height();}
int book_title_square_x() {return title_gap();}
int book_title_square_y() {return (book_title_win_height()-book_title_square_height())/2;}
int book_title_square_color() {return book_title_win_text_color();}
#undef TITLE_SQUARE_CONFIG


int book_title_refresh()
{
	XuiClearArea(book_title_win(),0,0,book_title_win_width(),book_title_win_height());

	{//draw entire square
		XuiCanvasDrawRect(book_title_win(),0,book_title_win_height()-1,book_title_win_width(),1,
			create_xui_color(book_title_square_color()),0,0);
	}

	
	{//draw product_count
		XuiCanvasDrawText(book_title_win(),title_product_count_text_x(),title_product_count_text_y(),title_product_count_text_height(),
			title_product_text_font(),0,create_xui_color(title_product_count_text_color()),title_product_count_text());
	}
	
	{//draw product_count_square
		XuiCanvasDrawRect(book_title_win(),book_title_square_x(),book_title_square_y(),book_title_square_width(),book_title_square_height(),
			create_xui_color(book_title_square_color()),0,0);
	}

	
	{//draw product_amount
		XuiCanvasDrawText(book_title_win(),title_product_amount_text_x(),title_product_amount_text_y(),title_product_amount_text_height(),
			title_product_text_font(),0,create_xui_color(title_product_amount_text_color()),title_product_amount_text());
	}
	return 0;
}

#endif//__PROLIN__
