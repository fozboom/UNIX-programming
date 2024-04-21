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
  while (isRunning) {
    std::string message = readFromSocket(clientDescriptor);
    if (message.empty()) {
      break;
    }

    parseMessage(message, clientDescriptor, &isRunning);
  }
}

void Server::parseMessage(std::string &message, int clientDescriptor,
                          bool *isRunning) {
  std::string response;
  if (message.substr(0, 4) == "ECHO") {
    response = message.substr(5);
  } else if (message.substr(0, 4) == "QUIT") {
    std::cout << getCurrentTime() << " Client disconnected" << std::endl;
    *isRunning = false;
    close(clientDescriptor);
    return;
  } else if (message.substr(0, 4) == "INFO") {
    response = getFileContents("./serverInfo.txt");
  } else if (message.substr(0, 2) == "CD") {
    changeDirectory(message.substr(3), clientDescriptor);
    return;
  } else if (message.substr(0, 4) == "LIST") {
    std::lock_guard<std::mutex> lock(mutex);
    response = listDirectory(".", "");
  } else {
    response = "Invalid command";
  }

  writeToSocket(clientDescriptor, response);
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

std::string Server::getFileContents(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    return "Error opening file";
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();
  return buffer.str();
}

void Server::changeDirectory(const std::string &path, int clientDescriptor) {
  std::string response;
  if (path.empty()) {
    response = "Invalid path";
  } else {
    std::filesystem::path fsPath(path);
    if (!std::filesystem::exists(fsPath) ||
        !std::filesystem::is_directory(fsPath)) {
      response = "Path is not a directory";
    } else {
      std::error_code ec;
      std::filesystem::current_path(fsPath, ec);
      if (ec) {
        response = "Error changing directory";
      } else {
        response = "Directory changed";
      }
    }
  }

  // Send the size of the response first
  uint32_t responseSize =
      htonl(response.size()); // Convert to network byte order
  if (write(clientDescriptor, &responseSize, sizeof(responseSize)) == -1) {
    std::cerr << "Error sending response size" << std::endl;
    return;
  }

  // Then send the response itself
  if (write(clientDescriptor, response.c_str(), response.size()) == -1) {
    std::cerr << "Error sending response" << std::endl;
  }
}

std::string Server::getCurrentTime() const {
  auto now = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(now);
  char buffer[100];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S",
                std::localtime(&now_c));
  return std::string(buffer);
}

void Server::writeToSocket(int clientDescriptor, const std::string &message) {
  uint32_t responseSize = htonl(message.size());
  if (write(clientDescriptor, &responseSize, sizeof(responseSize)) == -1) {
    std::cerr << "Error sending response size" << std::endl;
    return;
  }

  if (write(clientDescriptor, message.c_str(), message.size()) == -1) {
    std::cerr << "Error sending response" << std::endl;
  }
}

std::string Server::readFromSocket(int clientDescriptor) {
  uint32_t commandSize;
  if (read(clientDescriptor, &commandSize, sizeof(commandSize)) == -1) {
    std::cerr << "Error reading command size from client" << std::endl;
    return "";
  }
  commandSize = ntohl(commandSize); // Convert from network byte order

  char *buffer = new char[commandSize + 1];
  int bytesRead = read(clientDescriptor, buffer, commandSize);
  if (bytesRead == -1) {
    std::cerr << "Error reading command from client" << std::endl;
    delete[] buffer;
    return "";
  }
  buffer[commandSize] = '\0'; // Null-terminate the string

  std::string message(buffer, commandSize);
  delete[] buffer;

  return message;
}

Server::~Server() {
  for (auto &thread : clientThreads) {
    if (thread.joinable()) {
      thread.join();
    }
  }
  close(serverDescriptor);
}