#ifndef SERVER_H
#define SERVER_H

struct Server {
  void (*listenAndServe)();
};

struct Server* server_new();

#endif
