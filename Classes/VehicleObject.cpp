#include "WorldController.h"
#include "VehicleObject.h"

rtm::VehicleObject::VehicleObject()
    : DynamicObject{}
    , speed_{ 0.f }
    , maxSpeed_{ 0.f }
    , acceleration_{ 0.f }
    , deceleration_{ 0.f }
    , isMovement_{ NotStarted }
    , isRotation_{ NotStarted }
    , isLineChanging_{ NotStarted }
    , remainingAngle_{ 0.f }
    , remainingOffset_{ 0.f }
    , remainingOffsetAngle_{ 0.f }
{}

rtm::VehicleObject::VehicleObject(std::string const& filename, int column, int row, float angle, float maxSpeed, float acceleration, float deceleration)
    : DynamicObject(filename, WorldController::Col2X(column), WorldController::Row2Y(row), angle, 0)
    , speed_{ 0.f }
    , maxSpeed_{ maxSpeed }
    , acceleration_{ acceleration }
    , deceleration_{ deceleration }
    , isMovement_{ MustStart }
    , isRotation_{ NotStarted }
    , isLineChanging_{ NotStarted }
    , remainingAngle_{ 0.f }
    , remainingOffset_{ 0.f }
    , remainingOffsetAngle_{ 0.f }
{}

void rtm::VehicleObject::Update(WorldController* const world)
{
    if (HasCollision()) {
        MaintainSpeed_(0.f);
    }
    else {
        Move_(world);
        DynamicObject::Update(world);
    }    
}

bool rtm::VehicleObject::MoveForward_()
{
    if (isMovement_ == MustStart || isMovement_ == Started) {
        return false;
    }
    else {
        isMovement_ = MustStart;
        return true;
    }
}

bool rtm::VehicleObject::Stop_()
{
    if (isMovement_ == MustStop || isMovement_ == NotStarted) {
        return false;
    }
    else {
        isMovement_ = MustStop;
        return true;
    }
}

bool rtm::VehicleObject::Rotate_(float angle)
{
    if (isRotation_ == MustStart || isRotation_ == Started) {
        return false;
    }
    else {
        remainingAngle_ = RoundAngle_(angle); // Angle of rotation
        isRotation_ = MustStart;
        return true;
    }
}

bool rtm::VehicleObject::ChangeLine_(bool isRight)
{
    if (isLineChanging_ == MustStart || isLineChanging_ == Started) {
        return false;
    }
    else {
        remainingOffset_ = CELL_SIZE; // Length of normal relative to speed
        remainingOffsetAngle_ = isRight ? GetAngle() + F_PI_2 : GetAngle() - F_PI_2; // Angle of normal relative to speed
        isLineChanging_ = MustStart;
        return true;
    }
}

void rtm::VehicleObject::MaintainSpeed_(float speed)
{
    speed_ = speed;
}

void rtm::VehicleObject::Move_(WorldController* const world)
{
    LineChanging_(world);
    Rotation_(world);
    Movement_(world);
    Acceleration_(world->GetDeltaTime());
}

void rtm::VehicleObject::Acceleration_(float deltaTime)
{
    if (GetSpeed() < speed_) {
        SetSpeed_(GetSpeed() + acceleration_ * deltaTime);
        if (GetSpeed() > speed_) {
            SetSpeed_(speed_);
        }
    }
    else if (GetSpeed() > speed_) {
        SetSpeed_(GetSpeed() - deceleration_ * deltaTime);
        if (GetSpeed() < speed_) {
            SetSpeed_(speed_);
        }
    }
}

void rtm::VehicleObject::Movement_(WorldController * const world)
{
    if (isMovement_ == MustStart) {
        MaintainSpeed_(maxSpeed_);
        isMovement_ = Started;
    }
    if (isMovement_ == Started) {
        DynamicObject* object{ nullptr };
        for (auto& obj : world->GetDynamicObjects()) {
            if (IsBeholding_(
                obj.get()
                , VIEW_RADIUS
                , VIEW_ANGLE
                , VIEW_ANGLE_SHIFT * (remainingOffsetAngle_ - GetAngle() > 0 ? 1 : -1)
            )) {
                object = obj.get();
                break;
            }
        }
        
        if (object == nullptr) {
            MaintainSpeed_(maxSpeed_);
        }
        else {
            float newSpeed = object->GetSpeed() * FT::cos(object->GetAngle() - GetAngle());
            MaintainSpeed_(newSpeed > 0 ? newSpeed : 0); // If towards each other
        }
    }
    if (isMovement_ == MustStop) {
        MaintainSpeed_(0.f);
        isMovement_ = NotStarted;
    }
}

void rtm::VehicleObject::Rotation_(WorldController* const world)
{
    if (isRotation_ == MustStart) {
        bool isBeholding{ false };
        for (auto& obj : world->GetDynamicObjects()) {
            if (IsBeholding_(
                obj.get()
                , ROTATION_VIEW_RADIUS
                , ROTATION_VIEW_ANGLE
                , ROTATION_VIEW_ANGLE_SHIFT * (remainingOffsetAngle_ - GetAngle() > 0 ? 1 : -1)
            )) {
                isBeholding = true;
                break;
            }
        }
        if (!isBeholding) {
            MoveForward_();
            isRotation_ = Started;
        }
        else {
            SetSpeed_(0.f); // KOCTblJIb
            Stop_();
        }
    }
    if (isRotation_ == Started) {
        // Angular frequency
        float delta{ world->GetDeltaTime() * GetSpeed() / CELL_SIZE };
        delta = remainingAngle_ > 0.f ? MIN(delta, remainingAngle_) : MAX(-delta, remainingAngle_);

        // Offets
        SetAngle_(GetAngle() + delta);
        remainingAngle_ -= delta;

        // End
        if (remainingAngle_ == 0.f) {
            SetX_(RoundCoord_(GetX(), 2 * COORD_DELTA));
            SetY_(RoundCoord_(GetY(), 2 * COORD_DELTA));
            isRotation_ = MustStop;
        }
    }
    if (isRotation_ == MustStop) {
        isRotation_ = NotStarted;
    }
}

void rtm::VehicleObject::LineChanging_(WorldController* const world)
{
    if (isLineChanging_ == MustStart) {
        bool isBeholding{ false };
        for (auto& obj : world->GetDynamicObjects()) {
            if (IsBeholding_(
                obj.get()
                , LINE_CHANGING_VIEW_RADIUS
                , LINE_CHANGING_VIEW_ANGLE
                , LINE_CHANGING_VIEW_ANGLE_SHIFT * (remainingOffsetAngle_ - GetAngle() > 0 ? 1 : -1)
            )) {
                isBeholding = true;
                break;
            }
        }
        if (!isBeholding) {
            MoveForward_();
            isLineChanging_ = Started;
        }
        else {
            Stop_();
        }
    }
    if (isLineChanging_ == Started) {
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
        remainingOffset_ -= MIN(abs(FTA::cos(delta) * GetSpeed() * world->GetDeltaTime()), remainingOffset_);

        // End
        if (remainingOffset_ == 0.f) {
            remainingOffsetAngle_ = 0.f;
            SetX_(RoundCoord_(GetX(), 2 * COORD_DELTA));
            SetY_(RoundCoord_(GetY(), 2 * COORD_DELTA));
            isLineChanging_ = MustStop;
        }
    }
    if (isLineChanging_ == MustStop) {
        isLineChanging_ = NotStarted;
    }
}
