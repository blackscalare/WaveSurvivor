#include "ConnectionManager.h"
#include "Logger.h"

extern "C" unsigned __int64 __cdecl init(const char* server_ip, const int server_port);
extern "C" void __cdecl send_message(unsigned __int64 sock, std::string message);
extern "C" void __cdecl receive_messages_with_callback(unsigned __int64 sock, std::function<void(const char*)> callback);

ConnectionManager::ConnectionManager()
{
	_socket = init("127.0.0.1", 8080);
    Logger::Debug("Client socket", _socket);
}

void ConnectionManager::SendMessage(const char* message)
{
    //Logger::Debug("Sending message", message);
	send_message(_socket, message);
}

void ConnectionManager::StartReceivingMessages()
{
    unsigned __int64 sock = _socket;
    std::thread([this, sock]() {
        receive_messages_with_callback(sock, [this](const char* message) {
            this->ReceiveMessage(message);
            });
        }).detach();
}

std::map<int, NetPlayer>* ConnectionManager::GetPlayersPtr()
{
    return &players;
}

NetPlayer ConnectionManager::ParseMessage(const char* message)
{
    try {
        std::string str = message;
        str = str.substr(7, str.length());
        size_t delim = str.find(" ");
        int id = stoi(str.substr(0, delim));
        std::string pos = str.substr(delim + 1, str.length());
        size_t posDelim = pos.find(",");
        int x = stoi(pos.substr(0, posDelim));
        int y = stoi(pos.substr(posDelim + 1, pos.length()));
        
        return NetPlayer(id, x, y);
    }
    catch (std::exception e) {
        return NetPlayer();
    }
}

void ConnectionManager::ReceiveMessage(const char* message)
{
	Logger::Debug("Got message", message);
    NetPlayer netPlayer = ParseMessage(message);
    if (netPlayer.id != -1) {
        players[netPlayer.id] = netPlayer;
    }
}
