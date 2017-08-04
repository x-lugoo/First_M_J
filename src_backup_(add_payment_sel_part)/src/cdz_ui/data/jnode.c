#include "sm_all.h"
#ifdef __PROLIN__

#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "pax_log.h"




#define JNODE_CONFIG_AREA
char *json_file_path() {return "./data/json/newjason.dat";}
char *jnode_key_contents() {return "contents";}
char *jnode_key_type() {return "type";}
char *jnode_key_name() {return "name";}
char *jnode_key_id() {return "id";}
char *jnode_key_price() {return "price";}
char *jnode_key_variable_price() {return "variable_price";}
char *jnode_key_vat() {return "vat";}
char *jnode_type_folder() {return "folder";}
char *jnode_type_product() {return "product";}
#undef JNODE_CONFIG_AREA


#define JNODE_OPER_AREA
static json_t *jnode_get_key_value(void *jnode,char *key)
{
	json_t *object=jnode;
	json_t *value=json_object_get(object,key);
	return value;
}
static char *jnode_get_key_value_string(void *jnode,char *key)
{
	json_t *value=jnode_get_key_value(jnode,key);
	if (!json_is_string(value)) {
		return NULL;
	}
	return json_string_value(value);
}
static int jnode_get_key_value_int(void *jnode,char *key)
{
	json_t *value=jnode_get_key_value(jnode,key);
	if (!json_is_integer(value)) {
		return NULL;
	}
	return json_integer_value(value);
}
static double jnode_get_key_value_real(void *jnode,char *key)
{
	json_t *value=jnode_get_key_value(jnode,key);
	if (!json_is_real(value)) {
		return 0.0;
	}
	return json_real_value(value);
}

static json_t *jnode_get_key_value_array(void *jnode,char *key)
{
	json_t *value=jnode_get_key_value(jnode,key);
	if (!json_is_array(value)) {
		return NULL;
	}
	return value;
}
char *jnode_get_type(void *jnode) {return jnode_get_key_value_string(jnode,jnode_key_type());}
char *jnode_get_name(void *jnode) {return jnode_get_key_value_string(jnode,jnode_key_name());}
int jnode_get_id(void *jnode)
{
	json_t *json_id=jnode_get_key_value(jnode,jnode_key_id());
	if (json_is_integer(json_id)) {
		return jnode_get_key_value_int(jnode,jnode_key_id());
	}
	if (json_is_string(json_id)) {
		char *id_str=jnode_get_key_value_string(jnode,jnode_key_id());
		return atoi(id_str);
	}
	return 0;
}
int jnode_get_price(void *jnode)
{
	json_t *json_price=jnode_get_key_value(jnode,jnode_key_price());
	if (json_is_integer(json_price)) {
		return jnode_get_key_value_int(jnode,jnode_key_price());
	}
	if (json_is_string(json_price)) {
		char *price_str=jnode_get_key_value_string(jnode,jnode_key_price());
		return atoi(price_str);
	}
	return 0;
}

double jnode_get_vat(void *jnode)
{
	json_t *json_vat=jnode_get_key_value(jnode,jnode_key_vat());
	if (json_is_real(json_vat)) {
		return jnode_get_key_value_real(jnode,jnode_key_vat());
	}
	if (json_is_string(json_vat)) {
		char *vat_str=jnode_get_key_value_string(jnode,jnode_key_vat());
		return atof(vat_str);
	}
	return 0.0;
}

int jnode_get_variable_price(void *jnode)
{
	json_t *json_variable_price=jnode_get_key_value(jnode,jnode_key_variable_price());
	if (json_is_boolean(json_variable_price) && json_is_true(json_variable_price)) {
		return variable_price_true();
	}
	return variable_price_false();
}

json_t *jnode_get_contents(void *jnode) {return jnode_get_key_value_array(jnode,jnode_key_contents());}
static int jnode_type_is_value(void *jnode,char *type_value)
{
	if (!strcmp(jnode_get_type(jnode),type_value)) {
		return 1;
	}
	return 0;
}
int jnode_type_is_folder(void *jnode) {return jnode_type_is_value(jnode,jnode_type_folder());}
int jnode_type_is_product(void *jnode) {return jnode_type_is_value(jnode,jnode_type_product());}

#undef JNODE_OPER_AREA



#define JNODE_LIST_AREA
static void *jnode_root()
{
	static void *s_jnode=NULL;
	if (!s_jnode) {
		//read json from file
		json_t *json_root_arr=json_load_file(json_file_path(),0,NULL);
		if (!json_root_arr) {
			ERR("json_load_file=%x, fail",json_root_arr);
			return -1;
		}

		json_t *json=json_object();
		json_object_set(json,jnode_key_type(),json_string("folder"));
		json_object_set(json,jnode_key_name(),json_string("root"));
		json_object_set(json,jnode_key_contents(),json_root_arr);
		s_jnode=json;
	}
	return s_jnode;	
}
static int jnode_traverse_init_list(void *jnode,void **jnode_list,int *pindex)
{
	
	jnode_list[*pindex]=jnode;
	*pindex+=1;
	if (!jnode_type_is_folder(jnode)) {return 0;}

	json_t *array=jnode_get_contents(jnode);
	json_t *obj=NULL;
	int i=0;
	json_array_foreach(array,i,obj) {
		jnode_traverse_init_list(obj,jnode_list,pindex);
	}
	return 0;
}
static int s_jnode_init_index=0;
void *jnode_list_get_node(int i)
{
	static int first=1;
	static void *list[128]={0};
	if (first) {
		jnode_traverse_init_list(jnode_root(),list,&s_jnode_init_index);
		first=0;
	}
	return list[i];
}
int jnode_list_count() {jnode_list_get_node(0);return s_jnode_init_index;}
#undef JNODE_LIST_AREA






#define JNODE_AREA
void *jnode_get_father(void *jnode)
{
	int i;
	for (i=0;i!=jnode_list_count();++i) {
		void *jnode_father=jnode_list_get_node(i);
		if (jnode_type_is_folder(jnode_father)) {
			json_t *array=jnode_get_contents(jnode_father);
			int i;
			json_t *obj;
			json_array_foreach(array,i,obj) {
				if (obj==(void *)jnode) {
					return jnode_father;
				}
			}
		}
	}
	return NULL;
}
void *jnode_get_first_son(void *jnode)
{
	if (jnode_type_is_folder(jnode)) {
		json_t *array=jnode_get_contents(jnode);
		return json_array_get(array,0);
	}
	return NULL;
}
void *jnode_get_next_brother(void *jnode)
{
	int i;
	for (i=0;i!=jnode_list_count();++i) {
		void *jnode_father=jnode_list_get_node(i);
		if (jnode_type_is_folder(jnode_father)) {
			json_t *array=jnode_get_contents(jnode_father);
			int i;
			json_t *obj;
			json_array_foreach(array,i,obj) {
				if (obj==(void *)jnode) {
					int next_brother_index=i+1;
					if (next_brother_index<json_array_size(array)) {
						return json_array_get(array,next_brother_index);
					} else {
						return NULL;
					}
				}
			}
		}
	}
	return NULL;
}
int jnode_get_index(void *jnode)
{
	int i;
	for (i=0;i!=jnode_list_count();++i) {
		if (jnode==jnode_list_get_node(i)) {
			return i;
		}
	}
}
#undef JNODE_AREA



#endif//ifdef __PROLIN__

