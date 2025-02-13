#include "webserver.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

struct Server *server;

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
  struct Server *server = server_new();

  if (!server) return 1;

  server->listenAndServe();

  free(server);
  return 0;
}

