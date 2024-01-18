#include "OptionsMenu.h"
#include "../../Logger.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
OptionsMenu::OptionsMenu()
{
	Init();
}

void OptionsMenu::Update()
{
    // Resolution dropdown
    GuiUnlock();
    GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 4);
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 0);
    int previousValue = selectedResolution;
    if (GuiDropdownBox({ GetScreenWidth() / 2.0f - (125 / 2), 25, 125, 30}, resolutionDropdownText, &selectedResolution, resolutionDropdownEditMode)) {
        resolutionDropdownEditMode = !resolutionDropdownEditMode;
        if (previousValue != selectedResolution) {
            switch (selectedResolution) {
            case 0:
                SetWindowSize(640, 480);
                break;
            case 1:
                SetWindowSize(1280, 720);
                break;
            default:
                Logger::Log(Logger::ERROR, "Screen resolution not implemented");
            }
        }
    }
    // Resolution dropdown
}

void OptionsMenu::Init()
{

}
