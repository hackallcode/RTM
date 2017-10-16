#include "CarObject.h"
#include "WorldScene.h"

rtm::CarObject::CarObject()
    : VehicleObject()
{}

rtm::CarObject::CarObject(std::string const& filename, float maxSpeed, float acceleration, float deceleration, size_t row, size_t column, float angle)
    : VehicleObject(filename, maxSpeed, acceleration, deceleration, row, column, angle)
{}

rtm::CarObject::CarObject(CarType type, size_t row, size_t column, float angle)
    : CarObject(
        CarObject::GetClassFile_(type)
        , CarObject::GetClassMaxSpeed_(type)
        , CarObject::GetClassAcceleration_(type)
        , CarObject::GetClassDeceleration_(type)
        , row
        , column
        , angle
    )
{}

void rtm::CarObject::Update(World* const scene)
{
    VehicleObject::Update(scene);


}

std::string rtm::CarObject::GetClassFile_(CarType type)
{
    switch (type)
    {
    case rtm::CarTypeNo1:
        return CAR_NO_1_FILE;
    case rtm::CarTypeNo2:
        return CAR_NO_2_FILE;
    default:
        return CAR_NO_0_FILE;
    }
}

float rtm::CarObject::GetClassMaxSpeed_(CarType type)
{
    switch (type)
    {
    case rtm::CarTypeNo1:
        return CAR_NO_1_MAX_SPEED;
    case rtm::CarTypeNo2:
        return CAR_NO_2_MAX_SPEED;
    default:
        return CAR_NO_0_MAX_SPEED;
    }
}

float rtm::CarObject::GetClassAcceleration_(CarType type)
{
    switch (type)
    {
    case rtm::CarTypeNo1:
        return CAR_NO_1_ACCELERATION;
    case rtm::CarTypeNo2:
        return CAR_NO_2_ACCELERATION;
    default:
        return CAR_NO_0_ACCELERATION;
    }
}

float rtm::CarObject::GetClassDeceleration_(CarType type)
{
    switch (type)
    {
    case rtm::CarTypeNo1:
        return CAR_NO_1_DECELERATION;
    case rtm::CarTypeNo2:
        return CAR_NO_2_DECELERATION;
    default:
        return CAR_NO_0_DECELERATION;
    }
}
