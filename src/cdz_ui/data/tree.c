#include "sm_all.h"
#ifdef __PROLIN__

#include <stdlib.h>
#include <string.h>
#include "pax_log.h"

#define TREE_FAMILY_STRUCT_AREA
typedef struct family_node_t FNODE_T;
typedef struct family_node_t {
	char *name;
	char *type;
	int id;
	int price;
	int variable_price;
	int count;
	double vat;
	FNODE_T *father;
	FNODE_T *first_son;
	FNODE_T *next_brother;
} FNODE_T;
#undef TREE_FAMILY_STRUCT_AREA

double jnode_get_vat(jnode);


#define TREE_FAMILY_CONSTRUCT_AREA
static int s_family_tree_node_count=0;
void *create_family_tree()
{
	static void *s_tree=0;
	char *mallocNodeName;
	
	if (!s_tree) {
		s_family_tree_node_count=0;
		int mem_size=sizeof(FNODE_T) * jnode_list_count();
		FNODE_T *root=malloc(mem_size);
		memset(root,0,mem_size);
		int i;
		for (i=0;i!=jnode_list_count();++i) {
			FNODE_T *node=root+i;
			void *jnode=jnode_list_get_node(i);
			
			if(strlen(jnode_get_name(jnode)) > 14)
			{
				mallocNodeName = (char*)malloc(15);
				memset(mallocNodeName,0,15);
				strncpy(mallocNodeName,jnode_get_name(jnode),14);
				node->name =  mallocNodeName;
			} 
			else
			{
				node->name=jnode_get_name(jnode);
			}

			{//将type字串重定向到常量字串中，减少比较类型的延时
				if (!strcmp(jnode_get_type(jnode),jnode_type_product())) {
					node->type=jnode_type_product();
				}
				if (!strcmp(jnode_get_type(jnode),jnode_type_folder())) {
					node->type=jnode_type_folder();
				}
			}
			
			void *father=jnode_get_father(jnode);
			node->father=(father)?(root+jnode_get_index(father)):(0);

			void *first_son=jnode_get_first_son(jnode);
			node->first_son=(first_son)?(root+jnode_get_index(first_son)):(0);

			void *next_brother=jnode_get_next_brother(jnode);
			node->next_brother=(next_brother)?(root+jnode_get_index(next_brother)):(0);
			
			if (jnode_type_is_product(jnode)) {
				node->id=jnode_get_id(jnode);
				node->price=jnode_get_price(jnode);
				node->variable_price=jnode_get_variable_price(jnode);
				node->vat=jnode_get_vat(jnode);
				//ERR("node[%s].vat=%f",node->name,node->vat);
			}
			++s_family_tree_node_count;
		}
		s_tree=root;
	}
	return s_tree;
}
int family_tree_get_count() {create_family_tree();return s_family_tree_node_count;}
FNODE_T *family_tree_get_node(int i)
{
	if (i<family_tree_get_count()) {
		FNODE_T *first_node=create_family_tree();
		return first_node+i;
	}
	return NULL;
}
#undef TREE_FAMILY_CONSTRUCT_AREA





#define DATA_AREA
int id_none() {return -1;}
int price_none() {return -1;}
int count_none() {return -1;}
double vat_node() {return 0.0;}
int variable_price_true() {return 1;}
int variable_price_false() {return 0;}
char *node_get_name(FNODE_T *node) {return (node)?(node->name):(NULL);}
char *node_get_type(FNODE_T *node) {return (node)?(node->type):(NULL);}
int node_is_product(FNODE_T *node) {return (node && node_get_type(node)==jnode_type_product())?(1):(0);}
int node_is_folder(FNODE_T *node) {return (node && node_get_type(node)==jnode_type_folder())?(1):(0);}
int product_node_get_id(FNODE_T *node) {return (node_is_product(node))?(node->id):(id_none());}
int product_node_get_ifVariable_price(FNODE_T *node) {return (node_is_product(node))?(node->variable_price):(variable_price_false());}
int product_node_set_variable_price(FNODE_T *node,int varPrice) {node->variable_price = varPrice;return 0;} 
int product_node_get_price(FNODE_T *node) {return (node_is_product(node))?(node->price):(price_none());}
int product_node_set_price(FNODE_T *node,int price) {node->price=price;return 0;} 
int product_node_get_count(FNODE_T *node) {return (node_is_product(node))?(node->count):(count_none());}
int product_node_set_count(FNODE_T *node,int count) {node->count=count;return 0;}

double product_node_get_vat(FNODE_T *node) {
return (node_is_product(node))?(node->vat):(vat_node());
}

int prouduct_node_set_vat(FNODE_T *node,double vat) {node->vat=vat;return 0;}
#undef DATA_AREA

#define RELATION_AREA
FNODE_T *node_get_father_node(FNODE_T *node) {return (node)?(node->father):(NULL);}
FNODE_T *node_get_first_son_node(FNODE_T *node) {return (node)?(node->first_son):(NULL);}
FNODE_T *node_get_next_brother_node(FNODE_T *node) {return (node)?(node->next_brother):(NULL);}
int node_get_index(FNODE_T *node) {return node-(FNODE_T *)family_tree_get_node(0);}
#undef RELATION_AREA


#endif//ifdef __PROLIN__

