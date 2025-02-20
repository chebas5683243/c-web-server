#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

#define MAX_HEADERS 50
#define MAX_METHOD_LENGTH 16
#define MAX_RESOURCE_LENGTH 256
#define MAX_VERSION_LENGTH 16
#define MAX_KEY_LENGTH 128
#define MAX_VALUE_LENGTH 1024

typedef struct {
  char key[MAX_KEY_LENGTH];
  char value[MAX_VALUE_LENGTH];
} http_header_t;

typedef struct {
  char method[MAX_METHOD_LENGTH];
  char resource[MAX_RESOURCE_LENGTH];
  char version[MAX_VERSION_LENGTH];
  http_header_t headers[MAX_HEADERS];
  int header_count;
} http_request_t;

void parse_http_request(const char*, http_request_t*);
void print_http_request(http_request_t*);

#endif
