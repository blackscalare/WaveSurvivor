#pragma once
#include "Definitions.h"
#include "Character.h"

class Player : public Character
{
public:
	Player();
	
	void MovePlayer(MoveDirection direction, int limit);
	bool GainXp(int xpValue);
	int GetXp();
	int GetLevel();
	long long GetLevelThreshold();
	float GetPickupRadius();
	void SetPickupRadius(float pickupRadius);
	int GetFireSpeed();
	void SetFireSpeed(int fireSpeed);

private:
	void MoveUp(int limit);
	void MoveLeft(int limit);
	void MoveDown(int limit);
	void MoveRight(int limit);
	bool LevelUp();

	int xp;
	int level;
	long long levelThreshold;
	float pickupRadius;
	int fireSpeed;
};

