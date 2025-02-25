#include "http-helper.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void parse_http_request(const char* raw_request, http_request_t* request) {
  char *request_copy = strdup(raw_request);
  if (!request_copy) return;

  char *line = strtok(request_copy, "\r\n");
  if (!line) {
    free(request_copy);
    return;
  }

  sscanf(line, "%15s %255s %15s", request->method, request->resource, request->version);

  request->header_count = 0;
  while ((line = strtok(NULL, "\r\n")) && request->header_count < MAX_HEADERS) {
    char *colon = strchr(line, ':');
    if (!colon) continue;

    *colon = '\0';
    char *key = line;
    char *value = colon + 1;

    while (*value == ' ') value++;

    request->headers[request->header_count].key = strdup(key);
    request->headers[request->header_count].value = strdup(value);
    request->header_count++;
  }

  free(request_copy);
}

void print_http_request(http_request_t *request) {
  printf("Method: %s\n", request->method);
  printf("Resource: %s\n", request->resource);
  printf("Version: %s\n", request->version);
  printf("Headers:\n");

  for (int i = 0; i < request->header_count; i++) {
    printf("  %s: %s\n", request->headers[i].key, request->headers[i].value);
  }
}

void http_request_free(http_request_t *request) {
  if (!request) return;
  for (int i = 0; i < request->header_count; i++) {
    free(request->headers[i].key);
    free(request->headers[i].value);
  }
}

http_response_t *http_response_new(int status, http_header_t *headers, int header_count, const char *body) {
  http_response_t *response = malloc(sizeof(http_response_t));
  if (!response) return NULL;

  response->status = status;
  response->header_count = header_count;
  response->body = body ? strdup(body) : NULL;

  for (size_t i = 0; i < header_count && i < MAX_HEADERS; i++) {
    response->headers[i] = headers[i];
  }

  return response;
}

void http_response_free(http_response_t *response) {
  if (!response) return;

  for (size_t i = 0; i < response->header_count; i++) {
    free(response->headers[i].key);
    free(response->headers[i].value);
  }

  free(response->body);
  free(response);
}

void http_response_stringify(http_response_t *response, char *buffer, size_t buffer_size) {
  if (!response || !buffer) return;

  snprintf(buffer, buffer_size, "HTTP/1.1 %d\r\n", response->status);
  size_t offset = strlen(buffer);

  for (size_t i = 0; i < response->header_count; i++) {
    snprintf(buffer + offset, buffer_size - offset, "%s: %s\r\n", response->headers[i].key, response->headers[i].value);
    offset = strlen(buffer);
  }

  snprintf(buffer + offset, buffer_size - offset, "\r\n%s", response->body ? response->body : "");
}
