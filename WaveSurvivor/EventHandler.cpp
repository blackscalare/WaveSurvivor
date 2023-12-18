#include "EventHandler.h"
#include "Logger.h"

EventHandler::EventHandler()
{
	eventParser = new EventParser();
}

void EventHandler::LoadEvents()
{
	std::vector<Event<EnemyType>> events = eventParser->ParseEvents();
	if (events.size() > 0) {
		this->events = events;
	}
	else {
		Logger::Debug("No events loaded");
	}
}

std::vector<Event<EnemyType>> EventHandler::GetEvents()
{
	return events;
}

void EventHandler::RemoveEvent(int idx)
{
	events.erase(events.begin() + idx);
}
