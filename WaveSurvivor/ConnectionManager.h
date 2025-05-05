#pragma once
#include "Definitions.h"

class ConnectionManager
{
public:
	ConnectionManager();
	void SendMessage(const char* message);
	void ReceiveMessage(const char* message);
	void StartReceivingMessages();
	std::map<int, NetPlayer>* GetPlayersPtr();
private:
	unsigned __int64 _socket;
	std::map<int, NetPlayer> players;

	NetPlayer ParseMessage(const char* message);
};

