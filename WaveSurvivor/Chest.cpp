#include "Chest.h"

Chest::Chest()
{
}

Chest::Chest(Position pos)
{
    position = pos;
    type = CHEST;
}

ObjectType Chest::GetType() const
{
    return CHEST;
}
