#pragma once
#include "Definitions.h"
#include "Player.h"
#include "Zombie.h"
#include "Tools.h"
#include "World.h"
#include "XPOrb.h"
#include "TextureHandler.h"
#include "EventHandler.h"

class GameHandler
{
public:
	enum Difficulty
	{
		EASY,
		MEDIUM,
		HARD
	};

	GameHandler(TextureHandler* textureHandler, EventHandler* eventHandler);
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
	int GetZombiesKilled() const;
	long long GetStartTime() const;
	void ResetStartTime();
	void PauseGame();
	void UnpauseGame();
	long long GetElapsedTime() const;

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
	void HandleEvents();
	void SpawnEnemiesFromEvent(int num);
	void HandleTime();
	void SetState(GameState state);

	bool debugMode;
	int enemyId;
	int projectileId;
	long long lastTimeEnemySpawned;
	long long lastTimePlayerFired;
	Difficulty currentDifficulty;
	EventHandler* eventHandler;
	World* world;
	TextureHandler* textureHandler;
	Position playerFireDestination;
	bool gameOver;
	bool playerJustLeveledUp;
	bool playerJustOpenedChest;
	long long zombiesKilled = 0;
	bool playerIsMoving;
	long long startTime;
	long long elapsedTime;
	GameState currentState;
	//static quadtree::Box<float> GetBox(Node* node)
	//{
	//	return node->box;
	//}

	//quadtree::Quadtree<Node*, decltype(&GetBox)> quadTree;
	//size_t quadtreeId = 0;
};

