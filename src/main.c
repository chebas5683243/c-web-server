#include "router.h"
#include "webserver.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#define PORT 42069

server_t *server;

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
  server = server_new(PORT, router);
  if (!server) return 1;

  server_run(server);

  server_free(server);

  return 0;
}

