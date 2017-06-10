#ifndef SM_ALL_H
#define SM_ALL_H
#include "Sm_all.h"
#define __PROLIN__

typedef struct family_node_t FNODE_T;
typedef char NODE_T;


int menu_test();
int settle_product_count();
int settle_product_amount();
int settle_product_list_node_count();
NODE_T *settle_product_list_node(int i);


char *node_get_name(FNODE_T *node);
char *node_get_type(FNODE_T *node);

int node_is_product(FNODE_T *node);
int node_is_folder(FNODE_T *node);
int product_node_get_id(FNODE_T *node);
int product_node_get_price(FNODE_T *node);
int product_node_get_count(FNODE_T *node);

int book_menu_process();
int smenu_get_ret_value();
int page_code_exit();
int page_code_ensure();
int settle_menu_process_get_value();
int page_code_back();
int page_code_ensure();
typedef enum
{
	stat_main_menu,
	stat_settle_menu,
	stat_exit,
	stat_payment_menu,
	
}MENU_SELECTION;

#endif//SM_ALL_H
