


#include <stdio.h>
#include <json-c/json.h>
#include <stdlib.h>
#include <string.h>

json_object* add_obj_to_json(json_object *json_obj_out,char *k,char* v);
json_object * convert_json_data(char* s);
const char *json_common_get_string(json_object *js_obj, char *key );
const char* convert_json_to_str(json_object *js_obj);