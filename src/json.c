#include "json.h"
#include <stdlib.h>

json_value_t* json_new_object(){
  json_value_t* object = (json_value_t*)malloc(sizeof(json_value_t));
  if (!object) return NULL;
  object->type = JSON_ARRAY;
  object->object = (json_object_t*)malloc(sizeof(json_object_t));
  object->object->head = NULL;
  object->object->tail = NULL;
  return object;
}

json_value_t* json_new_int(int integer) {
  json_value_t* value = (json_value_t*)malloc(sizeof(json_value_t));
  if (!value) return NULL;
  value->type = JSON_INTEGER;
  value->integer = integer;
  return value;
}

json_value_t* json_new_decimal(double decimal) {
  json_value_t* value = (json_value_t*)malloc(sizeof(json_value_t));
  if (!value) return NULL;
  value->type = JSON_DECIMAL;
  value->decimal = decimal;
  return value;
}

json_value_t* json_new_string(char* string) {
  json_value_t* value = (json_value_t*)malloc(sizeof(json_value_t));
  if (!value) return NULL;
  value->type = JSON_STRING;
  value->string = string;
  return value;
}

json_value_t* json_new_bool(int boolean) {
  json_value_t* value = (json_value_t*)malloc(sizeof(json_value_t));
  if (!value) return NULL;
  value->type = JSON_BOOLEAN;
  value->boolean = boolean == 0 ? 0 : 1;
  return value;
}

json_value_t* json_new_array() {
  json_value_t* array = (json_value_t*)malloc(sizeof(json_value_t));
  if (!array) return NULL;
  array->type = JSON_ARRAY;
  array->array = (json_array_t*)malloc(sizeof(json_array_t));
  array->array->items = (json_value_t**)malloc(sizeof(json_value_t*)*100);
  array->array->size = 0;
  return array;
}

void json_object_add(json_value_t* object, char* key, json_value_t* value) {

}

void json_array_add(json_value_t*, json_value_t*);
