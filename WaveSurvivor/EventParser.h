#pragma once
#include "Definitions.h"
#include "Zombie.h"

class EventParser
{
public:
	EventParser();

	std::vector<std::string> FindEventFileNames();

	std::vector<Event<EnemyType>> ParseEvents();
	std::map<EnemyType, std::vector<int>> ParseSpawnStr(std::string& spawnStr);
	EnemyType ConvertStringToEnemyType(std::string enemyStr);
	int eventId = 0;
};

