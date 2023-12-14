#include "Bolt.h"

Bolt::Bolt(int id, Texture2D* texture, Shader* shader)
{
	/*
		int id;
		int level;
		int damage;
		float fireRate;
		Hitbox hitbox;
		SpellType type;
		Texture2D texture;
		Shader shader;
	*/
	this->id = id;
	this->level = 1;
	this->damage = DEFAULT_BOLT_DAMAGE;
	this->fireRate = DEFAULT_BOLT_FIRE_RATE;
	this->type = SPELL_BOLT;
	this->texture = *texture;
	this->shader = *shader;
}

SpellType Bolt::GetType() const
{
	return SPELL_BOLT;
}
