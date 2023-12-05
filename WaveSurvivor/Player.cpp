#include "Player.h"
#include "Logger.h"

Player::Player()
{
	moveSpeed = 10.0f;
	xp = 0;
	level = 0;
	levelThreshold = DEFAULT_PLAYER_LEVEL_THRESHOLD;
	pickupRadius = DEFAULT_PLAYER_PICKUP_RADIUS;
	fireSpeed = DEFAULT_PLAYER_ATTACK_SPEED;
}

void Player::MovePlayer(MoveDirection direction, int limit)
{
	switch (direction) {
	case UP:
		MoveUp(limit);
		break;
	case RIGHT:
		MoveRight(limit);
		break;
	case DOWN:
		MoveDown(limit);
		break;
	case LEFT:
		MoveLeft(limit);
		break;
	default:
		Logger::Log(Logger::ERROR, "Direction does not exist");
	}

	UpdateHitbox();
}

bool Player::GainXp(int xpValue)
{
	xp += xpValue;
	return LevelUp();
}

int Player::GetXp()
{
	return xp;
}

int Player::GetLevel()
{
	return level;
}

long long Player::GetLevelThreshold()
{
	return levelThreshold;
}

float Player::GetPickupRadius()
{
	return pickupRadius;
}

void Player::SetPickupRadius(float pickupRadius)
{
	this->pickupRadius = pickupRadius;
}

int Player::GetFireSpeed()
{
	return fireSpeed;
}

void Player::SetFireSpeed(int fireSpeed)
{
	this->fireSpeed = fireSpeed;
}

void Player::AddOrUpgradeSpell(CardType spell)
{
	switch (spell) {
	case THORN_AURA:
		spells.insert(std::make_pair(numSpells++, new ThornAura()));
		break;
	default:
		Logger::Log(Logger::ERROR, "Spell is not yet implemented", __FILE__, __LINE__);
	}
}

void Player::MoveUp(int limit)
{
	float newPos = position.y - moveSpeed;
	if (newPos <= limit) return;

	position.y = newPos;
}

void Player::MoveLeft(int limit)
{
	float newPos = position.x - moveSpeed;
	if (newPos <= limit) return;

	position.x = newPos;
}

void Player::MoveDown(int limit)
{
	float newPos = position.y + moveSpeed;
	if (newPos >= limit) return;
	
	position.y = newPos;
}

void Player::MoveRight(int limit)
{
	float newPos = position.x + moveSpeed;
	if (newPos >= limit) return;
	
	position.x = newPos;
}

bool Player::LevelUp()
{
	if (xp >= levelThreshold) {
		Logger::Debug("Player leveled up");
		level++;
		xp = 0;
		levelThreshold = (long long)level * DEFAULT_PLAYER_LEVEL_THRESHOLD;
		maxHealth += 20;
		fireSpeed -= 50;
		moveSpeed += 0.01f;
		return true;
	}
	return false;
}
