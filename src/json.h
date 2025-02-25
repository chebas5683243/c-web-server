#ifndef JSON_H
#define JSON_H

#include <stdlib.h>

typedef enum {
  JSON_INTEGER,
  JSON_DECIMAL,
  JSON_STRING,
  JSON_BOOLEAN,
  JSON_NULL,
  JSON_ARRAY,
  JSON_OBJECT
} json_type_t;

typedef struct json_element_t json_element_t;
typedef struct json_object_t json_object_t;
typedef struct json_array_t json_array_t;

typedef struct{
  json_type_t type;
  union {
    long integer;
    double decimal;
    char* string;
    int boolean;
    json_object_t* object;
    json_array_t* array;
  };
} json_value_t;

struct json_element_t {
  char* key;
  json_value_t* value;
  json_element_t* next;
};

struct json_array_t {
  json_value_t** items;
  size_t size;
};

struct json_object_t {
  json_element_t* head;
  json_element_t* tail;
};

json_value_t* json_new_object();
json_value_t* json_new_int(long);
json_value_t* json_new_decimal(double);
json_value_t* json_new_string(char*);
json_value_t* json_new_bool(int);
json_value_t* json_new_null();
json_value_t* json_new_array();
void json_object_add(json_value_t*, char*, json_value_t*);
void json_array_add(json_value_t*, json_value_t*);
void json_free(json_value_t*);
char* json_stringify(json_value_t*);

#endif
