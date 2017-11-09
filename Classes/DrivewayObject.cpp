#include "DrivewayObject.h"
#include "CoatingObject.h"
#include "RoadCoating.h"

rtm::DrivewayObject::DrivewayObject()
    : CoatingUnion{}
    , direction_{ TopDirection }
    , length_{ 0.f }
{}

rtm::DrivewayObject::DrivewayObject(int column, int row,
    std::vector<std::vector<CoatingObjectUnique>>&& objects, DirectionType direction)
    : CoatingUnion{ DrivewayType, column, row, std::move(objects) }
    , direction_{ direction }
    , length_{ CountLength_(GetWidth_(), GetHeight_(), direction) }
{}

rtm::DrivewayObject::DrivewayObject(int column, int row, size_t width, size_t height, DirectionType direction)
    : DrivewayObject{ column, row, std::move(DrivewayObject::CreateMatrix(column, row, width, height, direction)), direction }
{}

std::vector<std::vector<rtm::CoatingObjectUnique>> rtm::DrivewayObject::CreateMatrix(int column, int row,
    size_t width, size_t height, DirectionType direction)
{
    std::vector<std::vector<rtm::CoatingObjectUnique>> result{ width };

    for (size_t i = 0; i < width; ++i) {
        result[i] = std::vector<rtm::CoatingObjectUnique>{ height };

        for (size_t j = 0; j < height; ++j) {
            RoadType type{ static_cast<RoadType>(0) };
            DirectionType extraDirection{ TopDirection };

            // Choose properties of road
            if (direction == TopDirection || direction == BottomDirection) {
                if (width == 1) {
                    type = RoadTypeNo1;
                }
                else if (i == 0) {
                    type = RoadTypeNo2;
                    if (direction == BottomDirection) {
                        extraDirection = BottomDirection;
                    }
                }
                else if (i == width - 1) {
                    type = RoadTypeNo2;
                    if (direction == TopDirection) {
                        extraDirection = BottomDirection;
                    }
                }
                else {
                    type = RoadTypeNo3;
                }
            }
            else if (direction == RightDirection || direction == LeftDirection) {
                if (height == 1) {
                    type = RoadTypeNo1;
                }
                else if (j == 0) {
                    type = RoadTypeNo2;
                    if (direction == RightDirection) {
                        extraDirection = BottomDirection;
                    }
                }
                else if (j == height - 1) {
                    type = RoadTypeNo2;
                    if (direction == LeftDirection) {
                        extraDirection = BottomDirection;
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
                , DirectionsSum(direction, extraDirection)
            );

            // Block backward direction
            result[i][j]->SetDirectionAvailability(
                DirectionsSum(direction, BottomLeftDirection), false
            );
            result[i][j]->SetDirectionAvailability(
                DirectionsSum(direction, BottomDirection), false
            );
            result[i][j]->SetDirectionAvailability(
                DirectionsSum(direction, BottomRightDirection), false
            );
        }
    }
    return result;
}

rtm::DirectionType rtm::DrivewayObject::GetDirection() const
{
    return direction_;
}

float rtm::DrivewayObject::GetLength() const
{
    return length_;
}

float rtm::DrivewayObject::CountLength_(size_t width, size_t height, DirectionType direction)
{
    if (direction == TopDirection || direction == BottomDirection) {
        return static_cast<float>(height);
    }
    else if (direction == LeftDirection || direction == RightDirection) {
        return static_cast<float>(width);
    }
    else if (direction == TopRightDirection || direction == BottomRightDirection ||
        direction == BottomLeftDirection || direction == TopLeftDirection) {
        return FT::length(width, height);
    }
    else {
        return 0.f;
    }
}
