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
	int randomX = Tools::Random::GenerateRandomCoordinate(playerPosition.x + MIN_DISTANCE_FROM_PLAYER, maxDistance);
	int randomY = Tools::Random::GenerateRandomCoordinate(playerPosition.y + MIN_DISTANCE_FROM_PLAYER, maxDistance);


	position = Position(randomX, randomY);
	Logger::Log(Logger::DEBUG, "Zombie pos:", "(", randomX, ",", randomY, ")");
	health = DEFAULT_ZOMBIE_HEALTH;
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
