#include "DrivewayObject.h"
#include "CoatingObject.h"
#include "RoadCoating.h"

rtm::DrivewayObject::DrivewayObject()
    : CoatingUnion{}
    , angle_{ Up }
    , length_{ 0.f }
    , linesCount_{ 0 }
{}

rtm::DrivewayObject::DrivewayObject(CoatingType type, int column, int row, size_t width, size_t height, AngleType angle)
    : CoatingUnion{ DrivewayType, column, row, std::move(DrivewayObject::DrivewayMatrix(type, column, row, width, height, angle)) }
    , angle_{ angle }
    , length_{ CountLength_(GetWidth(), GetHeight(), angle) }
    , linesCount_{ CountLines_(GetWidth(), GetHeight(), angle) }
{}

rtm::CoatingMatrix rtm::DrivewayObject::DrivewayMatrix(CoatingType type, int column, int row, size_t width, size_t height, AngleType angle)
{
    CoatingMatrix result{ width };

    for (size_t i = 0; i < width; ++i) {
        result[i] = CoatingVector{ height };

        for (size_t j = 0; j < height; ++j) {
            size_t id{ 0 };
            AngleType extraDirection{ Up };

            // Choose properties of road
            if (angle == Up || angle == Down) {
                if (width == 1) {
                    id = 1;
                }
                else if (i == 0) {
                    id = 2;
                    if (angle == Down) {
                        extraDirection = Down;
                    }
                }
                else if (i == width - 1) {
                    id = 2;
                    if (angle == Up) {
                        extraDirection = Down;
                    }
                }
                else {
                    id = 3;
                }
            }
            else if (angle == Right || angle == Left) {
                if (height == 1) {
                    id = 1;
                }
                else if (j == 0) {
                    id = 2;
                    if (angle == Right) {
                        extraDirection = Down;
                    }
                }
                else if (j == height - 1) {
                    id = 2;
                    if (angle == Left) {
                        extraDirection = Down;
                    }
                }
                else {
                    id = 3;
                }
            }

            // Make road
            result[i][j] = std::make_unique<RoadCoating>(type, id, column + i, row + j, SumAngleTypes(angle, extraDirection));

            // Block backward direction
            result[i][j]->SetDirectionAvailability(
                SumAngleTypes(angle, DownLeft), false
            );
            result[i][j]->SetDirectionAvailability(
                SumAngleTypes(angle, Down), false
            );
            result[i][j]->SetDirectionAvailability(
                SumAngleTypes(angle, DownRight), false
            );
        }
    }
    return result;
}

float rtm::DrivewayObject::GetLength() const
{
    return length_;
}

size_t rtm::DrivewayObject::GetLinesCount() const
{
    return linesCount_;
}

bool rtm::DrivewayObject::isRightLine(int column, int row) const
{
    if (angle_ == Up) {
        return column == GetColumn_() + GetWidth() - 1;
    }
    else if (angle_ == Left) {
        return row == GetRow_() + GetHeight() - 1;
    }
    else if (angle_ == Down) {
        return column == GetColumn_();
    }
    else if (angle_ == Right) {
        return row == GetRow_();
    }
    else {
        return false;
    }
}

bool rtm::DrivewayObject::isRightLine(float x, float y) const
{
    if (angle_ == Up) {
        return PixelToCell(x) == GetColumn_() + GetWidth() - 1;
    }
    else if (angle_ == Left) {
        return PixelToCell(y) == GetRow_() + GetHeight() - 1;
    }
    else if (angle_ == Down) {
        return PixelToCell(x) == GetColumn_();
    }
    else if (angle_ == Right) {
        return PixelToCell(y) == GetRow_();
    }
    else {
        return false;
    }
}

bool rtm::DrivewayObject::isLeftLine(int column, int row) const
{
    if (angle_ == Up) {
        return column == GetColumn_();
    }
    else if (angle_ == Left) {
        return row == GetRow_();
    }
    else if (angle_ == Down) {
        return column == GetColumn_() + GetWidth() - 1;
    }
    else if (angle_ == Right) {
        return row == GetRow_() + GetHeight() - 1;
    }
    else {
        return false;
    }
}

bool rtm::DrivewayObject::isLeftLine(float x, float y) const
{
    if (angle_ == Up) {
        return PixelToCell(x) == GetColumn_();        
    }
    else if (angle_ == Left) {
        return PixelToCell(y) == GetRow_();        
    }
    else if (angle_ == Down) {
        return PixelToCell(x) == GetColumn_() + GetWidth() - 1;
    }
    else if (angle_ == Right) {
        return PixelToCell(y) == GetRow_() + GetHeight() - 1;
    }
    else {
        return false;
    }
}

float rtm::DrivewayObject::CountLength_(size_t width, size_t height, AngleType angle)
{
    if (angle == Up || angle == Down) {
        return static_cast<float>(height);
    }
    else if (angle == Left || angle == Right) {
        return static_cast<float>(width);
    }
    else {
        return 0.f;
    }
}

size_t rtm::DrivewayObject::CountLines_(size_t width, size_t height, AngleType angle)
{
    if (angle == Up || angle == Down) {
        return width;
    }
    else if (angle == Left || angle == Right) {
        return height;
    }
    else {
        return 0;
    }
}
