#pragma once
#include "Definitions.h"
#include "Object.h"

class Chest : public Object
{
public:
	Chest();
	Chest(Position pos);

	ObjectType GetType() const override;
};

