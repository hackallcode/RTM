#include "CarObject.h"
#include "CoatingObject.h"

rtm::CarObject::CarObject()
    : VehicleObject{}
    , recommendedSpeed_{ 0.f }
    , desiredSpeed_{ 0.f }
    , hasDesiredSpeed_{ false }
    , forwardSightEnabled_{ false }
{}

rtm::CarObject::CarObject(cocos2d::Sprite* const sprite, int column, int row, float angle, float maxSpeed, float acceleration)
    : VehicleObject{ sprite, column, row, angle, maxSpeed, acceleration, CountDeceleration(maxSpeed) }
    , recommendedSpeed_{ GetMaxSpeed_() }
    , desiredSpeed_{ 0.f }
    , hasDesiredSpeed_{ false }
    , forwardSightEnabled_{ true }
{}

rtm::CarObject::CarObject(std::string const& filename, int column, int row, float angle, float maxSpeed, float acceleration)
    : VehicleObject{ filename, column, row, angle, maxSpeed, acceleration, CountDeceleration(maxSpeed) }
    , recommendedSpeed_{ GetMaxSpeed_() }
    , desiredSpeed_{ 0.f }
    , hasDesiredSpeed_{ false }
    , forwardSightEnabled_{ true }
{}

rtm::CarObject::CarObject(CarType type, int column, int row, float angle)
    : CarObject{
        CarObject::GetClassFile_(type)
        , column
        , row
        , angle
        , CarObject::GetClassMaxSpeed_(type)
        , CarObject::GetClassAcceleration_(type)
    }
{}

void rtm::CarObject::Update(WorldController* const world)
{
    switch (GetCaseNumber()) {
    case 3:
        if (IsSameCoords(GetX(), 20.5 * CELL_SIZE) && (
            IsSameCoords(GetY(), 10.5 * CELL_SIZE) || IsSameCoords(GetY(), 20.5 * CELL_SIZE)
            )) {
            ChangeLine_(LEFT);
        }
        break;
    case 4:
        if (IsSameCoords(GetX(), 20.5 * CELL_SIZE) && (
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

bool rtm::CarObject::MovementStart_(WorldController* const world)
{
    return true;
}

bool rtm::CarObject::MovementTick_(WorldController* const world)
{
    // Check roads
    CheckRoadAhead_(world);

    // Set speed
    if (hasDesiredSpeed_) {
        SetFinalSpeed_(desiredSpeed_);
    }
    else {
        SetFinalSpeed_(recommendedSpeed_);
    }

    // Check other vehicles
    if (forwardSightEnabled_) {
        DynamicObject* object{ CheckMovingArea_(world) };
        if (object != nullptr) {
            float newSpeed{ object->GetSpeed() * FT::cos(object->GetAngle() - GetAngle()) };
            // If forward
            if (newSpeed >= 0.f) {
                SetBrakingFactor_(1.f);
                SetFinalSpeed_(min(GetFinalSpeed_(), newSpeed)); // If towards each other
            }
            // If towards
            else {
                SetBrakingFactor_(2.f);
                SetFinalSpeed_(0.f);
            }
        }
        else {
            SetBrakingFactor_(1.f);
        }
    }

    return false;
}

bool rtm::CarObject::MovementEnd_(WorldController* const world)
{
    SetFinalSpeed_(0.f);
    return true;
}

bool rtm::CarObject::RotationStart_(WorldController* const world)
{
    if (CheckRotationArea_(world) == nullptr) {
        ResetDesiredSpeed_();
        return true;
    }
    else {
        SetDesiredSpeed_(0.f);
        return false;
    }
}

bool rtm::CarObject::LineChangingStart(WorldController* const world)
{
    DynamicObject* object{ CheckLineChangingArea_(world) };
    if (object == nullptr) {
        ResetDesiredSpeed_();
        return true;
    }
    else {
        SetDesiredSpeed_(object->GetSpeed() / 2);
        return false;
    }
}

void rtm::CarObject::SetDesiredSpeed_(float speed)
{
    hasDesiredSpeed_ = true;
    desiredSpeed_ = speed;
}

void rtm::CarObject::ResetDesiredSpeed_()
{
    hasDesiredSpeed_ = false;
}

void rtm::CarObject::CheckRoadAhead_(WorldController* const world)
{
    // If does another movement
    if (IsRotation_() != NotStarted || IsLineChanging_() != NotStarted) {
        return;
    }

    // If not in center of cell
    if (!IsInCenter(GetX()) || !IsInCenter(GetY())) {
        return;
    }

    {
        // Coating ahead
        CoatingObject* coating{ CheckForwardCoating_(world, 1) };
        if (coating != nullptr) {
            // If has no forward road coating
            if (!coating->IsDirectionAvailable(AngleToDirection(GetAngle()))) {
                if (coating->IsDirectionAvailable(AngleToDirection(GetAngle() + ANGLE_RIGHT))) {
                    forwardSightEnabled_ = true;
                    ResetDesiredSpeed_();
                    Rotate_(ANGLE_RIGHT);
                }
                else if (coating->IsDirectionAvailable(AngleToDirection(GetAngle() + ANGLE_LEFT))) {
                    forwardSightEnabled_ = true;
                    ResetDesiredSpeed_();
                    Rotate_(ANGLE_LEFT);
                }
                else if (coating->IsDirectionAvailable(AngleToDirection(GetAngle() + ANGLE_BOTTOM))) {
                    forwardSightEnabled_ = true;
                    ResetDesiredSpeed_();
                    Rotate_(ANGLE_BOTTOM);
                }
                else {
                    SetDesiredSpeed_(0.f);
                }
            }
        }
    }
    
    {
        // Coating throw one cell
        CoatingObject* coating{ CheckForwardCoating_(world, 2) };
        if (coating != nullptr) {
            // If T-crossroad
            if (!coating->IsDirectionAvailable(AngleToDirection(GetAngle()))
                && coating->HasDirection(AngleToDirection(GetAngle() + ANGLE_RIGHT))
                && coating->HasDirection(AngleToDirection(GetAngle() + ANGLE_LEFT))) {
                forwardSightEnabled_ = false;
                StopAtDistance_(abs(FT::length(GetX() - coating->GetX(), GetY() - coating->GetY()) - CELL_SIZE));
            }
        }
    }
}

std::string rtm::CarObject::GetClassFile_(CarType type)
{
    std::string filename{ CAR_FILENAME_MASK };
    auto it{ filename.find("%No%") };
    filename.replace(it, 4, std::to_string(static_cast<int>(type)));

    return filename;
}

float rtm::CarObject::GetClassMaxSpeed_(CarType type)
{
    int number = static_cast<int>(type);
    if (0 <= number && number < static_cast<int>(CARS_MAX_SPEEDS.size())) {
        return CARS_MAX_SPEEDS[number];
    }
    else {
        return CARS_MAX_SPEEDS[0];
    }
}

float rtm::CarObject::GetClassAcceleration_(CarType type)
{
    int number = static_cast<int>(type);
    if (0 <= number && number < static_cast<int>(CARS_ACCELERATIONS.size())) {
        return CARS_ACCELERATIONS[number];
    }
    else {
        return CARS_ACCELERATIONS[0];
    }
}