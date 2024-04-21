#include "Client.h"
#include <csignal>
#include <stdlib.h>

Client *client;

void signalHandler(int signum) {
  client->~Client();
  exit(signum);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <server_ip> <port>" << std::endl;
    return 1;
  }

  std::string serverIP = argv[1];
  int port = atoi(argv[2]);

  client = new Client(serverIP, port);

  signal(SIGINT, signalHandler);

  client->start();
  return 0;
}