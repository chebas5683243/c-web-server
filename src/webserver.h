#ifndef SERVER_H
#define SERVER_H

struct server_t {
  void (*listen_and_serve)();
};

struct server_t* server_new();

#endif
