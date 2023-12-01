#pragma once
#include "Definitions.h"
#include "Player.h"
#include "Zombie.h"
#include "Tools.h"
#include "World.h"
#include "XPOrb.h"

class GameHandler
{
public:
	enum Difficulty
	{
		EASY,
		MEDIUM,
		HARD
	};

	GameHandler();
	~GameHandler();

	void Update();
	Position GetPlayerPosition();
	std::vector<Object> GetObjectsInViewport();
	std::vector<Zombie*> GetEnemiesInViewport();
	std::vector<Position_f> GetProjectilesInViewport();
	Position GetPlayerFireDestination();
	bool GetDebugMode();
	World* GetWorldPtr();
	bool GetGameOver();
	bool PlayerJustLeveledUp();
	bool PlayerJustOpenedChest();
	void SetPlayerJustLeveledUp(bool val);
	void SetPlayerJustOpenedChest(bool val);
	void HandleSelectedCard(Card* card);
	bool PlayerIsMoving();
	struct Node
	{
		quadtree::Box<float> box;
		std::size_t id;
		Character* c;
	};
private:
	void Initialize();
	void HandlePickup();
	void HandlePlayerMovement();
	bool CheckForCollision();
	bool CheckForCollision(Projectile* p);
	void HandlePlayer();
	void HandleEnemies();
	void HandleProjectiles();
	void HandleGeneralInput();
	bool ShouldEnemySpawn(long long currentMs);
	bool ShouldPlayerFire(long long currentMs);
	void SpawnEnemy();
	void MoveEnemies();
	void MoveProjectiles();
	Position GetNearestEnemyPosition();
	void UpdateZombiesKilled();

	bool debugMode;
	int enemyId;
	int projectileId;
	long long lastTimeEnemySpawned;
	long long lastTimePlayerFired;
	Difficulty currentDifficulty;
	World* world;
	Position playerFireDestination;
	bool gameOver;
	bool playerJustLeveledUp;
	bool playerJustOpenedChest;
	long long zombiesKilled = 0;
	bool playerIsMoving;
	//static quadtree::Box<float> GetBox(Node* node)
	//{
	//	return node->box;
	//}

	//quadtree::Quadtree<Node*, decltype(&GetBox)> quadTree;
	//size_t quadtreeId = 0;
};

