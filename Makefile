CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pthread
TARGETS = server client client_basic

all: $(TARGETS)

server: src/server.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

client: src/client.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

client_basic: src/client_basic.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)
