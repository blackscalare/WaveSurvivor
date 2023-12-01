#pragma once
#include "Definitions.h"
#include "Character.h"

class Zombie : public Character
{
public:
	Zombie();
	Zombie(const Position& playerPosition, int maxDistance);

	void UpdateHitbox() override;
	int GetXpValue();

private:
	int xpValue;
};

