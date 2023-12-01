#include "Projectile.h"

Projectile::Projectile()
{
    position = { 0,0 };
    speed = DEFAULT_PROJECTILE_SPEED;
    damage = DEFAULT_PROJECTILE_DAMAGE;
    destination = { 0, 0 };
}

Projectile::Projectile(Position_f pos, Position_f destination)
{
    position = pos;
    this->destination = destination;
    speed = DEFAULT_PROJECTILE_SPEED;
    damage = DEFAULT_PROJECTILE_DAMAGE;
}

Position_f Projectile::GetPosition()
{
    return position;
}

void Projectile::SetPosition(Position_f pos)
{
    position = pos;
}

float Projectile::GetSpeed()
{
    return speed;
}

void Projectile::SetSpeed(int speed)
{
    this->speed = speed;
}

int Projectile::GetDamage()
{
    return damage;
}

void Projectile::SetDamage(int damage)
{
    this->damage = damage;
}

Position_f Projectile::GetDestination()
{
    return destination;
}

bool Projectile::HasReachedDestination()
{
    int faultXStart = position.x - speed;
    int faultXEnd = position.x + speed;
    int faultYStart = position.y - speed;
    int faultYEnd = position.y + speed;

    return ((destination.x >= faultXStart && destination.x <= faultXEnd) &&
            (destination.y >= faultYStart && destination.y <= faultYEnd));
}
