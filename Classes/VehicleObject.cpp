#include "MapController.h"
#include "VehicleObject.h"

rtm::VehicleObject::VehicleObject()
    : DynamicObject()
    , maxSpeed_(0.f)
    , acceleration_(0.f)
    , deceleration_(0.f)
    , isRotation_(false)
    , isLineChanging_(false)
    , remainingAngle_(0.f)
    , remainingOffset_(0.f)
    , remainingOffsetAngle_(0.f)
{}

rtm::VehicleObject::VehicleObject(std::string const& filename, float maxSpeed, float acceleration, float deceleration, int row, int column, float angle)
    : DynamicObject((row + 0.5) * CELL_SIZE, (column + 0.5) * CELL_SIZE, angle, 0, filename)
    , maxSpeed_(maxSpeed)
    , acceleration_(acceleration)
    , deceleration_(deceleration)
    , isRotation_(false)
    , isLineChanging_(false)
    , remainingAngle_(0.f)
    , remainingOffset_(0.f)
    , remainingOffsetAngle_(0.f)
{}

void rtm::VehicleObject::Update(MapController* const map)
{
    if (HasCollision_()) {
        return;
    }
    else {
        Accelerate_(map->GetDeltaTime());
        if (IsSameCoords_(GetX(), 28.5 * CELL_SIZE)) {
            ChangeLine_(rand() % 2);
        } else if (IsSameCoords_(GetX(), 32.5 * CELL_SIZE)) {
            switch (rand() % 3)
            {
            case 0:
                Rotate_(ANGLE_RIGHT);
                break;
            case 1:
                Rotate_(ANGLE_BOTTOM);
                break;
            case 2:
                Rotate_(ANGLE_LEFT);
            }
        }
        Move_(map->GetDeltaTime());
    }

    DynamicObject::Update(map);
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
    if (isRotation_) {
        return false;
    }
    else {
        // Angle of rotation
        remainingAngle_ = RoundAngle_(angle);

        isRotation_ = true;
        return true;
    }
}

bool rtm::VehicleObject::ChangeLine_(bool isRight)
{
    if (isLineChanging_) {
        return false;
    }
    else {
        // Length of normal relative to speed
        remainingOffset_ = CELL_SIZE;
        // Angle of normal relative to speed
        remainingOffsetAngle_ = isRight ? GetAngle() + F_PI_2 : GetAngle() - F_PI_2;

        isLineChanging_ = true;
        return true;
    }
}

void rtm::VehicleObject::Move_(float deltaTime)
{
    Rotation_(deltaTime);
    LineChanging_(deltaTime);
}

void rtm::VehicleObject::Rotation_(float deltaTime)
{
    if (isRotation_) {
        float delta = (GetSpeed_() / CELL_SIZE) * deltaTime;
        delta = remainingAngle_ > 0.f ? MIN(delta, remainingAngle_) : MAX(-delta, remainingAngle_);

        SetAngle_(GetAngle() + delta);
        remainingAngle_ -= delta;

        // End
        if (remainingAngle_ == 0.f) {
            SetX_(RoundCoord_(GetX(), 2 * COORD_DELTA));
            SetY_(RoundCoord_(GetY(), 2 * COORD_DELTA));
            isRotation_ = false;
        }
    }
}

void rtm::VehicleObject::LineChanging_(float deltaTime)
{
    if (isLineChanging_) {
        // Angle between speed and normal relative to it
        float delta = remainingOffsetAngle_ - GetAngle();

        // Begin
        if (IsSameCoords_(remainingOffset_, CELL_SIZE)) {
            Rotate_(delta > 0.f ? F_PI_4 : -F_PI_4);
        }
        // I want to come back on my way!
        else if (IsSameCoords_(remainingOffset_, CELL_SIZE / 4)) {
            Rotate_(delta > 0.f ? -F_PI_4 : F_PI_4);
        }
        // End
        else if (remainingOffset_ == 0.f) {
            isLineChanging_ = false;
            remainingOffsetAngle_ = 0.f;
        }

        // Normal offset relative to speed
        remainingOffset_ -= MIN(
            abs(FTA::cos(delta) * GetSpeed_() * deltaTime), 
            remainingOffset_
        );
    }
}
