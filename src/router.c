#include "router.h"
#include <unistd.h>
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

int route_cmp(const void* r1, const void* r2) {
  route_t* route1 = *(route_t**) r1;
  route_t* route2 = *(route_t**) r2;

  int method_cmp = strcmp(route1->method, route2->method);

  if (method_cmp == 0) {
    return strcmp(route1->resource, route2->resource);
  }

  return method_cmp;
}

void print_routes(router_t* router) {
  for(int i = 0; i < router->route_count; i++) {
    printf("%s %s\n", router->routes[i]->method, router->routes[i]->resource);
  }
}

void router_prepare(router_t* router) {
  // print_routes(router);
  qsort(router->routes, router->route_count, sizeof(route_t*), route_cmp);
  // puts("---");
  // print_routes(router);
}

void route_free(route_t* route) {
  if (!route) return;
  if (route->method) free(route->method);
  if (route->resource) free(route->resource);
  free(route);
}

void router_free(router_t* router) {
  if (!router) return;

  for (int i = 0; i < router->route_count; i++) {
    route_free(router->routes[i]);
  }

  if (router->routes) {
    free(router->routes);
  }
  free(router);
}

http_handler_t get_request_handler(router_t* router, http_request_t* request) {
  route_t* key = route_new(request->method, request->resource, NULL);

  route_t** match = (route_t**)bsearch(&key, router->routes, router->route_count, sizeof(route_t*), route_cmp);

  free(key);

  if (match) {
    return (*match)->handler;
  }

  return NULL;
}
