#pragma once
#include "Definitions.h"
class Projectile
{
public:
	Projectile();
	Projectile(Position_f pos, Position_f destination);

	Position_f GetPosition();
	void SetPosition(Position_f pos);
	float GetSpeed();
	void SetSpeed(int speed);
	int GetDamage();
	void SetDamage(int damage);
	Position_f GetDestination();
	bool HasReachedDestination();

	friend bool operator<(const Projectile& lhs, const Projectile& rhs) {
		return (lhs.position.x < rhs.position.x) || ((lhs.position.x == rhs.position.x) && (lhs.position.y < rhs.position.y));
	}

private:
	Position_f position;
	float speed;
	int damage;
	Position_f destination;
};

