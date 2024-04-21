#include "Server.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Server started on port 8080" << std::endl;
    std::cout << "If you want to start server on different port, run: "
                 "./server <port_number>"
              << std::endl;
  }
  int port = (argc == 2) ? std::stoi(argv[1]) : 8080;
  Server server(port);

  int opt = 1;
  if (setsockopt(server.getServerDescriptor(), SOL_SOCKET, SO_REUSEADDR, &opt,
                 sizeof(opt))) {
    std::cerr << "Error setting socket options" << std::endl;
    exit(EXIT_FAILURE);
  }

  server.start();
  return 0;
}