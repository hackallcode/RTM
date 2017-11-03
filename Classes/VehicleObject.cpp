#include "WorldController.h"
#include "VehicleObject.h"

rtm::VehicleObject::VehicleObject()
    : DynamicObject{}
    , isMovement_{ NotStarted }
    , isRotation_{ NotStarted }
    , isLineChanging_{ NotStarted }
    , maxSpeed_{ 0.f }
    , acceleration_{ 0.f }
    , deceleration_{ 0.f }
    , finalSpeed_{ 0.f }
    , brakingFactor_{ 0.f }
    , brakingDistance_{ 0.f }
    , remainingAngle_{ 0.f }
    , remainingOffset_{ 0.f }
    , remainingOffsetAngle_{ 0.f }
    , wasCollision_{ false }
{}

rtm::VehicleObject::VehicleObject(cocos2d::Sprite* const sprite, int column, int row, float angle,
    float maxSpeed, float acceleration, float deceleration)
    : DynamicObject{ sprite, CellToPixel(column), CellToPixel(row), angle, 0 }
    , isMovement_{ NotStarted }
    , isRotation_{ NotStarted }
    , isLineChanging_{ NotStarted }
    , maxSpeed_{ maxSpeed }
    , acceleration_{ acceleration }
    , deceleration_{ deceleration }
    , finalSpeed_{ 0.f }
    , brakingFactor_{ 0.f }
    , brakingDistance_{ 0.f }
    , remainingAngle_{ 0.f }
    , remainingOffset_{ 0.f }
    , remainingOffsetAngle_{ 0.f }
    , wasCollision_{ false }
{
    SetBrakingFactor_(1.f);
    MoveForward_();
}

rtm::VehicleObject::VehicleObject(std::string const& filename, int column, int row, float angle,
    float maxSpeed, float acceleration, float deceleration)
    : DynamicObject{ filename, CellToPixel(column), CellToPixel(row), angle, 0 }
    , isMovement_{ NotStarted }
    , isRotation_{ NotStarted }
    , isLineChanging_{ NotStarted }
    , maxSpeed_{ maxSpeed }
    , acceleration_{ acceleration }
    , deceleration_{ deceleration }
    , finalSpeed_{ 0.f }
    , brakingFactor_{ 0.f }
    , brakingDistance_{ 0.f }
    , remainingAngle_{ 0.f }
    , remainingOffset_{ 0.f }
    , remainingOffsetAngle_{ 0.f }
    , wasCollision_{ false }
{
    SetBrakingFactor_(1.f);
    MoveForward_();
}

void rtm::VehicleObject::Update(WorldController* const world)
{
    if (HasCollision()) {
        if (!wasCollision_) {
            wasCollision_ = true;
            Stop_();
            SetSpeed_(0.f);
        }
    }
    else if (wasCollision_) {
        wasCollision_ = false;
        MoveForward_();
    }

    Move_(world);
    DynamicObject::Update(world);
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
        remainingAngle_ = RoundAngle(angle); // Angle of rotation
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

bool rtm::VehicleObject::IsMovement_() const
{
    return isMovement_ != NotStarted;
}

bool rtm::VehicleObject::IsRotation_() const
{
    return isRotation_ != NotStarted;
}

bool rtm::VehicleObject::IsLineChanging_() const
{
    return isLineChanging_ != NotStarted;
}

float rtm::VehicleObject::GetMaxSpeed_() const
{
    return maxSpeed_;
}

float rtm::VehicleObject::GetFinalSpeed_() const
{
    return finalSpeed_;
}

void rtm::VehicleObject::SetFinalSpeed_(float speed)
{
    finalSpeed_ = min(speed, maxSpeed_);
}

void rtm::VehicleObject::SetBrakingFactor_(float factor)
{
    brakingFactor_ = factor;
}

void rtm::VehicleObject::StopAtDistance_(float distance)
{
    brakingDistance_ = distance;
}

rtm::CoatingObject* const rtm::VehicleObject::CheckForwardCoating_(WorldController* const world, int delta)
{
    int col{ PixelToCell(GetX()) };
    int row{ PixelToCell(GetY()) };

    if (IsSameAngles(GetAngle(), ANGLE_TOP)) {
        return world->GetCoating(col, row + delta).get();
    }
    else if (IsSameAngles(GetAngle(), ANGLE_RIGHT)) {
        return world->GetCoating(col + delta, row).get();
    }
    else if (IsSameAngles(GetAngle(), ANGLE_BOTTOM)) {
        return world->GetCoating(col, row - delta).get();
    }
    else if (IsSameAngles(GetAngle(), ANGLE_LEFT)) {
        return world->GetCoating(col - delta, row).get();
    }
    else {
        return nullptr;
    }
}

rtm::DynamicObject * const rtm::VehicleObject::CheckForwardArea_(WorldController * const world, float radius, float angle, float angleShift)
{
    DynamicObject* object{ nullptr };
    for (auto& obj : world->GetDynamicObjects()) {
        if (IsBeholding_(obj.get(), radius, angle, angleShift)) {
            object = obj.get();
            break;
        }
    }
    return object;
}

rtm::DynamicObject* const rtm::VehicleObject::CheckMovingArea_(WorldController* const world)
{
    return CheckForwardArea_(
        world,
        VIEW_RADIUS,
        VIEW_ANGLE,
        VIEW_ANGLE_SHIFT
    );
}

rtm::DynamicObject* const rtm::VehicleObject::CheckRotationArea_(WorldController* const world)
{
    return CheckForwardArea_(
        world,
        ROTATION_VIEW_RADIUS,
        ROTATION_VIEW_ANGLE,
        remainingAngle_ < 0 ? ROTATION_VIEW_ANGLE_SHIFT : -ROTATION_VIEW_ANGLE_SHIFT
    );
}

rtm::DynamicObject* const rtm::VehicleObject::CheckLineChangingArea_(WorldController* const world)
{
    return CheckForwardArea_(
        world,
        LINE_CHANGING_VIEW_RADIUS,
        LINE_CHANGING_VIEW_ANGLE,
        remainingOffsetAngle_ > GetAngle() ? LINE_CHANGING_VIEW_ANGLE_SHIFT : -LINE_CHANGING_VIEW_ANGLE_SHIFT
    );
}

bool rtm::VehicleObject::MovementStart_(WorldController* const world)
{
    SetFinalSpeed_(maxSpeed_);
    return true;
}

bool rtm::VehicleObject::MovementTick_(WorldController * const world)
{
    return false;
}

bool rtm::VehicleObject::MovementEnd_(WorldController * const world)
{
    SetFinalSpeed_(0.f);
    return true;
}

bool rtm::VehicleObject::RotationStart_(WorldController* const world)
{
    return true;
}

bool rtm::VehicleObject::RotationTick_(WorldController * const world)
{
    // Angular frequency
    float delta{ world->GetDeltaTime() * GetSpeed() / CELL_SIZE };
    delta = remainingAngle_ > 0.f ? MIN(delta, remainingAngle_) : MAX(-delta, remainingAngle_);

    // Offets
    SetAngle_(GetAngle() + delta);
    remainingAngle_ -= delta;

    // End
    if (remainingAngle_ == 0.f) {
        SetX_(RoundCoord(GetX(), 2 * COORD_DELTA));
        SetY_(RoundCoord(GetY(), 2 * COORD_DELTA));
        SetAngle_(RoundAngle(GetAngle()));
        return true;
    }

    return false;
}

bool rtm::VehicleObject::RotationEnd_(WorldController * const world)
{
    return true;
}

bool rtm::VehicleObject::LineChangingStart(WorldController* const world)
{
    return true;
}

bool rtm::VehicleObject::LineChangingTick_(WorldController * const world)
{
    // Angle between speed and normal relative to it
    float delta{ remainingOffsetAngle_ - GetAngle() };

    // Begin
    if (IsSameCoords(remainingOffset_, CELL_SIZE)) {
        Rotate_(delta > 0.f ? F_PI_4 : -F_PI_4);
    }
    // I want to come back on my way!
    else if (IsSameCoords(remainingOffset_, CELL_SIZE / 4)) {
        Rotate_(delta > 0.f ? -F_PI_4 : F_PI_4);
    }

    // Normal offset relative to speed
    remainingOffset_ -= MIN(abs(FTA::cos(delta) * GetSpeed() * world->GetDeltaTime()), remainingOffset_);

    // End
    if (remainingOffset_ == 0.f) {
        remainingOffsetAngle_ = 0.f;
        SetX_(RoundCoord(GetX(), 2 * COORD_DELTA));
        SetY_(RoundCoord(GetY(), 2 * COORD_DELTA));
        return true;
    }

    return false;
}

bool rtm::VehicleObject::LineChangingEnd_(WorldController * const world)
{
    return true;
}

void rtm::VehicleObject::Move_(WorldController* const world)
{
    LineChanging_(world);
    Rotation_(world);
    Movement_(world);
    SpeedChanging_(world);
}

void rtm::VehicleObject::LineChanging_(WorldController* const world)
{
    if (isLineChanging_ == MustStart) {
        if (LineChangingStart(world)) {
            isLineChanging_ = Started;
        }
    }
    if (isLineChanging_ == Started) {
        if (LineChangingTick_(world)) {
            isLineChanging_ = MustStop;
        }
    }
    if (isLineChanging_ == MustStop) {
        if (LineChangingEnd_(world)) {
            isLineChanging_ = NotStarted;
        }
    }
}

void rtm::VehicleObject::Rotation_(WorldController* const world)
{
    if (isRotation_ == MustStart) {
        if (RotationStart_(world)) {
            isRotation_ = Started;
        }
    }
    if (isRotation_ == Started) {
        if (RotationTick_(world)) {
            isRotation_ = MustStop;
        }
    }
    if (isRotation_ == MustStop) {
        if (RotationEnd_(world)) {
            isRotation_ = NotStarted;
        }
    }
}

void rtm::VehicleObject::Movement_(WorldController* const world)
{
    if (isMovement_ == MustStart) {
        if (MovementStart_(world)) {
            isMovement_ = Started;
        }
    }
    if (isMovement_ == Started) {
        if (MovementTick_(world)) {
            isMovement_ = MustStop;
        }
    }
    if (isMovement_ == MustStop) {
        if (MovementEnd_(world)) {
            isMovement_ = NotStarted;
        }
    }
}

void rtm::VehicleObject::SpeedChanging_(WorldController* const world)
{
    // Smooth braking
    if (brakingDistance_ > 0.f) {
        SetFinalSpeed_(brakingDistance_);
        if (brakingDistance_ < 0.f || IsSameCoords(brakingDistance_, 0.f)) {
            SetX_(GetX() + brakingDistance_ * sin(GetAngle()));
            SetY_(GetY() + brakingDistance_ * cos(GetAngle()));
            brakingDistance_ = 0.f;
            SetFinalSpeed_(0.f);
        }
    }
    // Acceleration
    if (GetSpeed() < finalSpeed_) {
        SetSpeed_(GetSpeed() + acceleration_ * world->GetDeltaTime());
        if (GetSpeed() > finalSpeed_) {
            SetSpeed_(finalSpeed_);
        }
    }
    // Deceleration
    else if (GetSpeed() > finalSpeed_) {
        int col{ PixelToCell(GetX()) };
        int row{ PixelToCell(GetY()) };
        SetSpeed_(GetSpeed() - brakingFactor_ * deceleration_ * world->GetCoating(col, row)->GetResistance() * world->GetDeltaTime());
        if (GetSpeed() < finalSpeed_) {
            SetSpeed_(finalSpeed_);
        }
    }
    // Smooth braking counter
    if (brakingDistance_ > 0.f) {
        brakingDistance_ -= GetSpeed() * world->GetDeltaTime();
    }
}