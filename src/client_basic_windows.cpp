#include <iostream>
#include <thread>
#include <string>
#include <cstdlib>
#include <winsock2.h>
#include <ws2tcpip.h>

SOCKET socket_fd;

void receive_messages() {
    char buffer[1024];
    while (true) {
        int bytes = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            std::cout << "\nDisconnected from server" << std::endl;
            break;
        }
        buffer[bytes] = '\0';
        std::cout << "< " << buffer << std::endl;
        std::cout.flush();
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

    std::cout << "Connected to server at " << ip << ":" << port << std::endl;
    std::cout << "Type your messages (Ctrl+C to exit):" << std::endl;

    std::thread recv_thread(&receive_messages);
    recv_thread.detach();

    std::string message;
    while (std::getline(std::cin, message)) {
        if (!message.empty()) {
            send(socket_fd, message.c_str(), static_cast<int>(message.length()), 0);
        }
    }

    closesocket(socket_fd);
    WSACleanup();
    return 0;
}
