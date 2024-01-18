#include "Tools.h"
#include "Logger.h"

long Tools::Math::TickToMs(long tick)
{
    return (tick / FPS) / 1000;
}

int Tools::Math::MsToS(long ms)
{
    return ms * 1000;
}

long long Tools::Time::GetCurrentEpocMs()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Position Tools::ScreenSpace::GetWorldToScreen(Position& target, Position p)
{
    // Player is center (0,0)
    int x = target.x - p.x + GetScreenWidth() / 2;
    int y = target.y - p.y + GetScreenHeight() / 2;

    return Position(x, y);
}
Position_f Tools::ScreenSpace::GetWorldToScreen_f(Position_f& target, Position p)
{
    // Player is center (0,0)
    float x = target.x - p.x + GetScreenWidth() / 2;
    float y = target.y - p.y + GetScreenHeight() / 2;

    return Position_f(x, y);
}

Position Tools::Shape::GetRectangleCenter(int startX, int startY, int width, int height)
{
    int centerX = startX + width / 2;
    int centerY = startY + height / 2;

    return Position(centerX, centerY);
}

Position_f Tools::Shape::GetRectangleCenter(float startX, float startY, int width, int height)
{
    float centerX = startX + width / 2.0f;
    float centerY = startY + height / 2.0f;
    return Position_f(centerX, centerY);
}

/*
    Centers text based on width
    width -> max width
*/
int Tools::Text::CenterTextX(int width, const char* text, int fontSize)
{
    int textWidth = MeasureText(text, fontSize);
    return (width - textWidth) / 2;
}

int Tools::Text::CenterTextY(int height, const char* text, int fontSize)
{
    return (height - fontSize) / 2;
}

int Tools::Random::GenerateRandomCoordinate(int start, int maxDistance)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    int randomOffset = rand() % (2 * maxDistance + 1) - maxDistance;
    int coordinate = start + randomOffset;
    if (coordinate > WORLD_WIDTH)
        coordinate = WORLD_WIDTH - 1;
    return start + randomOffset;
}

Position Tools::Random::GenerateRandomPosition(int minX, int maxX, int minY, int maxY)
{
    Position pos;
    // TODO: Has to set the seed?
    pos.x = GetRandomValue(minX, maxX);
    pos.y = GetRandomValue(minY, maxY);
    return pos;
}

int Tools::Random::GenerateRandomSign()
{
    int randomValue = GetRandomValue(0, 1);
    return randomValue == 0 ? -1 : 1;
}
