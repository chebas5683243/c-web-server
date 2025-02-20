#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "format.h"
#include "router.h"
#include "webserver.h"

#define BACKLOG 50

typedef struct {
  router_t* router;
  int socket_fd;
} request_context_t;

void trace_pid() {
  pid_t pid = getpid();
  printf("Process ID: %d\n", pid);
}

int create_socket() {
  int server_fd;
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt failed");
    exit(EXIT_FAILURE);
  }

  return server_fd;
}

void setup_socket(int server_fd, int port) {
  struct sockaddr_in server_addr;

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);


  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    perror("Bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", port);
}

void set_max_requests(int server_fd) {
  if (listen(server_fd, BACKLOG)) {
    perror("Listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }
}

void* process_request(void* args) {
  char buffer[10000];
  request_context_t* ctx = (request_context_t* )args;

  print_section("Connection accepted");

  ssize_t count = recv(ctx->socket_fd, buffer, sizeof(buffer), 0);

  if(count == -1) {
    perror("Failed to read from socket");
    close(ctx->socket_fd);
    return NULL;
  }

  if (count == 0) {
    puts("Looks like connection has been closed.");
    close(ctx->socket_fd);
    return NULL;
  }

  printf("Count: %d\n", (int)count);

  print_separator();
  printf("%s", buffer);
  printf("%d", ctx->socket_fd);

  const char *response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 13\r\n"
    "\r\n"
    "Hello, World!";

  ctx->router->routes[0]->handler(NULL, NULL);

  send(ctx->socket_fd, response, strlen(response), 0);

  print_section("Connection closed");
  puts("\n");

  close(ctx->socket_fd);

  free(ctx);

  return NULL;
}

request_context_t* create_request_context(int sock_fd, router_t* router) {
  request_context_t* ctx = malloc(sizeof(request_context_t));
  if (!ctx) return NULL;

  ctx->router = router;
  ctx->socket_fd = sock_fd;
  return ctx;
}

void serve(router_t* router, int server_fd) {
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);

  while (1) {
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

    if (client_fd < 0) {
      perror("Accept failed");
      continue;
    }

    pthread_t thread;

    request_context_t* ctx = create_request_context(client_fd, router);

    if (pthread_create(&thread, NULL, process_request, ctx) < 0) {
      perror("Thread request creation failed");
      close(client_fd);
      free(ctx);
      exit(EXIT_FAILURE);
    }

    if (pthread_detach(thread)) {
      perror("Thread request detach failed");
      close(client_fd);
      exit(EXIT_FAILURE);
    }
  }
}

void server_run(server_t* server) {
  trace_pid();
  int server_fd = create_socket();

  setup_socket(server_fd, server->port);

  set_max_requests(server_fd);

  serve(server->router, server_fd);

  close(server_fd);
}

server_t* server_new(int port, router_t* router) {
  server_t* server = malloc(sizeof(server_t));
  if (!server) {
    return NULL;
  }
  server->port = port;
  server->router = router;
  return server;
}

void server_free(server_t* server) {
  if (!server) return;
  router_free(server->router);
  free(server);
}
