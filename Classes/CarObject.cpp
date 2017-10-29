#include "WorldController.h"
#include "CarObject.h"

rtm::CarObject::CarObject()
    : VehicleObject{}
{}

rtm::CarObject::CarObject(cocos2d::Sprite* const sprite, int column, int row, float angle, float maxSpeed, float acceleration, float deceleration)
    : VehicleObject{ sprite, column, row, angle, maxSpeed, acceleration, deceleration }
{}

rtm::CarObject::CarObject(std::string const& filename, int column, int row, float angle, float maxSpeed, float acceleration, float deceleration)
    : VehicleObject{ filename, column, row, angle, maxSpeed, acceleration, deceleration }
{}

rtm::CarObject::CarObject(CarType type, int column, int row, float angle)
    : CarObject{
        CarObject::GetClassFile_(type)
        , column
        , row
        , angle
        , CarObject::GetClassMaxSpeed_(type)
        , CarObject::GetClassAcceleration_(type)
        , CarObject::GetClassDeceleration_(type)
    }
{}

void rtm::CarObject::Update(WorldController* const world)
{
    switch (world->caseNum) {
    case 3:
        if (IsSameCoords(GetX(), 15.5 * CELL_SIZE) && (
            IsSameCoords(GetY(), 10.5 * CELL_SIZE) || IsSameCoords(GetY(), 20.5 * CELL_SIZE)
            )) {
            ChangeLine_(LEFT);
        }
        break;
    case 4:
        if (IsSameCoords(GetX(), 15.5 * CELL_SIZE) && (
            IsSameCoords(GetY(), 11.5 * CELL_SIZE) || IsSameCoords(GetY(), 21.5 * CELL_SIZE)
            )) {
            ChangeLine_(RIGHT);
        }
        break;
    case 5:
        if (IsSameCoords(GetY(), 14.5 * CELL_SIZE)) {
            Rotate_(ANGLE_RIGHT);
        }
        break;
    case 6:
        if (IsSameCoords(GetY(), 14.5 * CELL_SIZE)) {
            Rotate_(ANGLE_LEFT);
        }
        break;
    }

    VehicleObject::Update(world);
}

std::string rtm::CarObject::GetClassFile_(CarType type)
{
    switch (type)
    {
    case rtm::CarTypeNo1:
        return CAR_NO_1_FILE;
    case rtm::CarTypeNo2:
        return CAR_NO_2_FILE;
    case rtm::CarTypeNo3:
        return CAR_NO_3_FILE;
    case rtm::CarTypeNo4:
        return CAR_NO_4_FILE;
    case rtm::CarTypeNo5:
        return CAR_NO_5_FILE;
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
    case rtm::CarTypeNo3:
        return CAR_NO_3_MAX_SPEED;
    case rtm::CarTypeNo4:
        return CAR_NO_4_MAX_SPEED;
    case rtm::CarTypeNo5:
        return CAR_NO_5_MAX_SPEED;
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
    case rtm::CarTypeNo3:
        return CAR_NO_3_ACCELERATION;
    case rtm::CarTypeNo4:
        return CAR_NO_4_ACCELERATION;
    case rtm::CarTypeNo5:
        return CAR_NO_5_ACCELERATION;
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
    case rtm::CarTypeNo3:
        return CAR_NO_3_DECELERATION;
    case rtm::CarTypeNo4:
        return CAR_NO_4_DECELERATION;
    case rtm::CarTypeNo5:
        return CAR_NO_5_DECELERATION;
    default:
        return CAR_NO_0_DECELERATION;
    }
}
