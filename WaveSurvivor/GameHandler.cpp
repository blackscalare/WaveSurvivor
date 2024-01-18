#include "GameHandler.h"
#include "Logger.h"

GameHandler::GameHandler(TextureHandler* textureHandler, EventHandler* eventHandler)
{
	Initialize();
	this->textureHandler = textureHandler;
	this->eventHandler = eventHandler;
}

GameHandler::~GameHandler()
{
}

void GameHandler::Update()
{
	switch (currentState) {
		case RUNNING:
			HandlePlayer();
			HandleEnemies();
			HandleProjectiles();
			HandleGeneralInput();
			HandleEvents();
			HandleTime();
			break;
		case PAUSED:
			// Modify start time so it seems like no time passes
			break;
		case PLAYER_DEAD:
			// Handle player dead state
			currentState = PAUSED;
			break;
		case WIN:
			// Handle win state
			break;
	}
	
}

Position GameHandler::GetPlayerPosition()
{
	return world->GetPlayerPtr()->GetPosition();
}

std::vector<Object> GameHandler::GetObjectsInViewport()
{
	std::set<Object*>* objectsPtr = world->GetObjectsPtr();
	Position playerPos = world->GetPlayerPtr()->GetPosition();
	int xStart = playerPos.x - GetScreenWidth() / 2 > 0 ? playerPos.x - GetScreenWidth() / 2 : 0;
	int xEnd = playerPos.x + GetScreenWidth() / 2 < world->GetWorldSize().x ? playerPos.x + GetScreenWidth() / 2 : world->GetWorldSize().x;
	int yStart = playerPos.y - GetScreenHeight() / 2 > 0 ? playerPos.y - GetScreenHeight() / 2 : 0;
	int yEnd = playerPos.y + GetScreenHeight() / 2 < world->GetWorldSize().y ? playerPos.y + GetScreenHeight() / 2 : world->GetWorldSize().y;

	Position playerPosXRange = { xStart, xEnd };
	Position playerPosYRange = { yStart, yEnd };

	std::vector<Object> objectsInViewport = {};

	for (Object* o : *objectsPtr) {
		Position pos = o->GetPosition();
		if ((pos.x >= playerPosXRange.x && pos.x <= playerPosXRange.y) &&
			(pos.y >= playerPosYRange.x && pos.y <= playerPosYRange.y)) {
			objectsInViewport.push_back(*o);
		}
	}

	return objectsInViewport;
}

std::vector<Zombie*> GameHandler::GetEnemiesInViewport()
{
	std::map<int, Zombie*>* zombiesPtr = world->GetZombiesMapPtr();
	Position playerPos = world->GetPlayerPtr()->GetPosition();
	int xStart = playerPos.x - GetScreenWidth() / 2 > 0 ? playerPos.x - GetScreenWidth() / 2 : 0;
	int xEnd = playerPos.x + GetScreenWidth() / 2 < world->GetWorldSize().x ? playerPos.x + GetScreenWidth() / 2 : world->GetWorldSize().x;
	int yStart = playerPos.y - GetScreenHeight() / 2 > 0 ? playerPos.y - GetScreenHeight() / 2 : 0;
	int yEnd = playerPos.y + GetScreenHeight() / 2 < world->GetWorldSize().y ? playerPos.y + GetScreenHeight() / 2 : world->GetWorldSize().y;

	Position playerPosXRange = { xStart, xEnd };
	Position playerPosYRange = { yStart, yEnd };

	std::vector<Zombie*> objectsInViewport = {};

	for (const auto& pair : *zombiesPtr) {
		Zombie* z = pair.second;
		Position pos = z->GetPosition();
		if ((pos.x >= playerPosXRange.x && pos.x <= playerPosXRange.y) &&
			(pos.y >= playerPosYRange.x && pos.y <= playerPosYRange.y)) {
			objectsInViewport.push_back(z);
		}
	}

	return objectsInViewport;
}

std::vector<Projectile*> GameHandler::GetProjectilesInViewport()
{
	std::map<int, Projectile*>* projectilesPtr = world->GetProjectilesPtr();
	Position playerPos = world->GetPlayerPtr()->GetPosition();
	int xStart = playerPos.x - GetScreenWidth() / 2 > 0 ? playerPos.x - GetScreenWidth() / 2 : 0;
	int xEnd = playerPos.x + GetScreenWidth() / 2 < world->GetWorldSize().x ? playerPos.x + GetScreenWidth() / 2 : world->GetWorldSize().x;
	int yStart = playerPos.y - GetScreenHeight() / 2 > 0 ? playerPos.y - GetScreenHeight() / 2 : 0;
	int yEnd = playerPos.y + GetScreenHeight() / 2 < world->GetWorldSize().y ? playerPos.y + GetScreenHeight() / 2 : world->GetWorldSize().y;

	Position playerPosXRange = { xStart, xEnd };
	Position playerPosYRange = { yStart, yEnd };

	std::vector<Projectile*> projectilesInViewport = {};

	for (const auto& pair : *projectilesPtr) {
		Projectile* p = pair.second;
		Position_f pos = p->GetPosition();
		if ((pos.x >= playerPosXRange.x && pos.x <= playerPosXRange.y) &&
			(pos.y >= playerPosYRange.x && pos.y <= playerPosYRange.y)) {
			projectilesInViewport.push_back(p);
		}
	}

	return projectilesInViewport;
}

void GameHandler::Initialize()
{
	enemyId = 0;
	if (world != nullptr) delete world;
	world = new World;
	lastTimeEnemySpawned = 0;
	currentDifficulty = EASY;
	playerFireDestination = { 0,0 };
	debugMode = false;
	gameOver = false;
	startTime = Tools::Time::GetCurrentEpocMs();
	
	Position playerStartPosition = {world->GetWorldSize().x / 2, world->GetWorldSize().y / 2};
	world->GetPlayerPtr()->SetPosition(playerStartPosition);

	world->GetObjectsPtr()->insert(new Chest({ world->GetWorldSize().x / 2 + 100, world->GetWorldSize().y / 2 }));
}

void GameHandler::HandlePickup()
{
	std::set<Object*>* objectsMap = world->GetObjectsPtr();
	std::vector<Object*> objectsToRemove;
	for (const auto& o : *objectsMap) {
		Vector2 oPos = o->GetPosition().ToVector();
		Vector2 pPos = GetPlayerPosition().ToVector();
		if(CheckCollisionCircles({ pPos.x, pPos.y }, world->GetPlayerPtr()->GetPickupRadius(), { oPos.x, oPos.y }, DEFAULT_XP_ORB_RADIUS)) {
			objectsToRemove.push_back(o);
		}
	}

	if (objectsToRemove.size() > 0) {
		for (auto& o : objectsToRemove) {
			switch (o->GetType()) {
			case NONE:
				break;
			case XP_ORB: {
				XPOrb* xpOrb = dynamic_cast<XPOrb*>(o);
				SetPlayerJustLeveledUp(world->GetPlayerPtr()->GainXp(xpOrb->GetXpValue()));
				switch (world->GetPlayerPtr()->GetLevel()) {
				case 3:
					currentDifficulty = MEDIUM;
					break;
				case 8:
					currentDifficulty = HARD;
					break;
				}
			}
			   break;
			case CHEST:
				playerJustOpenedChest = true;
				break;
			default:
				Logger::Log(Logger::ERROR, "Object not implemented!");
			}

			objectsMap->erase(o);
			delete o;
		}
	}
}

void GameHandler::HandlePlayerMovement()
{
	if (IsKeyDown(KEY_W)) {
		if (CheckForCollision()) return;
		world->GetPlayerPtr()->MovePlayer(Player::UP, 0);
		playerIsMoving = true;
	}

	if (IsKeyDown(KEY_A)) {
		if (CheckForCollision()) return;
		world->GetPlayerPtr()->MovePlayer(Player::LEFT, 0);
		playerIsMoving = true;
	}

	if (IsKeyDown(KEY_S)) {
		if (CheckForCollision()) return;
		world->GetPlayerPtr()->MovePlayer(Player::DOWN, world->GetWorldSize().y);
		playerIsMoving = true;
	}

	if (IsKeyDown(KEY_D)) {
		if (CheckForCollision()) return;
		world->GetPlayerPtr()->MovePlayer(Player::RIGHT, world->GetWorldSize().x);
		playerIsMoving = true;
	}

	if (IsKeyReleased(KEY_W) || IsKeyReleased(KEY_A) || IsKeyReleased(KEY_S) || IsKeyReleased(KEY_D)) {
		playerIsMoving = false;
	}
}

bool GameHandler::CheckForCollision()
{
	
	return false;
}

bool GameHandler::CheckForCollision(Projectile* p)
{
	bool hitSomething = false;
	std::vector<int> zombiesToRemove;
	for (const auto& pair : *world->GetZombiesMapPtr()) {
		Zombie* z = pair.second;

		if (z == nullptr) {
			continue;
		}
		
		Hitbox zHitbox = pair.second->GetHitbox();
		int hitboxStartX = (int)zHitbox.area.x;
		int hitboxEndX = (int)zHitbox.area.x + (int)zHitbox.area.width;
		int hitboxStartY = (int)zHitbox.area.y;
		int hitboxEndY = (int)zHitbox.area.y + (int)zHitbox.area.width;

		if (CheckCollisionRecs(zHitbox.area, { p->GetPosition().x, p->GetPosition().y, DEFAULT_PROJECTILE_WIDTH, DEFAULT_PROJECTILE_HEIGHT })) {
			hitSomething = true;
			if (z->TakeDamage(p->GetDamage())) {
				zombiesToRemove.push_back(pair.first);
				world->SpawnXpOrb(z->GetPosition());
				UpdateZombiesKilled();
			}
		}
	}

	if (zombiesToRemove.size() > 0) {
		for (const int& id : zombiesToRemove) {
			world->GetZombiesMapPtr()->erase(id);
		}
	}

	return hitSomething;
}

void GameHandler::HandlePlayer()
{
	long long currentMs = Tools::Time::GetCurrentEpocMs();
	Position nearestEnemyPos = GetNearestEnemyPosition();
	if (CheckCollisionRecs(
		{ GetPlayerPosition().ToVector().x, GetPlayerPosition().ToVector().y, DEFAULT_PLAYER_WIDTH, DEFAULT_PLAYER_HEIGHT },
		{ nearestEnemyPos.ToVector().x, nearestEnemyPos.ToVector().y, DEFAULT_ZOMBIE_WIDTH, DEFAULT_ZOMBIE_HEIGHT }
	)) {
		world->GetPlayerPtr()->TakeDamage(DEFAULT_ZOMBIE_DAMAGE);
		if (world->GetPlayerPtr()->GetHealth() <= 0) {
			gameOverCallback();
			Initialize();
		}
	}

	if (ShouldPlayerFire(currentMs)) {
		if (nearestEnemyPos.x != 0 && nearestEnemyPos.y != 0) {
			lastTimePlayerFired = currentMs;
			playerFireDestination = Tools::Shape::GetRectangleCenter(nearestEnemyPos.x, nearestEnemyPos.y, DEFAULT_ZOMBIE_WIDTH, DEFAULT_ZOMBIE_HEIGHT);

			Position_f playerPosF = Position_f(world->GetPlayerPtr()->GetPosition().x, world->GetPlayerPtr()->GetPosition().y);
			Position_f playerCenterPosF = Tools::Shape::GetRectangleCenter(playerPosF.x, playerPosF.y, DEFAULT_PLAYER_WIDTH, DEFAULT_PLAYER_HEIGHT);
			Position_f nearestEnemyPosF = Position_f(playerFireDestination.x, playerFireDestination.y);
			Projectile* projectile = new Projectile(playerCenterPosF, nearestEnemyPosF);
			world->GetProjectilesPtr()->emplace(projectileId++, projectile);
		}
	}

	HandlePickup();
	HandlePlayerMovement();
}

void GameHandler::HandleEnemies()
{
	long long currentMs = Tools::Time::GetCurrentEpocMs();

	if (ShouldEnemySpawn(currentMs)) {
		lastTimeEnemySpawned = currentMs;
		SpawnEnemy();
	}
	
	MoveEnemies();
}

void GameHandler::HandleProjectiles()
{
	MoveProjectiles();
}

void GameHandler::HandleGeneralInput()
{
	if (IsKeyPressed(KEY_DELETE)) {
		debugMode = !debugMode;
	}
}

bool GameHandler::ShouldEnemySpawn(long long currentMs)
{
	if (lastTimeEnemySpawned == 0) {
		lastTimeEnemySpawned = currentMs;
		return false;
	}

	long long msPassed = currentMs - lastTimeEnemySpawned;

	switch (currentDifficulty) {
	case EASY:
		return msPassed >= 3000;
	case MEDIUM:
		return msPassed >= 2000;
	case HARD:
		return msPassed >= 1000;
	default:
		Logger::Log(Logger::ERROR, "Difficulty not implemented");
		return false;
	}

}

bool GameHandler::ShouldPlayerFire(long long currentMs)
{
	// Are there any enemies to fire at?
	// Get closest enemy within a certain range and fire projectile at that enemy
	long long msPassed = currentMs - lastTimePlayerFired;

	// TODO: Change so that the fire rate is based on a player stat
	return msPassed >= world->GetPlayerPtr()->GetFireSpeed();
}

void GameHandler::SpawnEnemy()
{
	// Spawn enemy somewhere out of view from the character, not too far from the player as to the enemy never being found or spawn too far off
	Zombie* zombie = new Zombie(world->GetPlayerPtr()->GetPosition(), MAX_DISTANCE_FROM_PLAYER);
	world->GetZombiesMapPtr()->emplace(enemyId++, zombie);
	//Node* zombieNode = new Node;

	//zombieNode->box.left = zombie->GetPosition().x;
	//zombieNode->box.top = zombie->GetPosition().y;
	//zombieNode->box.width = DEFAULT_ZOMBIE_WIDTH;
	//zombieNode->box.height = DEFAULT_ZOMBIE_HEIGHT;
	//zombieNode->id = quadtreeId++;
	//zombieNode->c = zombie;
	//quadTree.add(zombieNode);
}

// TODO: move similar to how projectiles are moved
void GameHandler::MoveEnemies()
{
	std::map<int, Zombie*>* zombiesMap = world->GetZombiesMapPtr();
	
	for (const auto& pair : *zombiesMap) {
		Zombie* z = pair.second;
		
		if (z == nullptr) {
			continue;
		}
		
		Position playerPos = world->GetPlayerPtr()->GetPosition();
		Position zombiePos = z->GetPosition();

		int x = zombiePos.x;
		int y = zombiePos.y;

		if (zombiePos.x > playerPos.x) {
			x -= z->GetMoveSpeed();
		}
		if (zombiePos.x < playerPos.x) { 
			x += z->GetMoveSpeed(); 
		}

		if (zombiePos.y > playerPos.y) {
			y -= z->GetMoveSpeed();
		}
		if (zombiePos.y < playerPos.y) {
			y += z->GetMoveSpeed();
		}

		Position newZombiePos = Position(x, y);
		z->SetPosition(newZombiePos);
		z->UpdateHitbox();
	}
}

void GameHandler::MoveProjectiles()
{
	std::map<int, Projectile*>* projectilesPtr = world->GetProjectilesPtr();
	std::vector<int> projectilesToRemove;

	for (const auto& pair : *projectilesPtr) {
		Projectile* p = pair.second;
		Position_f currentPos = p->GetPosition();
		Position_f dest = p->GetDestination();
		if (p->HasReachedDestination() || CheckForCollision(p)) {
			projectilesToRemove.push_back(pair.first);
			continue;
		}

		float angle = atan2(dest.y - currentPos.y, dest.x - currentPos.x);
		float dx = cos(angle) * p->GetSpeed();
		float dy = sin(angle) * p->GetSpeed();

		Position_f newProjectilePos = Position_f(currentPos.x + dx, currentPos.y + dy);
		p->SetPosition(newProjectilePos);
	}
	
	for (const int& id : projectilesToRemove) {
		projectilesPtr->erase(id);
	}
}

Position GameHandler::GetNearestEnemyPosition()
{
	Position nearestEnemyPos = {0, 0};
	int minDistance = INT_MAX;

	std::map<int, Zombie*>* zombiesMap = world->GetZombiesMapPtr();
	Position playerPos = world->GetPlayerPtr()->GetPosition();

	for (const auto& pair : *zombiesMap) {
		Zombie* z = pair.second;

		Vector2 direction = Vector2Normalize(Vector2Subtract(z->GetPosition().ToVector(), playerPos.ToVector()));
		int distance = Vector2Distance(playerPos.ToVector(), z->GetPosition().ToVector());
		if (distance < minDistance) {
			minDistance = distance;
			nearestEnemyPos = z->GetPosition();
		}
	}

	return nearestEnemyPos;
}

void GameHandler::UpdateZombiesKilled()
{
	if (++zombiesKilled % DEFAULT_CHEST_SPAWNRATE == 0) {
		world->SpawnChest();
	}

}

void GameHandler::HandleEvents()
{
	int seconds = static_cast<int>((elapsedTime % 60000) / 1000); // 1 second = 1000 milliseconds

	std::vector<Event<EnemyType>> events = eventHandler->GetEvents();

	std::vector<int> eventsToRemove;

	for (int i = 0; i < events.size(); ++i) {
		Event<EnemyType> e = events[i];

		if (seconds > e.time) {
			// Do event and add it for removal
			Logger::Debug("Executing event", e.id, e.name);
			for (auto& item : e.enemies) {
				EnemyType et = item.first;
				std::vector<int> nums = item.second;
				for (int num : nums) {
					switch (et) {
					case ENEMY_ZOMBIE: {
						// TODO: Is this safe? Prevents lock when spawning lots of enemies
						// causes issue on game over
						std::thread thread(&GameHandler::SpawnEnemiesFromEvent, this, num);
						thread.detach();
						break;
					}
					default:
						Logger::Log(Logger::ERROR, "Enemy type in event does not exist");
					}
				}
			}
			eventsToRemove.push_back(i);
		}
	}

	if (eventsToRemove.size() > 0) {
		for (int idx : eventsToRemove)
			eventHandler->RemoveEvent(idx);
	}
}

void GameHandler::SpawnEnemiesFromEvent(int num)
{
	// TODO this keeps spawning since world is recreated while this thread is still running
	for (int i = 0; i < num; ++i) {
		SpawnEnemy();
	}
}

void GameHandler::HandleTime()
{
	elapsedTime = Tools::Time::GetCurrentEpocMs() - startTime;
}

void GameHandler::PauseGame()
{
	currentState = PAUSED;
}

void GameHandler::UnpauseGame()
{
	startTime = Tools::Time::GetCurrentEpocMs() - elapsedTime;
	currentState = RUNNING;
}

long long GameHandler::GetElapsedTime() const
{
	return elapsedTime;
}

GameState GameHandler::GetState()
{
	return currentState;
}

void GameHandler::SetState(GameState state)
{
	currentState = state;
}

long long GameHandler::GetStartTime() const
{
	return startTime;
}

void GameHandler::ResetStartTime()
{
	startTime = Tools::Time::GetCurrentEpocMs();
}

Position GameHandler::GetPlayerFireDestination()
{
	return playerFireDestination;
}

bool GameHandler::GetDebugMode()
{
	return debugMode;
}

World* GameHandler::GetWorldPtr()
{
	return world;
}

bool GameHandler::GetGameOver()
{
	return gameOver;
}

bool GameHandler::PlayerJustLeveledUp()
{
	return playerJustLeveledUp;
}

bool GameHandler::PlayerJustOpenedChest()
{
	return playerJustOpenedChest;
}

void GameHandler::SetPlayerJustLeveledUp(bool val)
{
	playerJustLeveledUp = val;
	if (val) {
		currentState = PAUSED;
	}
}

void GameHandler::SetPlayerJustOpenedChest(bool val)
{
	playerJustOpenedChest = val;
}

void GameHandler::HandleSelectedCard(Card* card)
{
	switch (card->type) {
	case SPEED:
		world->GetPlayerPtr()->SetMoveSpeed(world->GetPlayerPtr()->GetMoveSpeed() * 1.05f);
		break;
	case DAMAGE:
		break;
	case ATTACK_SPEED:
		world->GetPlayerPtr()->SetFireSpeed(world->GetPlayerPtr()->GetFireSpeed() * 0.95f);
		break;
	case HEALTH:
		world->GetPlayerPtr()->SetHealth(world->GetPlayerPtr()->GetHealth() * 1.05f);
		break;
	case PICKUP:
		world->GetPlayerPtr()->SetPickupRadius(world->GetPlayerPtr()->GetPickupRadius() * 1.05f);
		break;
	case THORN_AURA:
		world->GetPlayerPtr()->AddOrUpgradeSpell(THORN_AURA);
		break;
	default:
		Logger::Log(Logger::ERROR, "Card type not implemented!", __FILE__, __LINE__);
	}
}

bool GameHandler::PlayerIsMoving()
{
	return playerIsMoving;
}

int GameHandler::GetZombiesKilled() const
{
	return zombiesKilled;
}
