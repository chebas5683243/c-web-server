#ifndef ROUTER_H
#define ROUTER_H

typedef struct {

} http_request_t;

typedef struct {

} http_response_t;

typedef void (*http_handler_t)(http_request_t*, http_response_t*);

typedef struct {
  char* method;
  char* resource;
  http_handler_t handler;
} route_t;

typedef struct {
  route_t** routes;
  int route_count;
  int capacity;
} router_t;

router_t* router_new();
route_t* route_new(const char* method, const char* resource, http_handler_t handler);
void router_add_route(router_t* router, route_t* route);
void router_prepare();
void router_free(router_t*);

#endif
