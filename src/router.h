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
  route_t* routes;
  void (*add_route)(const char* method, const char* resource, http_handler_t handler);
  void (*prepare)();
} routes_t;

typedef struct {
  routes_t* routes;

  void (*http_get)(const char* resource, http_handler_t handler);
  void (*http_patch)(const char* resource, http_handler_t handler);
  void (*http_post)(const char* resource, http_handler_t handler);
  void (*http_delete)(const char* resource, http_handler_t handler);

  void (*prepare)();
} router_t;

router_t* router_new();

#endif
