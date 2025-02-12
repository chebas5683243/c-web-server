#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 42069
#define BACKLOG 50

int createSocket() {
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

struct sockaddr_in defineServerAddress(){
  struct sockaddr_in server_addr;

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  return server_addr;
}

void tracePid() {
  pid_t pid = getpid();
  printf("Process ID: %d\n", pid);
}

void bindSocket(int server_fd, struct sockaddr_in* server_addr_pt) {
  if (bind(server_fd, (struct sockaddr *) server_addr_pt, sizeof(*server_addr_pt)) < 0) {
    perror("Bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", PORT);
}

void setMaxRequests(int server_fd) {
  if (listen(server_fd, BACKLOG)) {
    perror("Listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }
}

void processRequest(int client_fd) {
  static char buffer[10000];

  puts("-----------------------------------\nConnection accepted\n-----------------------------------\n");

  ssize_t count = recv(client_fd, buffer, sizeof(buffer), 0);

  if(count == -1) {
    perror("Failed to read from socket");
    close(client_fd);
    return;
  }

  if (count == 0) {
    puts("Looks like connection has been closed.");
    close(client_fd);
    return;
  }

  printf("Count: %d\n", (int)count);

  puts("-----------------------------------");
  printf("%s", buffer);

  const char *response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 13\r\n"
    "\r\n"
    "Hello, World!";

  send(client_fd, response, strlen(response), 0);

  sleep(4);
  send(client_fd, response, strlen(response), 0);

  puts("-----------------------------------\nConnection closed\n-----------------------------------\n\n\n");
  close(client_fd);
}

void serve(int server_fd) {
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);

  while (1) {
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

    if (client_fd < 0) {
      perror("Accept failed");
      close(server_fd);
      exit(EXIT_FAILURE);
    }

    processRequest(client_fd);
  }
}

void listenAndServe() {
  tracePid();
  int server_fd = createSocket();
  struct sockaddr_in server_addr = defineServerAddress();

  bindSocket(server_fd, &server_addr);
  setMaxRequests(server_fd);

  serve(server_fd);

  close(server_fd);
}
