#include "router.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

route_t* route_new(const char* method, const char* resource, http_handler_t handler) {
  route_t* route = malloc(sizeof(route_t));

  if (!route) {
    return NULL;
  }

  route->method = strdup(method);
  route->resource = strdup(resource);
  route->handler = handler;

  return route;
}

router_t* router_new() {
  router_t* router = malloc(sizeof(router_t));

  if (!router) {
    return NULL;
  }

  router->routes = NULL;
  router->route_count = 0;
  router->capacity = 0;

  return router;
}

void router_add_route(router_t* router, route_t* route) {
  if (!router || !route) return;

  if (router->route_count == router->capacity) {
    router->capacity = (router->capacity == 0) ? 2 : router->capacity * 2;
    route_t** routes = realloc(router->routes, sizeof(route_t*) * router->capacity);

    if (!routes) {
      perror("Failed to allocate memory for routes");
      return;
    }

    router->routes = routes;
  }

  router->routes[router->route_count] = route;
  router->route_count++;
}

void router_prepare();

void route_free(route_t* route) {
  if (!route) return;
  free(route->method);
  free(route->resource);
  free(route);
}

void router_free(router_t* router) {
  if (!router) return;

  for (int i = 0; i < router->route_count; i++) {
    route_free(router->routes[i]);
  }

  free(router->routes);
  free(router);
}
