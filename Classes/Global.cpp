#include "Global.h"

bool rtm::IsSameCoords(float a, float b, float delta)
{
    return abs(a - b) <= delta;
}

float rtm::RoundCoord(float coord, float delta)
{
    float rounded{ CellToPixel(PixelToCell(coord)) };
    if (IsSameCoords(coord, rounded, delta)) {
        return rounded;
    }
    else {
        return coord;
    }
}

bool rtm::IsSameAngles(float a, float b, float delta)
{
    return abs(a - b) <= delta;
}

float rtm::RoundAngle(float angle, float delta)
{
    float rounded{ F_PI_4 * round(angle / F_PI_4) };
    if (IsSameAngles(angle, rounded, delta)) {
        return rounded;
    }
    else {
        return angle;
    }
}

float rtm::NormalizeAngle(float angle)
{
    while (angle < -F_PI) angle += F_2_PI;
    while (angle >= F_PI) angle -= F_2_PI;
    return angle;
}

int rtm::PixelToCell(float coordinate)
{
    return static_cast<int>(floor(coordinate / CELL_SIZE));
}

float rtm::CellToPixel(int cellNumber)
{
    return (cellNumber + 0.5f) * CELL_SIZE;
}

float rtm::CellCenterRound(float coordinate)
{
    return CellToPixel(PixelToCell(coordinate));
}

bool rtm::IsInCenter(float coordinate)
{
    return IsSameCoords(CellCenterRound(coordinate), coordinate);
}

float rtm::CountDeceleration(float maxSpeed) {
    return (maxSpeed * maxSpeed) / (2 * CELL_SIZE);
}