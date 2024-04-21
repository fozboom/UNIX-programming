#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Client {
  int clientDescriptor;
  struct sockaddr_in serverAddress;

public:
  Client(const std::string &serverIP, int serverPort);

  void start();
  std::string getCommandFromUser();
  void sendCommandToServer(const std::string &command);
  std::string getResponseFromServer();

  ~Client();
};