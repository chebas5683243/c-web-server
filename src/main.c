#include "http-helper.h"
#include "json.h"
#include "webserver.h"
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

#define PORT 42069

server_t *server;

void handler(http_request_t *req, int socket_fd) {
  json_value_t* addr = json_new_object();
  json_object_add(addr, "country", json_new_string("US"));
  json_object_add(addr, "state", json_new_string("California"));
  json_object_add(addr, "city", json_new_string("Corona"));
  json_object_add(addr, "number", json_new_int(123));

  json_value_t* nicknames = json_new_array();
  json_array_add(nicknames, json_new_string("Coder"));
  json_array_add(nicknames, json_new_string("Coder2"));

  json_value_t* user = json_new_object();
  json_object_add(user, "address", addr);
  json_object_add(user, "nicknames", nicknames);
  json_object_add(user, "married", json_new_bool(0));
  json_object_add(user, "age", json_new_int(18));
  json_object_add(user, "heigth", json_new_decimal(1.95));
  json_object_add(user, "additional-info", json_new_null());

  char* json_string = json_stringify(user);

  char json_string_size[48];
  snprintf(json_string_size, 48, "%ld", strlen(json_string));

  http_header_t headers[] = {
    {"Content-Type", "application/json"},
    {"Connection", "close"},
    {"Content-Length", json_string_size}
  };

  http_response_t* response = http_response_new(200, headers, 3, json_string);
  char response_string[10000];
  http_response_stringify(response, response_string, 10000);

  send(socket_fd, response_string, strlen(response_string), 0);

  free(json_string);
  free(response);
  json_free(user);
}

void handle_sigint() {
  printf("\nCaught SIGINT! Cleaning up...\n");
  if (server) {
    server_free(server);
    server = NULL;
  }
  exit(0);
}

int main() {
  signal(SIGINT, handle_sigint);

  router_t* router = router_new();

  router_add_route(router, route_new("GET", "/hello-world", handler));
  router_add_route(router, route_new("GET", "/", handler));
  router_add_route(router, route_new("GET", "/atest", handler));
  router_add_route(router, route_new("POST", "/test", handler));
  router_add_route(router, route_new("DELETE", "/dtest", handler));
  router_add_route(router, route_new("GET", "/ztest", handler));

  router_prepare(router);

  server = server_new(PORT, router);
  if (!server) return 1;

  server_run(server);

  return 0;
}
