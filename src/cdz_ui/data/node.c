#include "sm_all.h"
#ifdef __PROLIN__


#include <xui.h>
#include <stdlib.h>
#include <string.h>
#include "pax_log.h"





#define NODE_RELATION_AREA
typedef char NODE_T;
int node_get_seq_of_father(NODE_T *node)
{
	if (node==NULL) {return -1;}
	NODE_T *father=node_get_father_node(node);
	if (father==NULL) {return -1;}

	int seq=0;
	NODE_T *n;
	for (n=node_get_first_son_node(father);n!=NULL;n=node_get_next_brother_node(n)) {
		++seq;
		if (n==node) {return seq;}
	}
	return -1;
}
NODE_T *node_get_seq_son_node(NODE_T *father,int seq)
{
	NODE_T *node=node_get_first_son_node(father);
	int i;
	for (i=0;i!=seq;++i) {
		if (node==NULL) {break;}
		node=node_get_next_brother_node(node);
	}
	return node;
}
int node_get_son_count(NODE_T *father)
{
	int count=0;
	NODE_T *node;
	for (node=node_get_first_son_node(father);node!=NULL;node=node_get_next_brother_node(node)) {
		++count;
	}
	return count;
}
int node_get_max_page(NODE_T *node)
{
	int son_count=node_get_son_count(node);
	int page_max_btn_count=book_page_btn_count_max();
	int full_page_count=son_count/page_max_btn_count;
	int page_count=(son_count%page_max_btn_count) ? (full_page_count+1) : (full_page_count);
	return page_count;
}
#undef NODE_RELATION_AREA


#define NODE_EXTERN_INTF

#define NODE_TITLE_AREA
XuiFont *node_get_text_font(NODE_T *node)
{
	return get_xui_text_font();
}
char *node_get_text(NODE_T *node)
{
	return node_get_name(node);
}

int node_get_title_color(NODE_T *node) {return book_title_win_text_color();}
int node_get_title_height(NODE_T *node) {return book_title_win_text_height();}
int node_get_title_x(NODE_T *node)
{
	if (node) {
		int text_width=XuiTextWidth(node_get_text_font(node),node_get_title_height(node),node_get_text(node));
		int text_x=(book_title_win_width()-text_width)/2;
		return text_x;
	}
	return 0;
}
int node_get_title_y(NODE_T *node) {return book_title_win_text_y();}
#undef NODE_TITLE_AREA

#define NODE_BTN_AREA
int node_get_btn_width(NODE_T *node) {return book_page_btn_width();}
int node_get_btn_height(NODE_T *node) {return book_page_btn_height();}
int node_get_btn_x(NODE_T *node)
{
	int x_seq=(node_get_seq_of_father(node)-1)%book_page_btn_count_x_max();
	return x_seq*node_get_btn_width(node)+(x_seq+1)*book_page_btn_x_gap();
}
int node_get_btn_y(NODE_T *node)
{
	int page_seq=(node_get_seq_of_father(node)-1)%(book_page_btn_count_max());
	int y_seq=page_seq/book_page_btn_count_x_max();
	return y_seq*book_page_btn_height() + (y_seq+1)*book_page_btn_y_gap();
}
int folder_color() {return 0xff74b243;}
int node_get_btn_bg_color(NODE_T *node)
{
	if (node_is_folder(node)) {
		return folder_color();
	}
	if (node_is_product(node)) {
		if (get_menu_stat()==menu_stat_settle()) {
			return minipos_page_background_color();
		}
		//return minipos_title_background_color();
		return minipos_product_button_color();
	}
	return 0xff0000ff;
}
int node_get_btn_bg_color_p(NODE_T *node)
{
	return book_page_btn_p_bg_color_default();
}
#undef NODE_BTN_AREA

#define FNODE_BTN_TEXT_AREA
char *node_get_btn_text(NODE_T *node) {return node_get_name(node);}
int node_get_btn_text_flag(NODE_T *node) {return book_page_btn_text_is_existed();}
int node_get_btn_text_height(NODE_T *node) {return book_page_btn_text_height();}
int node_get_btn_text_y(NODE_T *node) {return book_page_btn_text_y();}
int node_get_btn_text_x(NODE_T *node)
{
	if (node) {
		int text_width=XuiTextWidth(node_get_text_font(node),node_get_btn_text_height(node),node_get_text(node));
		int text_x=(node_get_btn_width(node)-text_width)/2;
		return text_x;
	}
	return 0;
}
int node_get_btn_text_color(NODE_T *node)
{
	
	if (node_is_folder(node)) {
		return 0xffffffff;
	}
	if (node_is_product(node)) {
		if (settle_page_process_get_select_node()==node) {
			return folder_color();
		}
		return 0xff000000;
	}
	return 0xffff0000;
}
#undef FNODE_BTN_TEXT_AREA

int node_get_btn_key(NODE_T *node)
{
	if (node) {
		int key=node_get_index(node)+book_page_btn_key_base();
		return key;
	}
	return 0;
}




#define NODE_IMG_AREA
XuiImg *node_get_img(NODE_T *node)
{
	return 0;
}
int node_get_img_x(NODE_T *node) 
{
	return 0;
}
int node_get_img_y(NODE_T *node)
{
	return 0;
}
XuiImg *node_get_img_p(NODE_T *node)
{
	return 0;
}
int node_get_img_x_p(NODE_T *node) 
{
	return 0;
}
int node_get_img_y_p(NODE_T *node)
{
	return 0;
}
#undef NODE_IMG_AREA

#undef NODE_EXTERN_INTF




#endif//ifdef __PROLIN__


