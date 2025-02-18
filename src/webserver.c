#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "format.h"
#include "webserver.h"

#define PORT 42069
#define BACKLOG 50

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

struct sockaddr_in define_server_address(){
  struct sockaddr_in server_addr;

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  return server_addr;
}

void trace_pid() {
  pid_t pid = getpid();
  printf("Process ID: %d\n", pid);
}

void bind_socket(int server_fd, struct sockaddr_in* server_addr_pt) {
  if (bind(server_fd, (struct sockaddr *) server_addr_pt, sizeof(*server_addr_pt)) < 0) {
    perror("Bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", PORT);
}

void set_max_requests(int server_fd) {
  if (listen(server_fd, BACKLOG)) {
    perror("Listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }
}

void* process_request(void* args) {
  static char buffer[10000];
  int client_fd = (intptr_t)args;

  print_section("Connection accepted");

  ssize_t count = recv(client_fd, buffer, sizeof(buffer), 0);

  if(count == -1) {
    perror("Failed to read from socket");
    close(client_fd);
    return NULL;
  }

  if (count == 0) {
    puts("Looks like connection has been closed.");
    close(client_fd);
    return NULL;
  }

  printf("Count: %d\n", (int)count);

  print_separator();
  printf("%s", buffer);
  printf("%d", client_fd);

  const char *response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 13\r\n"
    "\r\n"
    "Hello, World!";

  send(client_fd, response, strlen(response), 0);

  print_section("Connection closed");
  puts("\n");

  close(client_fd);

  return NULL;
}

void serve(int server_fd) {
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);

  while (1) {
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

    if (client_fd < 0) {
      perror("Accept failed");
    }

    pthread_t thread;

    if (pthread_create(&thread, NULL, process_request, (void *)(intptr_t)client_fd) < 0) {
      perror("Thread request creation failed");
      exit(EXIT_FAILURE);
      close(client_fd);
    }

    if (pthread_detach(thread)) {
      perror("Thread request detach failed");
      exit(EXIT_FAILURE);
      close(client_fd);
    }
  }
}

void listen_and_serve() {
  trace_pid();
  int server_fd = create_socket();
  struct sockaddr_in server_addr = define_server_address();

  bind_socket(server_fd, &server_addr);
  set_max_requests(server_fd);

  serve(server_fd);

  close(server_fd);
}

struct server_t* server_new() {
  struct server_t* server = malloc(sizeof(struct server_t));
  if (!server) {
    return NULL;
  }
  server->listen_and_serve = listen_and_serve;
  return server;
}
