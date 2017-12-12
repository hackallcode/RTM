#include "AllRtmClasses.h"

rtm::CarObject::CarObject()
    : VehicleObject{}
    , recommendedSpeed_{ 0.f }
    , desiredSpeed_{ 0.f }
    , hasDesiredSpeed_{ false }
    , isTurnNear_{ false }
    , waitForSignal_{ false }
    , waitForTurn_{ false }
    , desiredDirection_{ NullAngle }
{}

rtm::CarObject::CarObject(cocos2d::Sprite* const sprite, int column, int row, float angle, float maxSpeed, float acceleration)
    : VehicleObject{ sprite, column, row, angle, maxSpeed, acceleration, CountDeceleration(maxSpeed) }
    , recommendedSpeed_{ GetMaxSpeed_() }
    , desiredSpeed_{ 0.f }
    , hasDesiredSpeed_{ false }
    , isTurnNear_{ false }
    , waitForSignal_{ false }
    , waitForTurn_{ false }
    , desiredDirection_{ AngleToAngleType(angle) }
{}

rtm::CarObject::CarObject(std::string const& filename, int column, int row, float angle, float maxSpeed, float acceleration)
    : VehicleObject{ filename, column, row, angle, maxSpeed, acceleration, CountDeceleration(maxSpeed) }
    , recommendedSpeed_{ GetMaxSpeed_() }
    , desiredSpeed_{ 0.f }
    , hasDesiredSpeed_{ false }
    , isTurnNear_{ false }
    , waitForSignal_{ false }
    , waitForTurn_{ false }
    , desiredDirection_{ AngleToAngleType(angle) }
{}

rtm::CarObject::CarObject(size_t type, int column, int row, float angle)
    : CarObject{
        GetFilename(CAR_FILENAME_MASK, type)
        , column
        , row
        , angle
        , CarObject::GetClassMaxSpeed_(type)
        , CarObject::GetClassAcceleration_(type)
}
{}

bool rtm::CarObject::MovementStart_(WorldController* const world)
{
    return true;
}

bool rtm::CarObject::MovementTick_(WorldController* const world)
{
    if (GetY_() > 1030) {
        int error{ 0 };
    }

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
    DynamicObject* object{ nullptr };
    if (IsMovement_() && !isTurnNear_ && !IsRotation_() || IsLineChanging_()) {
        object = CheckMovingArea_(world);
    }
    else if (isTurnNear_ && !IsRotation_()) {
        object = CheckTurnArea_(world, isRightTurn_);
    }
    else if (IsRotation_()) {
        object = CheckRotationArea_(world);
    }

    if (object != nullptr) {
        float speedAngle{ !isTurnNear_ && !IsRotation_() || IsLineChanging_() ? object->GetAngle() - GetAngle() : 0.f };
        float newSpeed{ object->GetSpeed() * FT::cos(speedAngle) - COORD_DELTA };
        // If forward
        if (newSpeed >= 0.f) {
            SetBrakingFactor_(1.f);
            SetFinalSpeed_(std::min(GetFinalSpeed_(), newSpeed)); // If towards each other
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

void rtm::CarObject::CheckCoatingAhead_(WorldController * const world)
{
    CoatingObject* nextCoating{ CheckForwardCoating_(world, 1) };
    if (nextCoating != nullptr) {
        // If has desired direction
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
}

void rtm::CarObject::CheckCoatingUnionAhead_(WorldController * const world)
{
    // Coating union under car and ahead
    CoatingUnion* currentCoatingUnion{ CheckForwardCoatingUnion_(world, 0) };
    CoatingUnion* nextCoatingUnion{ CheckForwardCoatingUnion_(world, 1) };
    // Their types
    CoatingUnionType currentCoatingType{ currentCoatingUnion == nullptr ? NoCoatingUnion : currentCoatingUnion->GetType() };
    CoatingUnionType nextCoatingType{ nextCoatingUnion == nullptr ? NoCoatingUnion : nextCoatingUnion->GetType() };

    if (currentCoatingUnion != nextCoatingUnion && currentCoatingType == TurnType) {
        isTurnNear_ = false;
    }
    else if (currentCoatingUnion != nextCoatingUnion && nextCoatingType == TurnType) {
        TurnObject* turn{ static_cast<TurnObject*>(nextCoatingUnion) };
        isTurnNear_ = true;
        isRightTurn_ = turn->IsRight();
    }
    else if (currentCoatingUnion != nextCoatingUnion && (nextCoatingType == CrossroadType || nextCoatingType == TCrossroadType)) {
        CrossroadObject* crossroad{ static_cast<CrossroadObject*>(nextCoatingUnion) };
        DirectionType from{ AngleToDirection(GetAngle()) };

        bool mustTurn{ false };
        if (crossroad->GetControlUnit()->GetSignal(from, AngleTypeToDirection(desiredDirection_)) == Closed) {
            mustTurn = true;
        }

        bool hasRightTurn{ false };
        AngleType rightDirection{ AngleToAngleType(GetAngle() + ANGLE_RIGHT) };
        if (crossroad->GetControlUnit()->GetSignal(from, AngleTypeToDirection(rightDirection)) != Closed) {
            hasRightTurn = true;
        }

        bool hasLeftTurn{ false };
        AngleType leftDirection{ AngleToAngleType(GetAngle() + ANGLE_LEFT) };
        if (crossroad->GetControlUnit()->GetSignal(from, AngleTypeToDirection(leftDirection)) != Closed) {
            hasLeftTurn = true;
        }

        if (mustTurn) {
            if (!hasRightTurn) {
                desiredDirection_ = leftDirection;
            }
            else {
                desiredDirection_ = rightDirection;
            }
        }
        else {
            if (currentCoatingType == DrivewayType) {
                DrivewayObject* driveway{ static_cast<DrivewayObject*>(currentCoatingUnion) };

                if (driveway->isRightLine(GetX_(), GetY_()) && hasRightTurn && rand() % 2 == 0) {
                    desiredDirection_ = rightDirection;
                }
                else if (driveway->isLeftLine(GetX_(), GetY_()) && hasLeftTurn && rand() % 2 == 0) {
                    desiredDirection_ = leftDirection;
                }
            }
            else {
                size_t randDirection{ rand() % static_cast<size_t>(1 + (hasRightTurn ? 1 : 0) + (hasLeftTurn ? 1 : 0)) };
                if (hasRightTurn && randDirection == 0) {
                    desiredDirection_ = rightDirection;
                }
                else if (hasLeftTurn && randDirection == 1) {
                    desiredDirection_ = leftDirection;
                }
            }
        }

        SignalType signal{ crossroad->GetControlUnit()->GetSignal(from, AngleTypeToDirection(desiredDirection_)) };
        if (signal == Warning || signal == Forbidden || signal == Closed) {
            StopAtDistance_(DistanceToNextCenter(GetX_(), GetY_(), GetAngle()));
            waitForSignal_ = true;
        }
        else if (signal == NotWorking) {
            if (nextCoatingType == CrossroadType ||
                nextCoatingType == TCrossroadType && crossroad->GetNullDirection() == from) {
                StopAtDistance_(DistanceToNextCenter(GetX_(), GetY_(), GetAngle()));
                waitForTurn_ = true;
            }
        }
    }
}

void rtm::CarObject::CheckRoadAhead_(WorldController* const world)
{
    // If waiting for signal
    if (waitForSignal_) {
        // Stop, if not braking
        if (!hasDesiredSpeed_ && !IsBraking_()) {
            SetDesiredSpeed_(0.f);
        }

        // Coating union ahead and it type
        CoatingUnion* currentCoatingUnion{ CheckForwardCoatingUnion_(world, 0) };
        CoatingUnionType currentCoatingType{ currentCoatingUnion == nullptr ? NoCoatingUnion : currentCoatingUnion->GetType() };

        // If crossroad
        if (currentCoatingType == CrossroadType || currentCoatingType == TCrossroadType) {
            CrossroadObject* crossroad{ static_cast<CrossroadObject*>(currentCoatingUnion) };
            DirectionType from{ AngleToDirection(GetAngle()) };

            SignalType signal{ crossroad->GetControlUnit()->GetSignal(from, AngleTypeToDirection(desiredDirection_)) };
            if (signal == NotWorking || signal == Allowed) {
                ResetDesiredSpeed_();
                StopAtDistance_(0.f);
                waitForSignal_ = false;
            }
        }
    }

    // If waiting for turn
    if (waitForTurn_) {
        if (!IsBraking_()) {
            // Wait
            if (!hasDesiredSpeed_) {
                SetDesiredSpeed_(0.f);
            }
            // If empty, continue
            if (CheckCrossroadArea_(world) == nullptr) {
                ResetDesiredSpeed_();
                waitForTurn_ = false;
            }
        }
    }

    // If cross center of cell
    if (CenterIsCrossed(GetX_(), GetY_(), GetAngle(), GetLastDelta()) && !IsRotation_() && !IsLineChanging_()) {
        CheckCoatingAhead_(world);

        if (!waitForSignal_ && !waitForTurn_) {
            CheckCoatingUnionAhead_(world);
        }
    }
}

float rtm::CarObject::GetClassMaxSpeed_(size_t id)
{
    int number = static_cast<int>(id);
    if (0 <= number && number < static_cast<int>(CARS_MAX_SPEEDS.size())) {
        return CARS_MAX_SPEEDS[number];
    }
    else {
        return CARS_MAX_SPEEDS[0];
    }
}

float rtm::CarObject::GetClassAcceleration_(size_t id)
{
    int number = static_cast<int>(id);
    if (0 <= number && number < static_cast<int>(CARS_ACCELERATIONS.size())) {
        return CARS_ACCELERATIONS[number];
    }
    else {
        return CARS_ACCELERATIONS[0];
    }
}