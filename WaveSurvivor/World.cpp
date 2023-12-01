#include "World.h"
#include "Tools.h"
#include "Logger.h"

World::World()
{
	size = { WORLD_WIDTH, WORLD_HEIGHT};
	player = new Player;
	player->SetPosition({ size.x / 2, size.y / 2 });
	zombies = new std::map<int, Zombie*>;
	projectiles = new std::map<int, Projectile*>;
}

World::~World()
{
	if (player != nullptr) {
		delete player;
	}

	if (zombies != nullptr) {
		delete zombies;
	}

	if (projectiles != nullptr) {
		delete projectiles;
	}
}

Position World::GetWorldSize()
{
	return size;
}

Player* World::GetPlayerPtr()
{
	return player;
}

std::map<int, Zombie*>* World::GetZombiesMapPtr()
{
	return zombies;
}

std::set<Object*>* World::GetObjectsPtr()
{
	return &objects;
}

void World::SpawnXpOrb(Position pos)
{
	objects.insert(new XPOrb(pos));
}

void World::SpawnChest()
{
	Position pos;
	pos = Tools::Random::GenerateRandomPosition(0, size.x, 0, size.y);
	objects.insert(new Chest(pos));
	Logger::Debug("Spawned chest at ", pos.x, pos.y);
}

std::map<int, Projectile*>* World::GetProjectilesPtr()
{
	return projectiles;
}
