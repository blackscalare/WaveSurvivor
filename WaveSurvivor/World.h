#pragma once
#include "Definitions.h"
#include "Player.h"
#include "Zombie.h"
#include "Projectile.h"
#include "Object.h"
#include "XPOrb.h"
#include "Chest.h"

class World
{
public:
	World();
	~World();

	Position GetWorldSize();
	Player* GetPlayerPtr();
	std::map<int, NetPlayer>* GetOtherPlayerPtrs();
	void SetOtherPlayerPtrs(std::map<int, NetPlayer>* otherPlayers);
	std::map<int, Zombie*>* GetZombiesMapPtr();
	std::map<int, Projectile*>* GetProjectilesPtr();
	std::set<Object*>* GetObjectsPtr();
	void SpawnXpOrb(Position pos);
	void SpawnChest();

private:
	Position size;
	Player* player;
	std::map<int, NetPlayer>* otherPlayers;
	std::map<int, Zombie*>* zombies;
	std::set<Object*> objects;
	std::map<int, Projectile*>* projectiles;
};

