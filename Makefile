CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pthread
TARGETS = server client simple_client
SERVER_SRC = server.cpp
CLIENT_SRC = client.cpp
SIMPLE_CLIENT_SRC = simple_client.cpp

all: $(TARGETS)

server: $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o server $(SERVER_SRC)

client: $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o client $(CLIENT_SRC)

simple_client: $(SIMPLE_CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o simple_client $(SIMPLE_CLIENT_SRC)

clean:
	rm -f $(TARGETS)
