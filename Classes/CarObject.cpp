#include "CarObject.h"
#include "CoatingObject.h"
#include "CoatingUnion.h"
#include "DrivewayObject.h"
#include "CrossroadObject.h"
#include "ControlUnit.h"
#include "WorldController.h"

rtm::CarObject::CarObject()
    : VehicleObject{}
    , recommendedSpeed_{ 0.f }
    , desiredSpeed_{ 0.f }
    , hasDesiredSpeed_{ false }
    , waitForSignal_{ false }
    , desiredDirection_{ NullAngle }
{}

rtm::CarObject::CarObject(cocos2d::Sprite* const sprite, int column, int row, float angle, float maxSpeed, float acceleration)
    : VehicleObject{ sprite, column, row, angle, maxSpeed, acceleration, CountDeceleration(maxSpeed) }
    , recommendedSpeed_{ GetMaxSpeed_() }
    , desiredSpeed_{ 0.f }
    , hasDesiredSpeed_{ false }
    , waitForSignal_{ false }
    , desiredDirection_{ AngleToAngleType(angle) }
{}

rtm::CarObject::CarObject(std::string const& filename, int column, int row, float angle, float maxSpeed, float acceleration)
    : VehicleObject{ filename, column, row, angle, maxSpeed, acceleration, CountDeceleration(maxSpeed) }
    , recommendedSpeed_{ GetMaxSpeed_() }
    , desiredSpeed_{ 0.f }
    , hasDesiredSpeed_{ false }
    , waitForSignal_{ false }
    , desiredDirection_{ AngleToAngleType(angle) }
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
    if (!IsRotation_() && !IsLineChanging_()) {
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
    // If waiting for signal
    if (waitForSignal_) {
        // Coating union ahead and it type
        CoatingUnion* currentCoatingUnion{ CheckForwardCoatingUnion_(world, 0) };
        CoatingType currentCoatingType{ currentCoatingUnion == nullptr ? NoCoatingUnion : currentCoatingUnion->GetType() };

        // If crossroad
        if (currentCoatingType == CrossroadType || currentCoatingType == TCrossroadType) {
            CrossroadObject* crossroad{ static_cast<CrossroadObject*>(currentCoatingUnion) };
            DirectionType from{ AngleToDirection(GetAngle()) };

            SignalType signal{ crossroad->GetControlUnit()->GetSignal(from, AngleTypeToDirection(desiredDirection_)) };
            if (signal == NotWorking || signal == Allowed) {
                ResetDesiredSpeed_();
                waitForSignal_ = false;
            }
        }
    }

    // If cross center of cell
    if (CenterIsCrossed(GetX(), GetY(), GetAngle(), GetLastDelta()) && !IsRotation_() && !IsLineChanging_()) {
        CoatingObject* nextCoating{ CheckForwardCoating_(world, 1) };
        if (nextCoating != nullptr) {
            // If has no forward road coating
            if (GetAngle() != AngleTypeToAngle(desiredDirection_) && nextCoating->IsDirectionAvailable(desiredDirection_)) {
                Rotate_(NormalizeAngle(AngleTypeToAngle(desiredDirection_) - GetAngle()));
            }
            else if (!nextCoating->IsDirectionAvailable(AngleToAngleType(GetAngle()))) {
                if (nextCoating->IsDirectionAvailable(AngleToAngleType(GetAngle() + ANGLE_RIGHT))) {
                    desiredDirection_ = AngleToAngleType(AngleTypeToAngle(desiredDirection_) + ANGLE_RIGHT);
                    Rotate_(ANGLE_RIGHT);
                }
                else if (nextCoating->IsDirectionAvailable(AngleToAngleType(GetAngle() + ANGLE_LEFT))) {
                    desiredDirection_ = AngleToAngleType(AngleTypeToAngle(desiredDirection_) + ANGLE_LEFT);
                    Rotate_(ANGLE_LEFT);
                }
                else {
                    SetDesiredSpeed_(0.f);
                }
            }
        }

        // Coating union under car and ahead
        CoatingUnion* currentCoatingUnion{ CheckForwardCoatingUnion_(world, 0) };
        CoatingUnion* nextCoatingUnion{ CheckForwardCoatingUnion_(world, 1) };
        // Their types
        CoatingType currentCoatingType{ currentCoatingUnion == nullptr ? NoCoatingUnion : currentCoatingUnion->GetType() };
        CoatingType nextCoatingType{ nextCoatingUnion == nullptr ? NoCoatingUnion : nextCoatingUnion->GetType() };

        if (nextCoatingType == CrossroadType  && currentCoatingType != CrossroadType ||
            nextCoatingType == TCrossroadType && currentCoatingType != TCrossroadType) {
            CrossroadObject* crossroad{ static_cast<CrossroadObject*>(nextCoatingUnion) };
            DirectionType from{ AngleToDirection(GetAngle()) };

            if (currentCoatingType == DrivewayType) {
                DrivewayObject* driveway{ static_cast<DrivewayObject*>(currentCoatingUnion) };
                
                bool mustTurn{ false };
                if (crossroad->GetControlUnit()->GetSignal(from, AngleTypeToDirection(desiredDirection_)) == Closed) {
                    mustTurn = true;
                }

                bool rightTurn{ false };
                AngleType rightDirection{ AngleToAngleType(GetAngle() + ANGLE_RIGHT) };
                if (crossroad->GetControlUnit()->GetSignal(from, AngleTypeToDirection(rightDirection)) != Closed) {
                    rightTurn = true;
                }

                bool leftTurn{ false };
                AngleType leftDirection{ AngleToAngleType(GetAngle() + ANGLE_LEFT) };
                if (crossroad->GetControlUnit()->GetSignal(from, AngleTypeToDirection(leftDirection)) != Closed) {
                    leftTurn = true;
                }

                if (mustTurn) {
                    if (!rightTurn) {
                        desiredDirection_ = leftDirection;
                    }
                    else {
                        desiredDirection_ = rightDirection;
                    }
                }
                else {
                    if (driveway->isRightLine(GetX(), GetY()) && rightTurn) {
                        desiredDirection_ = rightDirection;
                    }
                    else if (driveway->isLeftLine(GetX(), GetY()) && leftTurn) {
                        desiredDirection_ = leftDirection;
                    }
                }
            }

            SignalType signal{ crossroad->GetControlUnit()->GetSignal(from, AngleTypeToDirection(desiredDirection_)) };
            if (signal == Warning || signal == Forbidden || signal == Closed) {
                StopAtDistance_(abs(FTA::length(GetX() - nextCoating->GetX(), GetY() - nextCoating->GetY())));
                SetDesiredSpeed_(0.f);
                waitForSignal_ = true;
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