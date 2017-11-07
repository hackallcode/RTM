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

float rtm::GetAngle(int number)
{
    switch (number) {
    case 1:
        return ANGLE_TOP;
        break;
    case 2:
        return ANGLE_RIGHT;
        break;
    case 3:
        return ANGLE_BOTTOM;
        break;
    case 4:
        return ANGLE_LEFT;
        break;
    default:
        return 0.f;
        break;
    }
}

float rtm::CountDeceleration(float maxSpeed) {
    return (maxSpeed * maxSpeed) / (2 * CELL_SIZE);
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
