#include "webserver.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

struct server_t *server;

void handle_sigint() {
  printf("\nCaught SIGINT! Cleaning up...\n");
  if (server) {
    free(server);
    server = NULL;
  }
  exit(0);
}

int main() {
  signal(SIGINT, handle_sigint);
  struct server_t *server = server_new();

  if (!server) return 1;

  server->listen_and_serve();

  free(server);
  return 0;
}

