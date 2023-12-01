#include "GameHandler.h"
#include "Logger.h"

GameHandler::GameHandler()
{
	Initialize();
}

GameHandler::~GameHandler()
{
}

void GameHandler::Update()
{
	HandlePlayer();
	HandleEnemies();
	HandleProjectiles();
	HandleGeneralInput();
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

std::vector<Position_f> GameHandler::GetProjectilesInViewport()
{
	std::map<int, Projectile*>* projectilesPtr = world->GetProjectilesPtr();
	Position playerPos = world->GetPlayerPtr()->GetPosition();
	int xStart = playerPos.x - GetScreenWidth() / 2 > 0 ? playerPos.x - GetScreenWidth() / 2 : 0;
	int xEnd = playerPos.x + GetScreenWidth() / 2 < world->GetWorldSize().x ? playerPos.x + GetScreenWidth() / 2 : world->GetWorldSize().x;
	int yStart = playerPos.y - GetScreenHeight() / 2 > 0 ? playerPos.y - GetScreenHeight() / 2 : 0;
	int yEnd = playerPos.y + GetScreenHeight() / 2 < world->GetWorldSize().y ? playerPos.y + GetScreenHeight() / 2 : world->GetWorldSize().y;

	Position playerPosXRange = { xStart, xEnd };
	Position playerPosYRange = { yStart, yEnd };

	std::vector<Position_f> projectilesInViewport = {};

	for (const auto& pair : *projectilesPtr) {
		Projectile p = *pair.second;
		Position_f pos = p.GetPosition();
		if ((pos.x >= playerPosXRange.x && pos.x <= playerPosXRange.y) &&
			(pos.y >= playerPosYRange.x && pos.y <= playerPosYRange.y)) {
			projectilesInViewport.push_back(pos);
		}
	}

	return projectilesInViewport;
}

void GameHandler::Initialize()
{
	enemyId = 0;
	world = new World;
	lastTimeEnemySpawned = 0;
	currentDifficulty = EASY;
	playerFireDestination = { 0,0 };
	debugMode = false;
	gameOver = false;
	
	Position playerStartPosition = {world->GetWorldSize().x / 2, world->GetWorldSize().y / 2};
	world->GetPlayerPtr()->SetPosition(playerStartPosition);

	world->GetObjectsPtr()->insert(new Chest({ world->GetWorldSize().x / 2 + 100, world->GetWorldSize().y / 2 }));
	//auto box = quadtree::Box(0.0f, 0.0f, (float)world->GetWorldSize().x, (float)world->GetWorldSize().y);
	//auto quadTree = quadtree::Quadtree<Node*, decltype(getBox)>(box, getBox);
	//quadTree = quadtree::Quadtree<Node*, decltype(&GetBox)>(box, &GetBox);
	//Node* playerNode = new Node;

	//playerNode->box.left = playerStartPosition.x;
	//playerNode->box.top = playerStartPosition.y;
	//playerNode->box.width = DEFAULT_PLAYER_WIDTH;
	//playerNode->box.height = DEFAULT_PLAYER_HEIGHT;
	//playerNode->id = quadtreeId++;
	//playerNode->c = world->GetPlayerPtr();

	//quadTree.add(playerNode);
	
	/*otherNode.box.left = playerStartPosition.x + 10;
	otherNode.box.top = playerStartPosition.y;
	otherNode.box.width = DEFAULT_ZOMBIE_WIDTH;
	otherNode.box.height = DEFAULT_ZOMBIE_HEIGHT;
	otherNode.id = quadtreeId++;

	othernode2.box.left = playerStartPosition.x;
	othernode2.box.top = playerStartPosition.y;
	othernode2.box.width = DEFAULT_ZOMBIE_WIDTH;
	othernode2.box.height = DEFAULT_ZOMBIE_HEIGHT;
	othernode2.id = quadtreeId++;
	
	quadTree.add(&playerNode);
	quadTree.add(&otherNode);
	quadTree.add(&othernode2);
	auto inters = quadTree.findAllIntersections();

	Logger::Debug("Intersections:", inters.size());*/

	/*auto box = quadtree::Box(0.0f, 0.0f, (float)world->GetWorldSize().x, (float)world->GetWorldSize().y);
	quadTree = quadtree::Quadtree<Node*, decltype(getBox)>(box, getBox);
	Node playerNode;
	playerNode.box.left = playerStartPosition.x;
	playerNode.box.top = playerStartPosition.y;
	playerNode.box.width = DEFAULT_PLAYER_WIDTH;
	playerNode.box.height = DEFAULT_PLAYER_HEIGHT;
	playerNode.id = quadtreeId++;

	quadTree.add(&playerNode);
	auto intersections = quadTree.findAllIntersections();*/
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
				playerJustLeveledUp = world->GetPlayerPtr()->GainXp(xpOrb->GetXpValue());
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
			gameOver = true;
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
	default:
		Logger::Log(Logger::ERROR, "Card type not implemented!", __FILE__, __LINE__);
	}
}

bool GameHandler::PlayerIsMoving()
{
	return playerIsMoving;
}
