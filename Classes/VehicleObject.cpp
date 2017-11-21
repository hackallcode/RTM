#include "VehicleObject.h"
#include "WorldController.h"
#include "CoatingObject.h"

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
    , rotationAngle_{ 0.f }
    , rotationRadius_{ 0.f }
    , remainingOffset_{ 0.f }
    , remainingOffsetAngle_{ 0.f }
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
    , rotationAngle_{ 0.f }
    , rotationRadius_{ 0.f }
    , remainingOffset_{ 0.f }
    , remainingOffsetAngle_{ 0.f }
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
    , rotationAngle_{ 0.f }
    , rotationRadius_{ 0.f }
    , remainingOffset_{ 0.f }
    , remainingOffsetAngle_{ 0.f }
{
    SetBrakingFactor_(1.f);
    MoveForward_();
}

void rtm::VehicleObject::Update(WorldController* const world)
{
    if (HasCollision()) {
        SetSpeed_(0);
    }
    else {
        BeforeMoving_(world);
        DynamicObject::Update(world);
        AfterMoving_(world);
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
    if (isRotation_ == MustStart || isRotation_ == Started || angle == 0.f) {
        return false;
    }
    else {
        rotationAngle_ = RoundAngle(angle); // Angle of rotation
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

bool rtm::VehicleObject::IsBraking_() const
{
    return brakingDistance_ > 0;
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
    finalSpeed_ = std::min(speed, maxSpeed_);
}

void rtm::VehicleObject::SetBrakingFactor_(float factor)
{
    brakingFactor_ = factor;
}

void rtm::VehicleObject::StopAtDistance_(float distance)
{
    brakingDistance_ = distance;
}

rtm::CoatingObject* rtm::VehicleObject::CheckForwardCoating_(WorldController* const world, int delta)
{
    int col{ PixelToCell(GetX()) };
    int row{ PixelToCell(GetY()) };

    if (delta == 0) {
        if (world->IsCorrectColumn(col) && world->IsCorrectRow(row)) {
            return world->GetCoatingObject(col, row);
        }
    }
    if (SameAngles(GetAngle(), ANGLE_UP)) {
        if (world->IsCorrectColumn(col) && world->IsCorrectRow(row + delta)) {
            return world->GetCoatingObject(col, row + delta);
        }
    }
    else if (SameAngles(GetAngle(), ANGLE_RIGHT)) {
        if (world->IsCorrectColumn(col + delta) && world->IsCorrectRow(row)) {
            return world->GetCoatingObject(col + delta, row);
        }
    }
    else if (SameAngles(GetAngle(), ANGLE_DOWN)) {
        if (world->IsCorrectColumn(col) && world->IsCorrectRow(row - delta)) {
            return world->GetCoatingObject(col, row - delta);
        }
    }
    else if (SameAngles(GetAngle(), ANGLE_LEFT)) {
        if (world->IsCorrectColumn(col - delta) && world->IsCorrectRow(row)) {
            return world->GetCoatingObject(col - delta, row);
        }
    }
    
    return nullptr;
}

rtm::CoatingUnion* rtm::VehicleObject::CheckForwardCoatingUnion_(WorldController* const world, int delta)
{
    int col{ PixelToCell(GetX()) };
    int row{ PixelToCell(GetY()) };

    if (delta == 0) {
        if (world->IsCorrectColumn(col) && world->IsCorrectRow(row)) {
            return world->GetCoatingUnion(col, row);
        }
    }
    else if (SameAngles(GetAngle(), ANGLE_UP)) {
        if (world->IsCorrectColumn(col) && world->IsCorrectRow(row + delta)) {
            return world->GetCoatingUnion(col, row + delta);
        }
    }
    else if (SameAngles(GetAngle(), ANGLE_RIGHT)) {
        if (world->IsCorrectColumn(col + delta) && world->IsCorrectRow(row)) {
            return world->GetCoatingUnion(col + delta, row);
        }
    }
    else if (SameAngles(GetAngle(), ANGLE_DOWN)) {
        if (world->IsCorrectColumn(col) && world->IsCorrectRow(row - delta)) {
            return world->GetCoatingUnion(col, row - delta);
        }
    }
    else if (SameAngles(GetAngle(), ANGLE_LEFT)) {
        if (world->IsCorrectColumn(col - delta) && world->IsCorrectRow(row)) {
            return world->GetCoatingUnion(col - delta, row);
        }
    }

    return nullptr;
}

rtm::DynamicObject* rtm::VehicleObject::CheckForwardArea_(WorldController* const world, float radius, float angle, float angleShift)
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

rtm::DynamicObject* rtm::VehicleObject::CheckMovingArea_(WorldController* const world)
{
    return CheckForwardArea_(world, VIEW_RADIUS, VIEW_ANGLE, VIEW_ANGLE_SHIFT);
}

rtm::DynamicObject* rtm::VehicleObject::CheckRotationArea_(WorldController* const world)
{
    float radius{ ROTATION_VIEW_RADIUS };
    float angle{ ROTATION_VIEW_ANGLE };
    float angleShift{ rotationAngle_ > 0 ? ROTATION_VIEW_ANGLE_SHIFT : -ROTATION_VIEW_ANGLE_SHIFT };

    if (abs(rotationAngle_) < F_PI_4) {
        float factor{ abs(rotationAngle_) / F_PI_4 };

        radius = radius * factor + VIEW_RADIUS * (1 - factor);
        angle = angle * factor + VIEW_ANGLE * (1 - factor);
        angleShift = angleShift * factor + VIEW_ANGLE_SHIFT * (1 - factor);
    }
    
    return CheckForwardArea_(world, radius, angle, angleShift);
}

rtm::DynamicObject* rtm::VehicleObject::CheckTurnArea_(WorldController* const world)
{
    float angleShift{ rotationAngle_ > 0 ? TURN_VIEW_ANGLE_SHIFT : -TURN_VIEW_ANGLE_SHIFT };
    return CheckForwardArea_(world, TURN_VIEW_RADIUS, TURN_VIEW_ANGLE, angleShift);
}

rtm::DynamicObject* rtm::VehicleObject::CheckLineChangingArea_(WorldController* const world)
{
    return CheckForwardArea_(
        world,
        LINE_CHANGING_VIEW_RADIUS,
        LINE_CHANGING_VIEW_ANGLE,
        remainingOffsetAngle_ > GetAngle() ? LINE_CHANGING_VIEW_ANGLE_SHIFT : -LINE_CHANGING_VIEW_ANGLE_SHIFT
    );
}

void rtm::VehicleObject::BeforeMoving_(WorldController* const world)
{
    Movement_(world);
    LineChanging_(world);
    Rotation_(world);
    SpeedChanging_(world);
}

void rtm::VehicleObject::AfterMoving_(WorldController * const world)
{
    SmoothBrakingCounter(world);
}

bool rtm::VehicleObject::MovementStart_(WorldController* const world)
{
    SetFinalSpeed_(maxSpeed_);
    return true;
}

bool rtm::VehicleObject::MovementTick_(WorldController* const world)
{
    return false;
}

bool rtm::VehicleObject::MovementEnd_(WorldController* const world)
{
    SetFinalSpeed_(0.f);
    return true;
}

bool rtm::VehicleObject::RotationStart_(WorldController* const world)
{
    rotationRadius_ = ROTATION_RADIUS - DistanceToSkippedCenter(GetX(), GetY(), GetAngle());
    return true;
}

bool rtm::VehicleObject::RotationTick_(WorldController* const world)
{
    // Angular frequency
    float delta{ rotationRadius_ != 0.f ? GetLastDelta() / rotationRadius_ : rotationAngle_ };
    delta = rotationAngle_ > 0.f ? MIN(delta, rotationAngle_) : MAX(-delta, rotationAngle_);

    // Offets
    SetAngle_(GetAngle() + delta);
    rotationAngle_ -= delta;

    // End
    if (rotationAngle_ == 0.f) {
        SetAngle_(RoundAngle(GetAngle()));

        AngleType angle{ AngleToAngleType(GetAngle()) };
        if (angle == Up || angle == Down) {
            SetX_(RoundToCenter(GetX()));
        }
        else if (angle == Left || angle == Right) {
            SetY_(RoundToCenter(GetY()));
        }
        else {
            SetX_(RoundCoordinate(GetX()));
            SetY_(RoundCoordinate(GetY()));
        }

        return true;
    }

    return false;
}

bool rtm::VehicleObject::RotationEnd_(WorldController* const world)
{
    return true;
}

bool rtm::VehicleObject::LineChangingStart(WorldController* const world)
{
    return true;
}

bool rtm::VehicleObject::LineChangingTick_(WorldController* const world)
{
    // Angle between speed and normal relative to it
    float angleDelta{ remainingOffsetAngle_ - GetAngle() };
    // Normal delta of speed
    float coordsDelta{ abs(FTA::cos(angleDelta) * GetLastDelta()) };

    // Normal offset relative to speed
    remainingOffset_ -= MIN(coordsDelta, remainingOffset_);

    // End
    if (remainingOffset_ == 0.f) {
        remainingOffsetAngle_ = 0.f;
        
        AngleType angle{ AngleToAngleType(GetAngle()) };
        // Hard round
        if (angle == Up || angle == Down) {
            SetX_(RoundToCenter(GetX()));
        }
        else if (angle == Left || angle == Right) {
            SetY_(RoundToCenter(GetY()));
        }
        // Light round
        else {
            SetX_(RoundCoordinate(GetX()));
            SetY_(RoundCoordinate(GetY()));
        }

        return true;
    }

    // Begin
    if (SameCoordinates(remainingOffset_, CELL_SIZE, coordsDelta)) {
        Rotate_(angleDelta > 0.f ? F_PI_4 : -F_PI_4);
    }
    // I want to come back on my way!
    else if (SameCoordinates(remainingOffset_, CELL_SIZE / 4, coordsDelta)) {
        Rotate_(angleDelta > 0.f ? -F_PI_4 : F_PI_4);
    }

    return false;
}

bool rtm::VehicleObject::LineChangingEnd_(WorldController* const world)
{
    return true;
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
    if (IsBraking_()) {
        SetFinalSpeed_(std::min(
            GetFinalSpeed_(),
            std::max(GetMaxSpeed_() * brakingDistance_ / CELL_SIZE, COORD_DELTA * world->GetDeltaTime())
        ));
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
        CoatingObject* coating{ world->GetCoatingObject(col, row) };
        float resistance{ coating != nullptr ? coating->GetResistance() : 1.f };

        SetSpeed_(GetSpeed() - brakingFactor_ * deceleration_ * resistance * world->GetDeltaTime());
        if (GetSpeed() < finalSpeed_) {
            SetSpeed_(finalSpeed_);
        }
    }
}

void rtm::VehicleObject::SmoothBrakingCounter(WorldController * const world)
{
    if (IsBraking_()) {
        brakingDistance_ -= GetLastDelta();
        if (brakingDistance_ < 0.f) {
            brakingDistance_ = 0;
        }
    }
}
