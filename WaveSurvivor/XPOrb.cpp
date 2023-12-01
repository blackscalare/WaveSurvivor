#include "XPOrb.h"

XPOrb::XPOrb()
{
    xpValue = DEFAULT_ZOMBIE_XP_VALUE;
    type = XP_ORB;
}

XPOrb::XPOrb(Position position)
{
    xpValue = DEFAULT_ZOMBIE_XP_VALUE;
    this->position = position;
    type = XP_ORB;
}

int XPOrb::GetXpValue() const
{
    return xpValue;
}

ObjectType XPOrb::GetType() const
{
    return ObjectType::XP_ORB;
}
