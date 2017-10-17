#include "VehicleObject.h"
#include "WorldScene.h"

rtm::VehicleObject::VehicleObject()
    : DynamicObject()
    , maxSpeed_(0.f)
    , acceleration_(0.f)
    , deceleration_(0.f)
{}

rtm::VehicleObject::VehicleObject(std::string const& filename, float maxSpeed, float acceleration, float deceleration, size_t row, size_t column, float angle)
    : DynamicObject((row + 0.5) * CELL_SIZE, (column + 0.5) * CELL_SIZE, angle, 0, filename)
    , maxSpeed_(maxSpeed)
    , acceleration_(acceleration)
    , deceleration_(deceleration)
{}

void rtm::VehicleObject::Update(World* const scene)
{
    DynamicObject::Update(scene);

    for (auto& obj : scene->getObjects()) {
        if (DoesIntersect_(obj.get())) {
            SetSpeed_(0.f);
        }
    }

    if (GetX() > CELL_SIZE && GetX() < scene->getContentSize().width - CELL_SIZE &&
        GetY() > CELL_SIZE && GetY() < scene->getContentSize().height - CELL_SIZE) {
        Accelerate_(scene->getMissedTime());
    }
    else {
        Decelerate_(scene->getMissedTime());
    }
    OnPositionUpdate_();
}

void rtm::VehicleObject::Accelerate_(float deltaTime)
{
    if (GetSpeed() < maxSpeed_) {
        SetSpeed_(GetSpeed() + acceleration_ * deltaTime);
        if (GetSpeed() > maxSpeed_) {
            SetSpeed_(maxSpeed_);
        }
    }
}

void rtm::VehicleObject::Decelerate_(float deltaTime)
{
    if (GetSpeed() > 0) {
        SetSpeed_(GetSpeed() - deceleration_ * deltaTime);
        if (GetSpeed() < 0) {
            SetSpeed_(0);
        }
    }
}
