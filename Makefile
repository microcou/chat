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

hello_curses: src/hello_curses.c
	gcc -Wall -Wextra -I./vendor/ncurses/include src/hello_curses.c ./vendor/ncurses/lib/libncursesw.a -o hello_curses

clean:
	rm -f $(TARGETS)
