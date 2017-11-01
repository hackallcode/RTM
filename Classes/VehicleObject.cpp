#include "WorldController.h"
#include "VehicleObject.h"

rtm::VehicleObject::VehicleObject()
    : DynamicObject{}
    , wasCollision_{ false }
    , recommendedSpeed_{ 0.f }
    , hasDesiredSpeed_{ false }
    , desiredSpeed_{ 0.f }
    , finalSpeed_{ 0.f }
    , breakingDistance_{ 0.f }
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

rtm::VehicleObject::VehicleObject(cocos2d::Sprite* const sprite, int column, int row,
    float angle, float maxSpeed, float acceleration, float deceleration)
    : DynamicObject{ sprite, CellToPixel(column), CellToPixel(row), angle, 0 }
    , wasCollision_{ false }
    , recommendedSpeed_{ 0.f }
    , hasDesiredSpeed_{ false }
    , desiredSpeed_{ 0.f }
    , finalSpeed_{ 0.f }
    , breakingDistance_{ 0.f }
    , maxSpeed_{ maxSpeed }
    , acceleration_{ acceleration }
    , deceleration_{ deceleration }
    , isMovement_{ NotStarted }
    , isRotation_{ NotStarted }
    , isLineChanging_{ NotStarted }
    , remainingAngle_{ 0.f }
    , remainingOffset_{ 0.f }
    , remainingOffsetAngle_{ 0.f }
{
    MoveForward_();
    SetRecommendedSpeed_(maxSpeed_);
}

rtm::VehicleObject::VehicleObject(std::string const& filename, int column, int row,
    float angle, float maxSpeed, float acceleration, float deceleration)
    : DynamicObject{ filename, CellToPixel(column), CellToPixel(row), angle, 0 }
    , wasCollision_{ false }
    , recommendedSpeed_{ 0.f }
    , hasDesiredSpeed_{ false }
    , desiredSpeed_{ 0.f }
    , finalSpeed_{ 0.f }
    , breakingDistance_{ 0.f }
    , maxSpeed_{ maxSpeed }
    , acceleration_{ acceleration }
    , deceleration_{ deceleration }
    , isMovement_{ NotStarted }
    , isRotation_{ NotStarted }
    , isLineChanging_{ NotStarted }
    , remainingAngle_{ 0.f }
    , remainingOffset_{ 0.f }
    , remainingOffsetAngle_{ 0.f }
{
    MoveForward_();
    SetRecommendedSpeed_(maxSpeed_);
}

void rtm::VehicleObject::Update(WorldController* const world)
{
    if (HasCollision()) {
        if (!wasCollision_) {
            SetSpeed_(0.f);
            Stop_();
            wasCollision_ = true;
        }
    }
    else if (wasCollision_) {
        MoveForward_();
        wasCollision_ = false;
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

void rtm::VehicleObject::SetRecommendedSpeed_(float speed)
{
    recommendedSpeed_ = speed;
}

void rtm::VehicleObject::SetDesiredSpeed_(float speed)
{
    hasDesiredSpeed_ = true;
    desiredSpeed_ = speed;
}

void rtm::VehicleObject::ResetDesiredSpeed_()
{
    hasDesiredSpeed_ = false;
}

void rtm::VehicleObject::SetBreakingDistance_(float distance)
{
    breakingDistance_ = distance;
}

rtm::CoatingObject* const rtm::VehicleObject::GetNextCoating_(WorldController* const world, int delta)
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

void rtm::VehicleObject::CheckRoadAhead_(WorldController* const world)
{
    // If does another movement
    if (isRotation_ != NotStarted || isLineChanging_ != NotStarted) {
        return;
    }

    // If not in center of cell
    if (!IsInCenter(GetX()) || !IsInCenter(GetY())) {
        return;
    }

    CoatingObject* coating{ GetNextCoating_(world, 1) };
    // If no coating ahead
    if (coating == nullptr) {
        return;
    }

    // If has no forward road coating
    if (!coating->HasDirection(GetAngle())) {
        if (coating->HasDirection(NormalizeAngle(GetAngle() + ANGLE_RIGHT))) {
            ResetDesiredSpeed_();
            Rotate_(ANGLE_RIGHT);
        }
        else if (coating->HasDirection(NormalizeAngle(GetAngle() + ANGLE_LEFT))) {
            ResetDesiredSpeed_();
            Rotate_(ANGLE_LEFT);
        }
        else if (coating->HasDirection(NormalizeAngle(GetAngle() + ANGLE_BOTTOM))) {
            ResetDesiredSpeed_();
            Rotate_(ANGLE_BOTTOM);
        }
        else {
            SetDesiredSpeed_(0.f);
        }
    }
    else {
        CoatingObject* fartherCoating{ GetNextCoating_(world, 2) };
        // If no coating ahead
        if (fartherCoating != nullptr) {
            // If T-crossroad
            if (!fartherCoating->HasDirection(GetAngle()) &&
                fartherCoating->HasDirection(NormalizeAngle(GetAngle() + ANGLE_RIGHT)) &&
                fartherCoating->HasDirection(NormalizeAngle(GetAngle() + ANGLE_LEFT))) {
                SetBreakingDistance_( abs(FT::length(GetX() - fartherCoating->GetX(), GetY() - fartherCoating->GetY()) - CELL_SIZE) );
            }
        }
    }
}

rtm::DynamicObject* const rtm::VehicleObject::CanMoveForward_(WorldController* const world)
{
    DynamicObject* object{ nullptr };
    for (auto& obj : world->GetDynamicObjects()) {
        if (IsBeholding_(
            obj.get()
            , VIEW_RADIUS
            , VIEW_ANGLE
            , VIEW_ANGLE_SHIFT
        )) {
            object = obj.get();
            break;
        }
    }
    return object;
}

rtm::DynamicObject* const rtm::VehicleObject::CanRotate_(WorldController* const world)
{
    DynamicObject* object{ nullptr };
    for (auto& obj : world->GetDynamicObjects()) {
        if (IsBeholding_(
            obj.get()
            , ROTATION_VIEW_RADIUS
            , ROTATION_VIEW_ANGLE
            , ROTATION_VIEW_ANGLE_SHIFT * (remainingAngle_ < 0 ? 1 : -1)
        )) {
            object = obj.get();
            break;
        }
    }
    return object;
}

rtm::DynamicObject* const rtm::VehicleObject::CanChangeLine_(WorldController* const world)
{
    DynamicObject* object{ nullptr };
    for (auto& obj : world->GetDynamicObjects()) {
        if (IsBeholding_(
            obj.get()
            , LINE_CHANGING_VIEW_RADIUS
            , LINE_CHANGING_VIEW_ANGLE
            , LINE_CHANGING_VIEW_ANGLE_SHIFT * (remainingOffsetAngle_ - GetAngle() > 0 ? 1 : -1)
        )) {
            object = obj.get();
            break;
        }
    }
    return object;
}

void rtm::VehicleObject::Move_(WorldController* const world)
{
    Breaking_(world);
    LineChanging_(world);
    Rotation_(world);
    Movement_(world);
    Acceleration_(world);
}

void rtm::VehicleObject::Acceleration_(WorldController* const world)
{
    if (GetSpeed() < finalSpeed_) {
        SetSpeed_(GetSpeed() + acceleration_ * world->GetDeltaTime());
        if (GetSpeed() > finalSpeed_) {
            SetSpeed_(finalSpeed_);
        }
    }
    else if (GetSpeed() > finalSpeed_) {
        int col{ PixelToCell(GetX()) };
        int row{ PixelToCell(GetY()) };
        SetSpeed_(GetSpeed() - deceleration_ * world->GetCoating(col, row)->GetResistance() * world->GetDeltaTime());
        if (GetSpeed() < finalSpeed_) {
            SetSpeed_(finalSpeed_);
        }
    }
}

void rtm::VehicleObject::Movement_(WorldController * const world)
{
    if (isMovement_ == MustStart) {
        isMovement_ = Started;
    }
    if (isMovement_ == Started) {
        // Check roads
        CheckRoadAhead_(world);

        // Set speed
        if (hasDesiredSpeed_) {
            finalSpeed_ = desiredSpeed_;
        }
        else {
            finalSpeed_ = recommendedSpeed_;
        }

        // Check other vehicles
        DynamicObject* object{ CanMoveForward_(world) };
        if (object != nullptr) {
            float newSpeed{ object->GetSpeed() * FT::cos(object->GetAngle() - GetAngle()) };
            newSpeed = newSpeed > 0 ? newSpeed : 0;
            finalSpeed_ = newSpeed < finalSpeed_ ? newSpeed : finalSpeed_; // If towards each other
        }
    }
    if (isMovement_ == MustStop) {
        finalSpeed_ = 0.f;
        isMovement_ = NotStarted;
    }
}

void rtm::VehicleObject::Breaking_(WorldController* const world)
{
    if (breakingDistance_ > 0.f) {
        breakingDistance_ -= GetSpeed() * world->GetDeltaTime();
        SetDesiredSpeed_(breakingDistance_);
        if (breakingDistance_ < 0.f || IsSameCoords(breakingDistance_, 0.f)) {
            SetDesiredSpeed_(0.f);
            SetX_(GetX() + breakingDistance_ * sin(GetAngle()));
            SetY_(GetY() + breakingDistance_ * cos(GetAngle()));
            breakingDistance_ = 0.f;
        }
    }
}

void rtm::VehicleObject::Rotation_(WorldController* const world)
{
    if (isRotation_ == MustStart) {
        DynamicObject* object{ CanRotate_(world) };
        if (object == nullptr) {
            ResetDesiredSpeed_();
            isRotation_ = Started;
        }
        else {
            SetDesiredSpeed_(0.f);
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
            SetX_(RoundCoord(GetX(), 2 * COORD_DELTA));
            SetY_(RoundCoord(GetY(), 2 * COORD_DELTA));
            SetAngle_(RoundAngle(GetAngle()));
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
        DynamicObject* object{ CanChangeLine_(world) };
        if (object == nullptr) {
            ResetDesiredSpeed_();
            isLineChanging_ = Started;
        }
        else {
            SetDesiredSpeed_(object->GetSpeed() / 2);
        }
    }
    if (isLineChanging_ == Started) {
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
            isLineChanging_ = MustStop;
        }
    }
    if (isLineChanging_ == MustStop) {
        isLineChanging_ = NotStarted;
    }
}
