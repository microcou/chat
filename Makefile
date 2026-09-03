CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pthread
# Additional compiler flags for static linking of the standard C++ library and GCC runtime, costs about 1.2 MB per binary.
CXXFLAGS += -static-libstdc++ -static-libgcc
TARGETS = server_linux client_linux client_linux_basic

all: $(TARGETS)

server_linux: src/server_linux.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

client_linux: src/client_linux.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

client_linux_basic: src/client_linux_basic.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)
