#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstring>
#include <ctime>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
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
  std::atomic<bool> isRunning;
  std::mutex mutex;

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
  void stop() { isRunning = false; }
  std::string listDirectory(const std::string &path, const std::string &prefix);

  ~Server();
};
