#pragma once
#include "Object.h"
#include "Definitions.h"

class XPOrb : public Object
{
public:
	XPOrb();
	XPOrb(Position position);

	int GetXpValue() const;
	ObjectType GetType() const override;
private:
	int xpValue;
};

