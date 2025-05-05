#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

const int PORT = 8080;
const int MAX_CLIENTS = 10;
std::vector<SOCKET> clients;  // List of connected clients
std::mutex clients_mutex;     // Mutex to protect client list

#ifdef _WIN32
using socklen_t = int;
#endif

void broadcast_message(const std::string& message, SOCKET sock) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (SOCKET client_socket : clients) {
        if (sock != client_socket) {
            send(client_socket, message.c_str(), message.length(), 0);
        }
    }
}

void handle_client(SOCKET client_socket) {
    char buffer[1024] = { 0 };
    int bytes_received;

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytes_received] = '\0';
        std::string message = "Client " + std::to_string(client_socket) + ": " + buffer;
        std::cout << message << std::endl;
        broadcast_message(message, client_socket);
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
    }

#ifdef _WIN32
    closesocket(client_socket);
#else
    close(client_socket);
#endif
    std::cout << "Client " << client_socket << " disconnected." << std::endl;
}

int main() {
#ifdef _WIN32
    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }
#endif

    SOCKET server_fd;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
#ifdef _WIN32
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
#else
        perror("socket failed");
#endif
        exit(EXIT_FAILURE);
    }

#ifdef _WIN32
    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        std::cerr << "setsockopt failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }
#else
    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
#endif

    // Bind to the given port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
#ifdef _WIN32
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
#else
        perror("bind failed");
#endif
        exit(EXIT_FAILURE);
    }

    // Start listening
    if (listen(server_fd, MAX_CLIENTS) < 0) {
#ifdef _WIN32
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
#else
        perror("listen failed");
#endif
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on port " << PORT << std::endl;

    while (true) {
        SOCKET new_socket;
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
#ifdef _WIN32
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            continue;
#else
            perror("accept failed");
            continue;
#endif
        }

        std::cout << "New connection: " << new_socket << std::endl;

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(new_socket);
        }

        std::thread(handle_client, new_socket).detach();  // Handle each client in a new thread
    }

#ifdef _WIN32
    // Cleanup Winsock
    closesocket(server_fd);
    WSACleanup();
#else
    close(server_fd);
#endif

    return 0;
}
