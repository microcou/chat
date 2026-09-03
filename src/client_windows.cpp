#include <iostream>
#include <thread>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>
#include <windows.h>

SOCKET socket_fd;
std::string current_input;
std::mutex input_mutex;

HANDLE input_handle;
DWORD original_input_mode;
HANDLE output_handle;
DWORD original_output_mode;

void restore_console() {
    SetConsoleMode(input_handle, original_input_mode);
    SetConsoleMode(output_handle, original_output_mode);
}

bool configure_console() {
    input_handle = GetStdHandle(STD_INPUT_HANDLE);
    output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!GetConsoleMode(input_handle, &original_input_mode) ||
        !GetConsoleMode(output_handle, &original_output_mode)) {
        return false;
    }

    DWORD input_mode = original_input_mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    DWORD output_mode = original_output_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(input_handle, input_mode) ||
        !SetConsoleMode(output_handle, output_mode)) {
        restore_console();
        return false;
    }

    std::setbuf(stdout, nullptr);
    std::setbuf(stdin, nullptr);
    return true;
}

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
            std::cout << "\033[2K\r";
            std::cout << buffer << std::endl;
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

    WSADATA wsa_data{};
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        std::cerr << "Error initializing Winsock" << std::endl;
        return 1;
    }

    const char* ip = argv[1];
    int port = std::atoi(argv[2]);

    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(static_cast<u_short>(port));
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) != 1) {
        std::cerr << "Error parsing server IP address" << std::endl;
        closesocket(socket_fd);
        WSACleanup();
        return 1;
    }

    if (connect(socket_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Error connecting to server" << std::endl;
        closesocket(socket_fd);
        WSACleanup();
        return 1;
    }

    if (!configure_console()) {
        std::cerr << "Error configuring console" << std::endl;
        closesocket(socket_fd);
        WSACleanup();
        return 1;
    }
    std::atexit(restore_console);

    std::cout << "Connected to server at " << ip << ":" << port << std::endl;
    std::cout << "Type your messages (Ctrl+C to exit):" << std::endl;

    std::thread recv_thread(&receive_messages);
    recv_thread.detach();

    std::cout << "> ";
    std::cout.flush();

    while (true) {
        int key = _getch();
        if (key == 3) {
            break;
        }

        if (key == 0 || key == 224) {
            _getch();
            continue;
        }

        if (key == '\r') {
            {
                std::lock_guard<std::mutex> lock(input_mutex);
                if (!current_input.empty()) {
                    std::cout << "\033[2K\r";
                    std::cout << current_input;
                    send(socket_fd, current_input.c_str(), static_cast<int>(current_input.length()), 0);
                    current_input.clear();
                }
            }
            std::cout << std::endl << "Typing: ";
            std::cout.flush();
        } else if (key == '\b') {
            std::lock_guard<std::mutex> lock(input_mutex);
            if (!current_input.empty()) {
                current_input.pop_back();
                std::cout << "\b \b";
                std::cout.flush();
            }
        } else if (key >= 32 && key <= 126) {
            {
                std::lock_guard<std::mutex> lock(input_mutex);
                current_input += static_cast<char>(key);
            }
            std::cout << static_cast<char>(key);
            std::cout.flush();
        }
    }

    restore_console();
    closesocket(socket_fd);
    WSACleanup();
    return 0;
}
