#include <algorithm>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <vector>

class Server {
  int serverDescriptor;
  struct sockaddr_in serverAddress;
  std::vector<std::thread> clientThreads;

public:
  Server(int port);

  void start();
  void handleClient(int clientDescriptor);
  void parseMessage(std::string &message, int clientDescriptor,
                    bool *isRunning);
  void sendFileContents(const std::string &filePath, int clientDescriptor);
  void changeDirectory(const std::string &path, int clientDescriptor);
  int getServerDescriptor() const { return serverDescriptor; }
  std::string getCurrentTime() const;

  ~Server();
};
