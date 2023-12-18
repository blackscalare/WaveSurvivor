#include "Character.h"

Character::Character()
{
    maxHealth = 100;
    health = 100;
    position = Position(0, 0);
    moveSpeed = 10.0f;
    hitbox = Hitbox(&position, DEFAULT_PLAYER_WIDTH, DEFAULT_PLAYER_HEIGHT);
}

int Character::GetHealth()
{
    return health;
}

void Character::SetHealth(int health)
{
    this->health = health;
}

Position Character::GetPosition()
{
    return position;
}

void Character::SetPosition(Position position)
{
    this->position = position;
    UpdateHitbox();
}

float Character::GetMoveSpeed()
{
    return moveSpeed;
}

void Character::SetMoveSpeed(float moveSpeed)
{
    this->moveSpeed = moveSpeed;
}

Hitbox Character::GetHitbox()
{
    return hitbox;
}

void Character::UpdateHitbox()
{
    hitbox.UpdateHitbox();
}

bool Character::TakeDamage(int damage)
{
    health -= damage;
    return health <= 0;
}

int Character::GetMaxHealth()
{
    return maxHealth;
}
