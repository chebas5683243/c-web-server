#include "json.h"
#include <ios>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

json_value_t* json_new_object(){
  json_value_t* object = (json_value_t*)malloc(sizeof(json_value_t));
  if (!object) return NULL;

  object->type = JSON_OBJECT;
  object->object = (json_object_t*)malloc(sizeof(json_object_t));

  if (!object->object) {
    free(object);
    return NULL;
  }

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
  value->string = strdup(string);

  if (!value->string) {
    free(value);
    return NULL;
  }

  return value;
}

json_value_t* json_new_bool(int boolean) {
  json_value_t* value = (json_value_t*)malloc(sizeof(json_value_t));
  if (!value) return NULL;
  value->type = JSON_BOOLEAN;
  value->boolean = boolean == 0 ? 0 : 1;
  return value;
}

json_value_t* json_new_null() {
  json_value_t* value = (json_value_t*)malloc(sizeof(json_value_t));
  if (!value) return NULL;
  value->type = JSON_NULL;
  return value;
}

json_value_t* json_new_array() {
  json_value_t* array = (json_value_t*)malloc(sizeof(json_value_t));
  if (!array) return NULL;

  array->type = JSON_ARRAY;
  array->array = (json_array_t*)malloc(sizeof(json_array_t));

  if (!array->array) {
    free(array);
    return NULL;
  }

  array->array->items = (json_value_t**)malloc(sizeof(json_value_t*)*100);

  if (!array->array->items) {
    free(array->array);
    free(array);
    return NULL;
  }

  array->array->size = 0;
  return array;
}

void json_object_add(json_value_t* object_el, char* key, json_value_t* value) {
  if (object_el->type != JSON_OBJECT || object_el->object == NULL) {
    printf("Error appending %s to object: Element is not a valid object", key);
    return;
  }

  json_element_t* new_el = (json_element_t*)malloc(sizeof(json_element_t));

  if (!new_el) {
    printf("Error allocating new element %s", key);
    return;
  }

  new_el->key = strdup(key);

  if (!new_el->value) {
    free(new_el);
    return;
  }

  new_el->value = value;
  new_el->next = NULL;

  if (!object_el->object->head) {
    object_el->object->head = new_el;
    object_el->object->tail = new_el;
    return;
  }

  object_el->object->tail->next = new_el;
  object_el->object->tail = new_el;
}

void json_array_add(json_value_t* array_el, json_value_t* value) {
  if (array_el->type != JSON_ARRAY || array_el->array == NULL) {
    puts("Error inserting to array: Element is not a valid array");
    return;
  }

  array_el->array->items[array_el->array->size] = value;
  array_el->array->size++;
}

void json_free(json_object_t*);
void json_array_free(json_array_t*);

void json_value_free(json_value_t* value) {
  if (!value) return;

  json_type_t type = value->type;
  switch (type) {
    case JSON_OBJECT:
      json_free(value->object);
      break;
    case JSON_ARRAY:
      json_array_free(value->array);
      break;
    case JSON_STRING:
      if (value->string) free(value->string);
      break;
    case JSON_INTEGER:
    case JSON_DECIMAL:
    case JSON_BOOLEAN:
    case JSON_NULL:
      break;
  }

  free(value);
}

void json_array_free(json_array_t* array) {
  if (!array) return;

  if (array->items) {
    for (int i = 0; i < array->size; i++) {
      json_value_free(array->items[i]);
    }
    free(array->items);
  }

  free(array);
}

void json_free(json_object_t* object) {
  if (!object) return;

  json_element_t* curr_el = object->head;

  while (curr_el) {
    if (curr_el->key) free(curr_el->key);

    json_element_t* tmp = curr_el->next;
    json_value_free(curr_el->value);
    free(curr_el);

    curr_el = tmp;
  }

  free(object);
}
