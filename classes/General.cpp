#include "General.h"

bool rtm::SameCoordinates(float a, float b, float delta)
{
    return abs(a - b) <= delta;
}

float rtm::RoundCoordinate(float coord, float delta)
{
    float rounded{ CellToPixel(PixelToCell(coord)) };
    if (SameCoordinates(coord, rounded, delta)) {
        return rounded;
    }
    else {
        return coord;
    }
}

float rtm::RoundToCenter(float coordinate)
{
    return CellToPixel(PixelToCell(coordinate));
}

bool rtm::InCenter(float coordinate, float delta)
{
    return SameCoordinates(RoundToCenter(coordinate), coordinate, delta);
}

float rtm::DistanceToNextCenter(float x, float y, float angle)
{
    switch (AngleToAngleType(angle))
    {
    case rtm::Up: {
        return CellToPixel(PixelToCell(y) + 1) - y;
    }
    case rtm::Right: {
        return CellToPixel(PixelToCell(x) + 1) - x;
    }
    case rtm::Down: {
        return y - CellToPixel(PixelToCell(y) - 1);
    }
    case rtm::Left: {
        return x - CellToPixel(PixelToCell(x) - 1);
    }
    default:
        return 0.f;
    }
}

bool rtm::CenterIsCrossed(float x, float y, float angle, float lastDelta)
{
    switch (AngleToAngleType(angle))
    {
    case rtm::Up: {
        float center{ RoundToCenter(y) };
        return y - lastDelta <= center && center <= y;
    }
    case rtm::Right: {
        float center{ RoundToCenter(x) };
        return x - lastDelta <= center && center <= x;
    }
    case rtm::Down: {
        float center{ RoundToCenter(y) };
        return y <= center && center <= y + lastDelta;
    }
    case rtm::Left: {
        float center{ RoundToCenter(x) };
        return x <= center && center <= x + lastDelta;
    }
    default:
        return false;
    }
}

bool rtm::SameAngles(float a, float b, float delta)
{
    return abs(a - b) <= delta;
}

float rtm::RoundAngle(float angle, float delta)
{
    float rounded{ F_PI_4 * round(angle / F_PI_4) };
    if (SameAngles(angle, rounded, delta)) {
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

rtm::AngleType rtm::AngleToAngleType(float angle)
{
    angle = NormalizeAngle(angle);
    if (SameAngles(angle, ANGLE_UP, F_PI_8)) {
        return Up;
    }
    else if (SameAngles(angle, ANGLE_RIGHT, F_PI_8)) {
        return Right;
    }
    else if (SameAngles(angle, ANGLE_DOWN, F_PI_8) || SameAngles(angle, -ANGLE_DOWN, F_PI_8)) {
        return Down;
    }
    else if (SameAngles(angle, ANGLE_LEFT, F_PI_8)) {
        return Left;
    }
    else if (SameAngles(angle, ANGLE_UP_RIGHT, F_PI_8)) {
        return UpRight;
    }
    else if (SameAngles(angle, ANGLE_DOWN_RIGHT, F_PI_8)) {
        return DownRight;
    }
    else if (SameAngles(angle, ANGLE_DOWN_LEFT, F_PI_8)) {
        return DownLeft;
    }
    else if (SameAngles(angle, ANGLE_UP_LEFT, F_PI_8)) {
        return UpLeft;
    }
    else {
        return NullAngle;
    }
}

rtm::DirectionType rtm::AngleToDirection(float angle)
{
    angle = NormalizeAngle(angle);
    if (SameAngles(angle, ANGLE_UP, F_PI_4)) {
        return Upward;
    }
    else if (SameAngles(angle, ANGLE_RIGHT, F_PI_4)) {
        return Rightward;
    }
    else if (SameAngles(angle, ANGLE_DOWN, F_PI_4) || SameAngles(angle, -ANGLE_DOWN, F_PI_4)) {
        return Downward;
    }
    else if (SameAngles(angle, ANGLE_LEFT, F_PI_4)) {
        return Leftward;
    }
    else {
        return NullDirection;
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

rtm::DirectionType rtm::AngleTypeToDirection(AngleType angle)
{
    switch (angle) {
    case Up:
        return Upward;
    case Right:
        return Rightward;
    case Down:
        return Downward;
    case Left:
        return Leftward;
    default:
        return NullDirection;
    }
}

float rtm::DirectionToAngle(DirectionType direction)
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

rtm::AngleType rtm::DirectionToAngleType(DirectionType direction)
{
    switch (direction)
    {
    case rtm::Upward:
        return Up;
    case rtm::Rightward:
        return Right;
    case rtm::Downward:
        return Down;
    case rtm::Leftward:
        return Left;
    default:
        return NullAngle;
    }
}

std::string rtm::GetFilename(std::string const & mask, size_t number)
{
    std::string filename{ mask };
    auto it{ filename.find("%No%") };
    filename.replace(it, 4, std::to_string(static_cast<int>(number)));

    return filename;
}

rtm::AngleType rtm::SumAngleTypes(AngleType a, AngleType b)
{
    return AngleToAngleType(AngleTypeToAngle(a) + AngleTypeToAngle(b));
}

float rtm::CountDeceleration(float maxSpeed) {
    return (maxSpeed * maxSpeed) / (0.4f * VIEW_RADIUS);
}