#ifndef SERVER_H
#define SERVER_H

#include "router.h"

typedef struct {
  router_t* router;
  int socket_fd;
} request_context_t;

typedef struct {
  router_t* router;
  int port;
} server_t;

server_t* server_new(int, router_t*);
void server_run(server_t*);
void server_free(server_t*);

#endif
