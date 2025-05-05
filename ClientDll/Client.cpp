#include "pch.h"
#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Client.h"
#pragma comment(lib, "Ws2_32.lib")

unsigned __int64 init(const char* server_ip, const int server_port)
{
    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Define server address
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_address.sin_addr);

    // Connect to server
    if (connect(sock, (sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server." << std::endl;

    // Start a thread to receive messages from the server
    //std::thread(receive_messages, sock).detach();

    
    return sock;
    // Cleanup
    /*closesocket(sock);
    WSACleanup();
    return 0;*/
}

void receive_messages(unsigned __int64 sock)
{
    char buffer[1024];
    int bytes_received;
    while ((bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Server: " << buffer << std::endl;
    }
}

void receive_messages_with_callback(unsigned __int64 sock, std::function<void(const char*)> callback)
{
    char buffer[1024];
    int bytes_received;
    while ((bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';

        // Invoke the callback with the received message
        callback(buffer);
    }
}

void send_message(unsigned __int64 sock, std::string message)
{
    send(sock, message.c_str(), message.size(), 0);
}