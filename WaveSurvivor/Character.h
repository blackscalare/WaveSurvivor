#pragma once
#include "Definitions.h"

class Character
{
public:
	enum MoveDirection
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	Character();

	int GetHealth();
	void SetHealth(int health);
	Position GetPosition();
	void SetPosition(Position position);
	float GetMoveSpeed();
	void SetMoveSpeed(float moveSpeed);
	Hitbox GetHitbox();
	virtual void UpdateHitbox();
	bool TakeDamage(int damage);
	int GetMaxHealth();

protected:
	int maxHealth;
	int health;
	Position position;
	float moveSpeed;
	Hitbox hitbox;
};

