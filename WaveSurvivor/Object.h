#pragma once
#include "Definitions.h"

class Object
{
public:
	Object();
	Object(Position position);

	Position GetPosition() const;
	void SetPosition(Position position);
	virtual ObjectType GetType() const;
	Hitbox GetHitbox();

	friend bool operator<(const Object& lhs, const Object& rhs) {
		return (lhs.position.x < rhs.position.x) || ((lhs.position.x == rhs.position.x) && (lhs.position.y < rhs.position.y));
	}

	friend bool operator==(const Object& lhs, const Object& rhs) {
		return lhs.position.x == rhs.position.x && lhs.position.y == rhs.position.y;
	}

protected:
	Position position;
	ObjectType type;
	Hitbox hitbox;
};

