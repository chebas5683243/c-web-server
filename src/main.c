#include "router.h"
#include "webserver.h"
#include "json.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#define PORT 42069

server_t *server;

void get_hello_world(http_request_t* req, http_response_t* res) {
  puts(req->resource);
  puts("Hello World! wow");
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

  json_print(user);

  json_free(user);


  // signal(SIGINT, handle_sigint);

  // router_t* router = router_new();

  // router_add_route(router, route_new("GET", "/hello-world", get_hello_world));
  // router_add_route(router, route_new("GET", "/", get_hello_world));
  // router_add_route(router, route_new("GET", "/atest", get_hello_world));
  // router_add_route(router, route_new("POST", "/test", get_hello_world));
  // router_add_route(router, route_new("DELETE", "/dtest", get_hello_world));
  // router_add_route(router, route_new("GET", "/ztest", get_hello_world));

  // router_prepare(router);

  // server = server_new(PORT, router);
  // if (!server) return 1;

  // server_run(server);

  return 0;
}

