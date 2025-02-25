#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

#define MAX_HEADERS 50
#define MAX_METHOD_LENGTH 16
#define MAX_RESOURCE_LENGTH 256
#define MAX_VERSION_LENGTH 16

#include <stdlib.h>

typedef struct {
  char* key;
  char* value;
} http_header_t;

typedef struct {
  char method[MAX_METHOD_LENGTH];
  char resource[MAX_RESOURCE_LENGTH];
  char version[MAX_VERSION_LENGTH];
  http_header_t headers[MAX_HEADERS];
  int header_count;
} http_request_t;

typedef struct {
  int status;
  http_header_t headers[MAX_HEADERS];
  int header_count;
  char *body;
} http_response_t;

void parse_http_request(const char*, http_request_t*);
void print_http_request(http_request_t*);
void http_request_free(http_request_t*);
http_response_t *http_response_new(int status, http_header_t *headers, int header_count, const char *body);
void http_response_stringify(http_response_t *response, char *buffer, size_t buffer_size);
void http_response_free(http_response_t *response);

#endif
