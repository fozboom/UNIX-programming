CXX = g++
CXXFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c++17

all: server client

server: server_main.o Server.o
	$(CXX) $(CXXFLAGS) -o server server_main.o Server.o

client: client_main.o Client.o
	$(CXX) $(CXXFLAGS) -o client client_main.o Client.o

server_main.o: server_main.cpp Server.h
	$(CXX) $(CXXFLAGS) -c server_main.cpp

client_main.o: client_main.cpp Client.h
	$(CXX) $(CXXFLAGS) -c client_main.cpp

Server.o: Server.cpp Server.h
	$(CXX) $(CXXFLAGS) -c Server.cpp

Client.o: Client.cpp Client.h
	$(CXX) $(CXXFLAGS) -c Client.cpp

clean:
	rm -f *.o server client