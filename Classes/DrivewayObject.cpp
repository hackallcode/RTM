#include "DrivewayObject.h"
#include "CoatingObject.h"
#include "RoadCoating.h"

rtm::DrivewayObject::DrivewayObject()
    : CoatingUnion{}
    , angle_{ Up }
    , length_{ 0.f }
{}

rtm::DrivewayObject::DrivewayObject(int column, int row, size_t width, size_t height, AngleType angle)
    : CoatingUnion{ DrivewayType, column, row, std::move(DrivewayObject::DrivewayMatrix(column, row, width, height, angle)) }
    , angle_{ angle }
    , length_{ CountLength_(GetWidth(), GetHeight(), angle) }
{}

rtm::CoatingMatrix rtm::DrivewayObject::DrivewayMatrix(int column, int row,
    size_t width, size_t height, AngleType angle)
{
    CoatingMatrix result{ width };

    for (size_t i = 0; i < width; ++i) {
        result[i] = CoatingVector{ height };

        for (size_t j = 0; j < height; ++j) {
            RoadType type{ static_cast<RoadType>(0) };
            AngleType extraDirection{ Up };

            // Choose properties of road
            if (angle == Up || angle == Down) {
                if (width == 1) {
                    type = RoadTypeNo1;
                }
                else if (i == 0) {
                    type = RoadTypeNo2;
                    if (angle == Down) {
                        extraDirection = Down;
                    }
                }
                else if (i == width - 1) {
                    type = RoadTypeNo2;
                    if (angle == Up) {
                        extraDirection = Down;
                    }
                }
                else {
                    type = RoadTypeNo3;
                }
            }
            else if (angle == Right || angle == Left) {
                if (height == 1) {
                    type = RoadTypeNo1;
                }
                else if (j == 0) {
                    type = RoadTypeNo2;
                    if (angle == Right) {
                        extraDirection = Down;
                    }
                }
                else if (j == height - 1) {
                    type = RoadTypeNo2;
                    if (angle == Left) {
                        extraDirection = Down;
                    }
                }
                else {
                    type = RoadTypeNo3;
                }
            }

            // Make road
            result[i][j] = std::make_unique<RoadCoating>(
                type
                , column + i
                , row + j
                , AngleTypeSum(angle, extraDirection)
            );

            // Block backward direction
            result[i][j]->SetDirectionAvailability(
                AngleTypeSum(angle, DownLeft), false
            );
            result[i][j]->SetDirectionAvailability(
                AngleTypeSum(angle, Down), false
            );
            result[i][j]->SetDirectionAvailability(
                AngleTypeSum(angle, DownRight), false
            );
        }
    }
    return result;
}

rtm::AngleType rtm::DrivewayObject::GetAngle() const
{
    return angle_;
}

float rtm::DrivewayObject::GetLength() const
{
    return length_;
}

float rtm::DrivewayObject::CountLength_(size_t width, size_t height, AngleType angle)
{
    if (angle == Up || angle == Down) {
        return static_cast<float>(height);
    }
    else if (angle == Left || angle == Right) {
        return static_cast<float>(width);
    }
    else if (angle == UpRight || angle == DownRight ||
        angle == DownLeft || angle == UpLeft) {
        return FT::length(width, height);
    }
    else {
        return 0.f;
    }
}
