#pragma once
#include "../Definitions.h"
class BaseSpell
{
public:
	BaseSpell();

	bool operator==(const BaseSpell& other) const
	{
		return id == other.id;
	}

	friend bool operator<(const BaseSpell& a, const BaseSpell& b)
	{
		return a.id < b.id;
	}

	virtual SpellType GetType() const;

protected:
	int id;
	int level;
	int damage;
	float fireRate;
	Hitbox hitbox;
	SpellType type;
	Texture2D texture;
	Shader shader;
};

