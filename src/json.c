#include "json.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// JSON NEW

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

json_value_t* json_new_int(long integer) {
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

// JSON MANIPULATION

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

  if (!new_el->key) {
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

// JSON FREE

void json_object_free(json_object_t*);
void json_array_free(json_array_t*);

void json_value_free(json_value_t* value) {
  if (!value) return;

  json_type_t type = value->type;
  switch (type) {
    case JSON_OBJECT:
      json_object_free(value->object);
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

void json_object_free(json_object_t* object) {
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

void json_free(json_value_t* json) {
  if (!json) return;

  if (json->type != JSON_OBJECT) {
    puts("json_free requires a json_value_t*");
    return;
  }

  json_object_free(json->object);
  free(json);
}

// JSON PRINT

#define INITIAL_CAPACITY 50
#define GROWTH_FACTOR 2
#define NUMERIC_CAPACITY 48

void string_append(char** buffer, size_t *capacity, char* suffix) {
  if (!buffer || !suffix) return;

  size_t new_len = strlen(suffix) + strlen(*buffer) + 1;

  if (new_len > *capacity) {
    *capacity = new_len * GROWTH_FACTOR;
    char* new_buffer = realloc(*buffer, *capacity);

    if (!new_buffer) return;
    *buffer = new_buffer;
  }

  strcat(*buffer, suffix);
}

void json_object_stringify(char** str, size_t* capacity, json_object_t* object);
void json_array_stringify(char** str, size_t* capacity, json_array_t* object);

void json_value_stringify(char** str, size_t* capacity, json_value_t* el) {
  char numeric_string[NUMERIC_CAPACITY];

  switch (el->type) {
    case JSON_STRING:
      string_append(str, capacity, "\"");
      string_append(str, capacity, el->string);
      string_append(str, capacity, "\"");
      break;
    case JSON_NULL:
      string_append(str, capacity, "null");
      break;
    case JSON_INTEGER:
      snprintf(numeric_string, NUMERIC_CAPACITY, "%ld", el->integer);
      string_append(str, capacity, numeric_string);
      break;
    case JSON_DECIMAL:
      snprintf(numeric_string, NUMERIC_CAPACITY, "%.7f", el->decimal);
      string_append(str, capacity, numeric_string);
      break;
    case JSON_BOOLEAN:
      string_append(str, capacity, el->boolean ? "true" : "false");
      break;
    case JSON_ARRAY:
      json_array_stringify(str, capacity, el->array);
      break;
    case JSON_OBJECT:
      json_object_stringify(str, capacity, el->object);
  }
}

void json_array_stringify(char** str, size_t* capacity, json_array_t* array) {
  if (!array) return;
  string_append(str, capacity, "[");
  for (int i = 0; i < array->size; i++) {
    json_value_stringify(str, capacity, array->items[i]);
    if (i != array->size - 1) {
      string_append(str, capacity, ",");
    }
  }
  string_append(str, capacity, "]");
}

void json_object_stringify(char** str, size_t* capacity, json_object_t* object) {
  if (!object) return;

  json_element_t* curr_el = object->head;

  string_append(str, capacity, "{");
  while (curr_el) {
    string_append(str, capacity, "\"");
    string_append(str, capacity, curr_el->key);
    string_append(str, capacity, "\":");
    json_value_stringify(str, capacity, curr_el->value);
    curr_el = curr_el->next;
    if (curr_el) string_append(str, capacity, ",");
  }
  string_append(str, capacity, "}");
}

void json_print(json_value_t* json) {
  if (!json) return;

  if (json->type != JSON_OBJECT) {
    puts("json_print requires a json_value_t*");
    return;
  }
  size_t initial_capacity = INITIAL_CAPACITY;
  char* json_string = calloc(initial_capacity, sizeof(char));

  json_object_stringify(&json_string, &initial_capacity, json->object);

  puts(json_string);
  free(json_string);
}
