#include "webserver.h"
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
  signal(SIGINT, handle_sigint);

  router_t* router = router_new();

  router_add_route(router, route_new("GET", "/hello-world", get_hello_world));

  server = server_new(PORT, router);
  if (!server) return 1;

  server_run(server);

  return 0;
}

