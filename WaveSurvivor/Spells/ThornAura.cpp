#include "ThornAura.h"

ThornAura::ThornAura()
{
	texture = LoadTexture("../textures/thornAuraTexture.png");
	shader = LoadShader(NULL, "../shaders/thornAura.fs");
}
