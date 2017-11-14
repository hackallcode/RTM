#include "General.h"

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

bool rtm::IsInCenter(float coordinate)
{
    return IsSameCoords(CellCenterRound(coordinate), coordinate);
}

float rtm::CellCenterRound(float coordinate)
{
    return CellToPixel(PixelToCell(coordinate));
}

int rtm::PixelToCell(float coordinate)
{
    return static_cast<int>(floor(coordinate / CELL_SIZE));
}

float rtm::CellToPixel(int cellNumber)
{
    return (cellNumber + 0.5f) * CELL_SIZE;
}

rtm::AngleType rtm::AngleToAngleType(float angle)
{
    angle = NormalizeAngle(angle);
    if (IsSameAngles(angle, ANGLE_UP, F_PI_8)) {
        return Up;
    }
    else if (IsSameAngles(angle, ANGLE_RIGHT, F_PI_8)) {
        return Right;
    }
    else if (IsSameAngles(angle, ANGLE_DOWN, F_PI_8)) {
        return Down;
    }
    else if (IsSameAngles(angle, ANGLE_LEFT, F_PI_8)) {
        return Left;
    }
    else if (IsSameAngles(angle, ANGLE_UP_RIGHT, F_PI_8)) {
        return UpRight;
    }
    else if (IsSameAngles(angle, ANGLE_DOWN_RIGHT, F_PI_8)) {
        return DownRight;
    }
    else if (IsSameAngles(angle, ANGLE_DOWN_LEFT, F_PI_8)) {
        return DownLeft;
    }
    else if (IsSameAngles(angle, ANGLE_UP_LEFT, F_PI_8)) {
        return UpLeft;
    }
    else {
        return Up;
    }
}

float rtm::AngleTypeToAngle(AngleType angle)
{
    switch (angle) {
    case Up:
        return ANGLE_UP;
    case Right:
        return ANGLE_RIGHT;
    case Down:
        return ANGLE_DOWN;
    case Left:
        return ANGLE_LEFT;
    case UpRight:
        return ANGLE_UP_RIGHT;
    case DownRight:
        return ANGLE_DOWN_RIGHT;
    case DownLeft:
        return ANGLE_DOWN_LEFT;
    case UpLeft:
        return ANGLE_UP_LEFT;
    default:
        return 0.f;
    }
}

rtm::DirectionType rtm::AngleToDirection(float angle)
{
    angle = NormalizeAngle(angle);
    if (IsSameAngles(angle, ANGLE_UP, F_PI_4)) {
        return Upward;
    }
    else if (IsSameAngles(angle, ANGLE_RIGHT, F_PI_4)) {
        return Rightward;
    }
    else if (IsSameAngles(angle, ANGLE_DOWN, F_PI_4)) {
        return Downward;
    }
    else if (IsSameAngles(angle, ANGLE_LEFT, F_PI_4)) {
        return Leftward;
    }
    else {
        return Upward;
    }
}

float rtm::AngleToDirection(DirectionType direction)
{
    switch (direction)
    {
    case rtm::Upward:
        return ANGLE_UP;
    case rtm::Rightward:
        return ANGLE_RIGHT;
    case rtm::Downward:
        return ANGLE_DOWN;
    case rtm::Leftward:
        return ANGLE_LEFT;
    default:
        return 0.f;
    }
}

rtm::AngleType rtm::AngleTypeSum(AngleType a, AngleType b)
{
    return AngleToAngleType(AngleTypeToAngle(a) + AngleTypeToAngle(b));
}

float rtm::CountDeceleration(float maxSpeed) {
    return (maxSpeed * maxSpeed) / (2 * CELL_SIZE);
}

/* TODO: Delete (only for tests) */

int caseNumber_ = -1;

int rtm::GetCaseNumber()
{
    return caseNumber_;
}

void rtm::SetCaseNumber(int number)
{
    caseNumber_ = number;
}
