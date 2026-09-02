#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct ClientInfo {
    int fd;
    std::string ip;
};

std::vector<ClientInfo> clients;
std::mutex clients_mutex;

void broadcast_message(const std::string& message, int sender_fd) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (const auto& client : clients) {
        if (client.fd != sender_fd) {
            send(client.fd, message.c_str(), message.length(), 0);
        }
    }
}

void handle_client(int client_fd, struct sockaddr_in client_addr) {
    char buffer[1024];
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, INET_ADDRSTRLEN);
    
    std::string ip = ip_str;
    
    std::string client_info = std::string("*** New client connected : ") + ip + " ***";
    broadcast_message(client_info, -1);

    while (true) {
        int bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            break;
        }
        buffer[bytes] = '\0';
        std::string message(buffer);
        broadcast_message(message, client_fd);
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        std::erase_if(clients, [client_fd](const ClientInfo& c) { return c.fd == client_fd; });
    }

    std::string disconnect_msg = std::string("*** Client disconnected : ") + ip + " ***";
    broadcast_message(disconnect_msg, client_fd);

    close(client_fd);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Error setting socket option" << std::endl;
        return 1;
    }

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    listen(server_fd, 5);
    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        struct sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        
        if (client_fd < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, INET_ADDRSTRLEN);

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            ClientInfo info;
            info.fd = client_fd;
            info.ip = ip_str;
            clients.push_back(info);
        }

        std::thread(&handle_client, client_fd, client_addr).detach();
    }

    close(server_fd);
    return 0;
}
