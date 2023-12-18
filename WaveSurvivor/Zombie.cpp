#include "Zombie.h"
#include "Logger.h"
#include "Tools.h"

Zombie::Zombie()
{
	//std::srand(static_cast<unsigned>(std::time(nullptr)));
	xpValue = DEFAULT_ZOMBIE_XP_VALUE;
}

Zombie::Zombie(const Position& playerPosition, int maxDistance)
{
	int randomSign1 = Tools::Random::GenerateRandomSign();
	int randomSign2 = Tools::Random::GenerateRandomSign();

	int minDistanceFromPlayerX = playerPosition.x + (MIN_DISTANCE_FROM_PLAYER * randomSign1);
	int minDistanceFromPlayerY = playerPosition.y + (MIN_DISTANCE_FROM_PLAYER * randomSign2);

	Position randomPosition = Tools::Random::GenerateRandomPosition(
		randomSign1 == -1 ? std::max((maxDistance * randomSign1) + minDistanceFromPlayerX, 1) : std::min(minDistanceFromPlayerX, WORLD_WIDTH - 1),
		randomSign1 == -1 ? std::max(minDistanceFromPlayerX, 1) : std::min(maxDistance + minDistanceFromPlayerX, WORLD_WIDTH - 1),
		randomSign1 == -1 ? std::max((maxDistance * randomSign1) + minDistanceFromPlayerY, 1) : std::min(minDistanceFromPlayerY, WORLD_HEIGHT - 1),
		randomSign1 == -1 ? std::max(minDistanceFromPlayerY, 1) : std::min(maxDistance + minDistanceFromPlayerY, WORLD_HEIGHT - 1)
	);

	position = randomPosition;
	Logger::Log(Logger::DEBUG, "Zombie pos:", "(", randomPosition.x, ",", randomPosition.y, ")");
	health = DEFAULT_ZOMBIE_HEALTH;
	maxHealth = DEFAULT_ZOMBIE_HEALTH;
	moveSpeed = DEFAULT_ZOMBIE_MOVE_SPEED;
	hitbox.area.width = DEFAULT_ZOMBIE_WIDTH;
	hitbox.area.height = DEFAULT_ZOMBIE_HEIGHT;
	xpValue = DEFAULT_ZOMBIE_XP_VALUE;
}

void Zombie::UpdateHitbox()
{
	hitbox.UpdateHitbox();
}

int Zombie::GetXpValue()
{
	return xpValue;
}
