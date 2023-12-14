#include "CharacterSelect.h"

CharacterSelect::CharacterSelect(TextureHandler* textureHandler)
{
	this->textureHandler = textureHandler;
	Init();
}

void CharacterSelect::Update()
{
}

void CharacterSelect::Init()
{
	backgroundTexture = textureHandler->GetTexture(ALT_MENU_BACKGROUND_TEXTURE);
	characterSelectBorderTexture = textureHandler->GetTexture(CHARACTER_SELECT_BORDER_TEXTURE);


}
