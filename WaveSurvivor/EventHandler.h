#pragma once
#include "Definitions.h"
#include "EventParser.h"

class EventHandler
{
public:
	EventHandler();
	void LoadEvents();
	std::vector<Event<EnemyType>> GetEvents();
	void RemoveEvent(int idx);
private:
	EventParser* eventParser;
	std::vector<Event<EnemyType>> events;
};

