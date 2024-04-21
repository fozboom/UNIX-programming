#include "Server.h"
#include <filesystem>
#include <mutex>
#include <unistd.h>

Server::Server(int port) {
  serverDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (serverDescriptor == -1) {
    std::cerr << "Error creating socket" << std::endl;
    exit(EXIT_FAILURE);
  }

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  if (bind(serverDescriptor, (struct sockaddr *)&serverAddress,
           sizeof(serverAddress)) == -1) {
    std::cerr << "Error binding socket" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (listen(serverDescriptor, 5) == -1) {
    std::cerr << "Error listening on socket" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Server is ready." << std::endl;
}

void Server::start() {
  if (fcntl(serverDescriptor, F_SETFL, O_NONBLOCK) < 0) {
    std::cerr << "Error setting socket to non-blocking mode" << std::endl;
    exit(EXIT_FAILURE);
  }
  while (isRunning) {
    int client_descriptor = accept(serverDescriptor, NULL, NULL);
    if (client_descriptor == -1) {
      if (errno == EWOULDBLOCK || errno == EAGAIN) {
        usleep(1000);
        continue;
      } else {
        std::cerr << "Error accepting connection" << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    std::cout << getCurrentTime() << " Client connected" << std::endl;

    std::thread client_thread(
        [this](int client_descriptor) {
          this->handleClient(client_descriptor);
        },
        client_descriptor);
    clientThreads.push_back(std::move(client_thread));
  }
}

void Server::handleClient(int clientDescriptor) {
  bool isRunning = true;
  std::string message;
  while (isRunning) {
    char buffer[256];
    int bytesRead = read(clientDescriptor, buffer, 256);

    if (bytesRead == -1) {
      std::cerr << "Error reading from client" << std::endl;
      break;
    }
    message = std::string(buffer, bytesRead);
    parseMessage(message, clientDescriptor, &isRunning);
  }
}

void Server::parseMessage(std::string &message, int clientDescriptor,
                          bool *isRunning) {
  if (message.substr(0, 4) == "ECHO") {
    write(clientDescriptor, message.substr(5).c_str(),
          message.substr(5).size());
  } else if (message.substr(0, 4) == "QUIT") {
    std::cout << getCurrentTime() << " Client disconnected" << std::endl;
    *isRunning = false;
    close(clientDescriptor);
  } else if (message.substr(0, 4) == "INFO") {
    sendFileContents("./serverInfo.txt", clientDescriptor);
  } else if (message.substr(0, 2) == "CD") {
    changeDirectory(message.substr(3), clientDescriptor);
  } else if (message.substr(0, 4) == "LIST") {
    std::lock_guard<std::mutex> lock(mutex);
    std::string list = listDirectory(".", "");
    write(clientDescriptor, list.c_str(), list.size());
  } else {
    std::string error = "Invalid command";
    write(clientDescriptor, error.c_str(), error.size());
  }
}

std::string Server::listDirectory(const std::string &path,
                                  const std::string &prefix) {
  std::string result;
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_directory()) {
      result += prefix + entry.path().filename().string() + "/\n";
      result += listDirectory(entry.path().string(), prefix + "  ");
    } else if (entry.is_symlink()) {
      auto target = std::filesystem::read_symlink(entry.path());
      result += prefix + entry.path().filename().string() + " --> ";
      if (std::filesystem::is_regular_file(target)) {
        result += " " + target.filename().string() + "\n";
      } else if (std::filesystem::is_symlink(target)) {
        result += "> " + target.filename().string() + "\n";
      }
    } else if (entry.is_regular_file()) {
      result += prefix + entry.path().filename().string() + "\n";
    }
  }
  return result;
}

void Server::sendFileContents(const std::string &filePath,
                              int clientDescriptor) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    std::string error = "Error opening file";
    write(clientDescriptor, error.c_str(), error.size());
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    line += '\n';
    write(clientDescriptor, line.c_str(), line.size());
  }

  file.close();
}

void Server::changeDirectory(const std::string &path, int clientDescriptor) {
  if (path.empty()) {
    std::string error = "Invalid path";
    write(clientDescriptor, error.c_str(), error.size());
    return;
  }

  struct stat pathStat;
  if (stat(path.c_str(), &pathStat) != 0 || !S_ISDIR(pathStat.st_mode)) {
    std::string error = "Path is not a directory";
    write(clientDescriptor, error.c_str(), error.size());
    return;
  }

  if (chdir(path.c_str()) != 0) {
    std::string error = "Error changing directory";
    write(clientDescriptor, error.c_str(), error.size());
    return;
  }
  write(clientDescriptor, "Directory changed", 17);
}

std::string Server::getCurrentTime() const {
  auto now = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(now);
  char buffer[100];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S",
                std::localtime(&now_c));
  return std::string(buffer);
}

Server::~Server() {
  for (auto &thread : clientThreads) {
    if (thread.joinable()) {
      thread.join();
    }
  }
  close(serverDescriptor);
}