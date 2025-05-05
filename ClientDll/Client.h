#pragma once
#include <functional>

#ifdef CLIENT_EXPORTS
#define CLIENT_API __declspec(dllexport)
#else
#define CLIENT_API __declspec(dllimport)
#endif

extern "C" CLIENT_API unsigned __int64 init(const char* server_ip, const int server_port);
extern "C" CLIENT_API void receive_messages(unsigned __int64 sock);
extern "C" CLIENT_API void send_message(unsigned __int64 sock, std::string message);
extern "C" CLIENT_API void receive_messages_with_callback(unsigned __int64 sock, std::function<void(const char*)> callback);