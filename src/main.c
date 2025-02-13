#include "webserver.h"

int main() {
  struct Server *server = server_new();

  server->listenAndServe();
  return 0;
}

