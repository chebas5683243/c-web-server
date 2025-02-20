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

    strncpy(request->headers[request->header_count].key, key, MAX_KEY_LENGTH);
    strncpy(request->headers[request->header_count].value, value, MAX_VALUE_LENGTH);
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
