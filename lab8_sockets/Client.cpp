#include "Client.h"
#include <sys/types.h>

Client::Client(const std::string &serverIP, int serverPort) {
  clientDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (clientDescriptor == -1) {
    std::cerr << "Error creating socket" << std::endl;
    exit(EXIT_FAILURE);
  }

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(serverPort);
  if (inet_pton(AF_INET, serverIP.c_str(), &serverAddress.sin_addr) <= 0) {
    std::cerr << "Invalid server IP address" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (connect(clientDescriptor, (struct sockaddr *)&serverAddress,
              sizeof(serverAddress)) < 0) {
    std::cerr << "Error connecting to server" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Client::start() {
  char buffer[256];
  while (true) {
    std::cout << "> ";
    std::string command;
    std::getline(std::cin, command);

    if (write(clientDescriptor, command.c_str(), command.size()) == -1) {
      std::cerr << "Error sending command" << std::endl;
      close(clientDescriptor);
      exit(EXIT_FAILURE);
    }

    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = read(clientDescriptor, buffer, sizeof(buffer));
    if (bytesRead == -1) {
      std::cerr << "Error receiving response. Server may have closed."
                << std::endl;
      close(clientDescriptor);
      exit(EXIT_FAILURE);
    } else if (bytesRead == 0) {
      std::cerr << "Server closed connection." << std::endl;
      close(clientDescriptor);
      exit(EXIT_SUCCESS);
    }

    std::cout << buffer << std::endl;
  }
}

Client::~Client() { close(clientDescriptor); }