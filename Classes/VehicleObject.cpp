#include "WorldController.h"
#include "VehicleObject.h"

rtm::VehicleObject::VehicleObject()
    : DynamicObject{}
    , maxSpeed_{ 0.f }
    , acceleration_{ 0.f }
    , deceleration_{ 0.f }
    , isRotation_{ false }
    , isLineChanging_{ false }
    , remainingAngle_{ 0.f }
    , remainingOffset_{ 0.f }
    , remainingOffsetAngle_{ 0.f }
{}

rtm::VehicleObject::VehicleObject(std::string const& filename, int column, int row, float angle, float maxSpeed, float acceleration, float deceleration)
    : DynamicObject(filename, WorldController::Col2X(column), WorldController::Row2Y(row), angle, 0)
    , maxSpeed_{ maxSpeed }
    , acceleration_{ acceleration }
    , deceleration_{ deceleration }
    , isRotation_{ false }
    , isLineChanging_{ false }
    , remainingAngle_{ 0.f }
    , remainingOffset_{ 0.f }
    , remainingOffsetAngle_{ 0.f }
{}

void rtm::VehicleObject::Update(WorldController* const world)
{
    if (HasCollision()) {
        return;
    }

    bool IsBeholding{ false };
    switch (world->caseNum) {
    case 1:
        for (auto& obj : world->GetDynamicObjects()) {
            if (IsBeholding_(
                obj.get()
                , VIEW_RADIUS
                , VIEW_ANGLE
                , VIEW_ANGLE_SHIFT * (remainingOffsetAngle_ - GetAngle() > 0 ? 1 : -1)
            )) {
                IsBeholding = true;
                break;
            }
        }
        break;
    case 2:
        for (auto& obj : world->GetDynamicObjects()) {
            if (IsBeholding_(
                obj.get()
                , VIEW_RADIUS
                , VIEW_ANGLE
                , VIEW_ANGLE_SHIFT * (remainingOffsetAngle_ - GetAngle() > 0 ? 1 : -1)
            )) {
                IsBeholding = true;
                break;
            }
        }
        break;
    case 3:
        if (GetX() > 15.5 * CELL_SIZE && (
            IsSameCoords_(GetY(), 10.5 * CELL_SIZE) || IsSameCoords_(GetY(), 20.5 * CELL_SIZE)
            )) {
            for (auto& obj : world->GetDynamicObjects()) {
                if (IsBeholding_(
                    obj.get()
                    , LINE_CHANGING_VIEW_RADIUS
                    , LINE_CHANGING_VIEW_ANGLE
                    , LINE_CHANGING_VIEW_ANGLE_SHIFT * (remainingOffsetAngle_ - GetAngle() > 0 ? 1 : -1)
                )) {
                    IsBeholding = true;
                    break;
                }
            }
            if (!IsBeholding) ChangeLine_(LEFT);
        }
        break;
    case 4:
        if (GetX() > 15.5 * CELL_SIZE && (
            IsSameCoords_(GetY(), 11.5 * CELL_SIZE) || IsSameCoords_(GetY(), 21.5 * CELL_SIZE)
            )) {
            for (auto& obj : world->GetDynamicObjects()) {
                if (IsBeholding_(
                    obj.get()
                    , LINE_CHANGING_VIEW_RADIUS
                    , LINE_CHANGING_VIEW_ANGLE
                    , LINE_CHANGING_VIEW_ANGLE_SHIFT * (remainingOffsetAngle_ - GetAngle() > 0 ? 1 : -1)
                )) {
                    IsBeholding = true;
                    break;
                }
            }
            if (!IsBeholding) ChangeLine_(RIGHT);
        }
        break;
    case 5:
        if (IsSameCoords_(GetY(), 14.5 * CELL_SIZE)) {
            for (auto& obj : world->GetDynamicObjects()) {
                if (IsBeholding_(
                    obj.get()
                    , ROTATION_VIEW_RADIUS
                    , ROTATION_VIEW_ANGLE
                    , ROTATION_VIEW_ANGLE_SHIFT * (remainingOffsetAngle_ - GetAngle() > 0 ? 1 : -1)
                )) {
                    IsBeholding = true;
                    break;
                }
            }
            if (IsBeholding) {
                SetSpeed_(0.f);
            }
            else {
                Rotate_(ANGLE_RIGHT);
            }
        }
        break;
    case 6:
        if (IsSameCoords_(GetY(), 14.5 * CELL_SIZE)) {
            for (auto& obj : world->GetDynamicObjects()) {
                if (IsBeholding_(
                    obj.get()
                    , ROTATION_VIEW_RADIUS
                    , ROTATION_VIEW_ANGLE
                    , ROTATION_VIEW_ANGLE_SHIFT * (remainingOffsetAngle_ - GetAngle() > 0 ? 1 : -1)
                )) {
                    IsBeholding = true;
                    break;
                }
            }
            if (IsBeholding) {
                SetSpeed_(0.f);
            }
            else {
                Rotate_(ANGLE_LEFT);
            }
        }
        break;
    }
    if (IsBeholding) {
        Decelerate_(world->GetDeltaTime());
    }
    else {
        Accelerate_(world->GetDeltaTime());
    }

    Move_(world->GetDeltaTime());
    DynamicObject::Update(world);
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

bool rtm::VehicleObject::Rotate_(float angle)
{
    if (isRotation_) {
        return false;
    }
    else {
        remainingAngle_ = RoundAngle_(angle); // Angle of rotation
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
        remainingOffset_ = CELL_SIZE; // Length of normal relative to speed
        remainingOffsetAngle_ = isRight ? GetAngle() + F_PI_2 : GetAngle() - F_PI_2; // Angle of normal relative to speed
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
        // Angular frequency
        float delta{ deltaTime * GetSpeed() / CELL_SIZE };
        delta = remainingAngle_ > 0.f ? MIN(delta, remainingAngle_) : MAX(-delta, remainingAngle_);

        // Offets
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
        float delta{ remainingOffsetAngle_ - GetAngle() };

        // Begin
        if (IsSameCoords_(remainingOffset_, CELL_SIZE)) {
            Rotate_(delta > 0.f ? F_PI_4 : -F_PI_4);
        }
        // I want to come back on my way!
        else if (IsSameCoords_(remainingOffset_, CELL_SIZE / 4)) {
            Rotate_(delta > 0.f ? -F_PI_4 : F_PI_4);
        }

        // Normal offset relative to speed
        remainingOffset_ -= MIN(abs(FTA::cos(delta) * GetSpeed() * deltaTime), remainingOffset_);

        // End
        if (remainingOffset_ == 0.f) {
            isLineChanging_ = false;
            remainingOffsetAngle_ = 0.f;
        }
    }
}
