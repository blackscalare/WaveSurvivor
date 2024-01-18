#pragma once
#include "../../Definitions.h"
#include "../../TextureHandler.h"

class OptionsMenu
{
public:
	OptionsMenu();
	void Update();

private:
	void Init();

    char resolutionDropdownText[1024] = "640x480;1280x720";
    int selectedResolution = 1;
    bool resolutionDropdownEditMode = false;

	float frameHeight;
	Rectangle sourceRec;
	Vector2 mousePoint;
};

