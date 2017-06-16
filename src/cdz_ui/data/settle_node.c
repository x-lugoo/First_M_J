#include "sm_all.h"
#ifdef __PROLIN__

#include <stdlib.h>
#include "pax_log.h"

#define SETTLE_AREA

#define PRODUCT_AREA
typedef char NODE_T;
static int s_product_list_node_count;
extern double product_node_get_vat(FNODE_T *node);

NODE_T *product_list_node(int i)
{
	static NODE_T *s_list[128]={0};
	static int first=1;
	if (first) {
		s_product_list_node_count=0;
		int i;
		int count=family_tree_get_count();
		for (i=0;i!=count;++i) {
			NODE_T *node=family_tree_get_node(i);
			//PaxLog(LOG_INFO,"product[name:%s]  func:%s,line:%d",node_get_name(node),__FUNCTION__,__LINE__);
			if (node_is_product(node)) {
				s_list[s_product_list_node_count++]=node;
				//PaxLog(LOG_INFO,"product[name:%s][variable_price:%d],func:%s,line:%d",node_get_name(node),product_node_get_variable_price(node),__FUNCTION__,__LINE__);
			}
		}
		first=0;
	}
	return s_list[i];
}
int product_list_node_count() {product_list_node(0);return s_product_list_node_count;}
#undef PRODUCT_AREA

#define SETTLE_PRODUCT_AREA
static int s_settle_product_list_node_count=0;
NODE_T *settle_product_list_node(int i)
{
	static NODE_T *s_list[128]={0};
	static int update_flag=1;
	if (update_flag) {
		memset(s_list,0,sizeof(s_list));
		s_settle_product_list_node_count=0;
		int count=product_list_node_count();
		int i;
		for (i=0;i!=count;++i) {
			NODE_T *node=product_list_node(i);
			//ERR("product[name:%s]",node_get_name(node));
			if (product_node_get_count(node)>0) {
				s_list[s_settle_product_list_node_count++]=node;
			}
		}
		update_flag=1;
	}
	return s_list[i];
}
int settle_product_list_node_count() {settle_product_list_node(0);return s_settle_product_list_node_count;}
#undef SETTLE_PRODUCT_AREA

int settle_product_count()
{
	int count=0;
	int i;
	for (i=0;i!=product_list_node_count();++i) {
		NODE_T *node=product_list_node(i);
		count+=product_node_get_count(node);
	}
	return count;
}

double settle_product_vat()
{
	double vat=0.0;
	int i;
	for (i=0;i!=product_list_node_count();++i) {
		NODE_T *node=product_list_node(i);
		vat+=product_node_get_count(node) * product_node_get_vat(node);
		ERR("product_node_get_vat(node)=%f",product_node_get_vat(node));
	}
	return vat;
}



int settle_product_amount()
{
	int amount=0;
	int i;
	for (i=0;i!=product_list_node_count();++i) {
		NODE_T *node=product_list_node(i);
		amount+=product_node_get_count(node) * product_node_get_price(node);
	}
	//ERR("------------------------------amount=%d",amount);
	return amount;
}
#undef SETTLE_AREA

#endif//__PROLIN__

