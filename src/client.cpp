#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include <cstdio>
#include <mutex>
#include <unistd.h>
#include <termios.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int socket_fd;
std::string current_input;
std::mutex input_mutex;
termios original_tty;

void receive_messages() {
    char buffer[1024];
    while (true) {
        int bytes = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            std::cout << "\nDisconnected from server" << std::endl;
            break;
        }
        buffer[bytes] = '\0';
        
        {
            std::lock_guard<std::mutex> lock(input_mutex);
            
            // Erase the current line
            std::cout << "\033[2K\r";
            
            // Print the received message
            std::cout << buffer << std::endl;
            
            // Put back the prompt with the saved input
            std::cout << "Typing: " << current_input;
            std::cout.flush();
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
        return 1;
    }

    const char* ip = argv[1];
    int port = std::atoi(argv[2]);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        return 1;
    }

    // Disable buffering for stdout and stdin
    std::setbuf(stdout, nullptr);
    std::setbuf(stdin, nullptr);
    
    // Set terminal to non-canonical mode
    termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    original_tty = tty;  // Save original settings
    tty.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);

    std::cout << "Connected to server at " << ip << ":" << port << std::endl;
    std::cout << "Type your messages (Ctrl+C to exit):" << std::endl;

    std::thread recv_thread(&receive_messages);
    recv_thread.detach();

    std::cout << "> ";
    std::cout.flush();

    while (true) {
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == '\n') {
                {
                    std::lock_guard<std::mutex> lock(input_mutex);
                    if (!current_input.empty()) {
                        // Erase the current line
                        std::cout << "\033[2K\r";
                        std::cout.flush();
                        std::cout << current_input;

                        send(socket_fd, current_input.c_str(), current_input.length(), 0);
                        
                        current_input.clear();
                    }
                }
                std::cout << std::endl << "Typing: ";
                std::cout.flush();
            } else if (c == '\b' || c == 127) {
                std::lock_guard<std::mutex> lock(input_mutex);
                if (!current_input.empty()) {
                    current_input.pop_back();
                    std::cout << "\b \b";
                    std::cout.flush();
                }
            } else {
                {
                    std::lock_guard<std::mutex> lock(input_mutex);
                    current_input += c;
                }
                std::cout << c;
                std::cout.flush();
            }
        }
    }

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tty);
    
    close(socket_fd);
    return 0;
}
