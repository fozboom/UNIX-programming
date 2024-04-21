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
  while (true) {
    std::string command = getCommandFromUser();
    sendCommandToServer(command);
    std::string response = getResponseFromServer();
    std::cout << response << std::endl;
  }
}

std::string Client::getCommandFromUser() {
  std::cout << "> ";
  std::string command;
  std::getline(std::cin, command);
  return command;
}

void Client::sendCommandToServer(const std::string &command) {
  uint32_t commandSize = htonl(command.size());
  if (write(clientDescriptor, &commandSize, sizeof(commandSize)) == -1) {
    std::cerr << "Error sending command size" << std::endl;
    close(clientDescriptor);
    exit(EXIT_FAILURE);
  }

  if (write(clientDescriptor, command.c_str(), command.size()) == -1) {
    std::cerr << "Error sending command" << std::endl;
    close(clientDescriptor);
    exit(EXIT_FAILURE);
  }
}

std::string Client::getResponseFromServer() {
  uint32_t responseSize;
  if (read(clientDescriptor, &responseSize, sizeof(responseSize)) == -1) {
    std::cerr << "Error receiving response size. Server may have closed."
              << std::endl;
    close(clientDescriptor);
    exit(EXIT_FAILURE);
  }
  responseSize = ntohl(responseSize);

  char *buffer = new char[responseSize + 1];
  ssize_t bytesRead = read(clientDescriptor, buffer, responseSize);
  if (bytesRead == -1) {
    std::cerr << "Error receiving response. Server may have closed."
              << std::endl;
    delete[] buffer;
    close(clientDescriptor);
    exit(EXIT_FAILURE);
  } else if (bytesRead == 0) {
    std::cerr << "Server closed connection." << std::endl;
    delete[] buffer;
    close(clientDescriptor);
    exit(EXIT_SUCCESS);
  }
  buffer[responseSize] = '\0';

  std::string response(buffer);
  delete[] buffer;
  return response;
}

Client::~Client() { close(clientDescriptor); }