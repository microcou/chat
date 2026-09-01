CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pthread
TARGETS = server client
SERVER_SRC = server.cpp
CLIENT_SRC = client.cpp

all: $(TARGETS)

server: $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o server $(SERVER_SRC)

client: $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o client $(CLIENT_SRC)

clean:
	rm -f $(TARGETS)
