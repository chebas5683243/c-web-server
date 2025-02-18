#include "router.h"
#include <stdlib.h>
#include <stdio.h>

routes_t* routes_new() {
  routes_t* routes = malloc(sizeof(routes_t) * 5);

  if (routes == NULL) {
    perror("setsockopt failed");
    exit(EXIT_FAILURE);
  }

  return routes;
}

router_t* router_new(){
  router_t* router = malloc(sizeof(router_t));

  router->routes = routes_new();

  if (!router) {
    return NULL;
  }
  return router;
}
