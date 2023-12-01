#include "Object.h"

Object::Object()
{
    position = { 0, 0 };
    type = NONE;
}

Object::Object(Position position)
{
    this->position = position;
    type = NONE;
}

Position Object::GetPosition() const
{
    return position;
}

void Object::SetPosition(Position position)
{
    this->position = position;
}

ObjectType Object::GetType() const
{
    return type;
}

Hitbox Object::GetHitbox()
{
    return hitbox;
}
