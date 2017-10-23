#include "WorldScene.h"
#include "VehicleObject.h"

rtm::VehicleObject::VehicleObject()
    : DynamicObject()
    , maxSpeed_(0.f)
    , acceleration_(0.f)
    , deceleration_(0.f)
    , finalAngle_(0.f)
{}

rtm::VehicleObject::VehicleObject(std::string const& filename, float maxSpeed, float acceleration, float deceleration, int row, int column, float angle)
    : DynamicObject((row + 0.5) * CELL_SIZE, (column + 0.5) * CELL_SIZE, angle, 0, filename)
    , maxSpeed_(maxSpeed)
    , acceleration_(acceleration)
    , deceleration_(deceleration)
    , finalAngle_(angle)
{}

void rtm::VehicleObject::Update(World* const scene)
{
    if (HasCollision_()) {
        return;
    }
    else {
        Accelerate_(scene->getMissedTime());
        if (IsSameCoords_(GetX(), 32.5 * CELL_SIZE)) {
            Rotate_(ANGLE_BOTTOM);
            /*switch (rand() % 3)
            {
            case 0:
                Rotate_(ANGLE_RIGHT);
                break;
            case 1:
                Rotate_(ANGLE_BOTTOM);
                break;
            case 2:
                Rotate_(ANGLE_LEFT);
                break;
            }*/
        }
        Move_(scene->getMissedTime());
    }

    DynamicObject::Update(scene);
}

void rtm::VehicleObject::Accelerate_(float deltaTime)
{
    if (GetSpeed_() < maxSpeed_) {
        SetSpeed_(GetSpeed_() + acceleration_ * deltaTime);
        if (GetSpeed_() > maxSpeed_) {
            SetSpeed_(maxSpeed_);
        }
    }
}

void rtm::VehicleObject::Decelerate_(float deltaTime)
{
    if (GetSpeed_() > 0) {
        SetSpeed_(GetSpeed_() - deceleration_ * deltaTime);
        if (GetSpeed_() < 0) {
            SetSpeed_(0);
        }
    }
}

bool rtm::VehicleObject::Rotate_(float angle)
{
    if (finalAngle_ == GetAngle()) {
        finalAngle_ += angle;
        finalAngle_ = RoundAngle_(NormalizeAngle_(finalAngle_));
        return true;
    }
    else {
        return false;
    }
}

bool rtm::VehicleObject::ChangeLine_(bool isRight)
{
    return false;
}

void rtm::VehicleObject::Move_(float deltaTime)
{
    Rotation_(deltaTime);
    LineChanging_(deltaTime);
}

void rtm::VehicleObject::Rotation_(float deltaTime)
{
    if (finalAngle_ != GetAngle()) {
        float delta = NormalizeAngle_(finalAngle_ - GetAngle());

        if (delta > 0) {
            SetAngle_(GetAngle() + MIN(deltaTime * GetSpeed_() / CELL_SIZE, delta));
        }
        else {
            SetAngle_(GetAngle() + MAX(-1 * deltaTime * GetSpeed_() / CELL_SIZE, delta));
        }
        if (finalAngle_ == GetAngle()) {
            SetX_(RoundCoord_(GetX(), 2 * COORD_DELTA));
            SetY_(RoundCoord_(GetY(), 2 * COORD_DELTA));
        }
    }
}

void rtm::VehicleObject::LineChanging_(float deltaTime)
{
    
}
