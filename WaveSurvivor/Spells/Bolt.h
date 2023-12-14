#pragma once
#include "BaseSpell.h"
class Bolt : public BaseSpell
{
public:
	Bolt(int id, Texture2D* texture, Shader* shader);
	SpellType GetType() const override;
};

