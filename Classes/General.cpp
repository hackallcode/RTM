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

rtm::DirectionType rtm::AngleToDirection(float angle)
{
    angle = NormalizeAngle(angle);
    if (IsSameAngles(angle, ANGLE_TOP, F_PI_8)) {
        return TopDirection;
    }
    else if (IsSameAngles(angle, ANGLE_RIGHT, F_PI_8)) {
        return RightDirection;
    }
    else if (IsSameAngles(angle, ANGLE_BOTTOM, F_PI_8)) {
        return BottomDirection;
    }
    else if (IsSameAngles(angle, ANGLE_LEFT, F_PI_8)) {
        return LeftDirection;
    }
    else if (IsSameAngles(angle, ANGLE_TOP_RIGHT, F_PI_8)) {
        return TopRightDirection;
    }
    else if (IsSameAngles(angle, ANGLE_BOTTOM_RIGHT, F_PI_8)) {
        return BottomRightDirection;
    }
    else if (IsSameAngles(angle, ANGLE_BOTTOM_LEFT, F_PI_8)) {
        return BottomLeftDirection;
    }
    else if (IsSameAngles(angle, ANGLE_TOP_LEFT, F_PI_8)) {
        return TopLeftDirection;
    }
    else {
        return TopDirection;
    }
}

float rtm::DirectionToAngle(DirectionType direction)
{
    switch (direction) {
    case TopDirection:
        return ANGLE_TOP;
    case RightDirection:
        return ANGLE_RIGHT;
    case BottomDirection:
        return ANGLE_BOTTOM;
    case LeftDirection:
        return ANGLE_LEFT;
    case TopRightDirection:
        return ANGLE_TOP_RIGHT;
    case BottomRightDirection:
        return ANGLE_BOTTOM_RIGHT;
    case BottomLeftDirection:
        return ANGLE_BOTTOM_LEFT;
    case TopLeftDirection:
        return ANGLE_TOP_LEFT;
    default:
        return 0.f;
    }
}

rtm::DirectionType rtm::DirectionsSum(DirectionType a, DirectionType b)
{
    return AngleToDirection(DirectionToAngle(a) + DirectionToAngle(b));
}

float rtm::CountDeceleration(float maxSpeed) {
    return (maxSpeed * maxSpeed) / (2 * CELL_SIZE);
}

/* TODO: Delete */

bool rtm::IsSeparator(char c)
{
    return c == ' ';
}

bool rtm::IsNotSeparator(char c)
{
    return !IsSeparator(c);
}

std::vector<int> rtm::Split(std::string const& str) {
    std::vector<int> result;
    auto& it = str.begin();
    while (it != str.end()) {
        it = std::find_if(it, str.end(), IsNotSeparator); // Word beginnig
        auto& j = std::find_if(it, str.end(), IsSeparator); // Word end
        if (it != str.end()) {
            result.push_back(std::stoi(std::string(it, j)));
            it = j; // Move current to word end
        }
    }
    return result;
}

int caseNumber_ = -1;

int rtm::GetCaseNumber()
{
    return caseNumber_;
}

void rtm::SetCaseNumber(int number)
{
    caseNumber_ = number;
}
