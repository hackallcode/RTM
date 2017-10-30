#include "WorldController.h"
#include "CoatingObject.h"

rtm::CoatingObject::CoatingObject()
    : CoatingObject{ nullptr, 0.f, 0.f, ANGLE_TOP, 1.f, { true, true, true, true } }
{}

rtm::CoatingObject::CoatingObject(cocos2d::Sprite* const sprite, int column, int row,
    float angle, float resistance, Directions directions)
    : sprite_(nullptr)
    , x_(CellToPixel(column))
    , y_(CellToPixel(row))
    , resistance_(resistance)
    , directions_(directions)
{
    SetSprite_(sprite);

    if (angle == ANGLE_RIGHT) {
        sprite_->setRotation(RAD_DEG * ANGLE_RIGHT);
        std::swap(directions_[0], directions_[3]);
        std::swap(directions_[3], directions_[2]);
        std::swap(directions_[2], directions_[1]);
    }
    else if (angle == ANGLE_BOTTOM) {
        sprite_->setRotation(RAD_DEG * ANGLE_BOTTOM);
        std::swap(directions_[0], directions_[2]);
        std::swap(directions_[1], directions_[3]);
    }
    else if (angle == ANGLE_LEFT) {
        sprite_->setRotation(RAD_DEG * ANGLE_LEFT);
        std::swap(directions_[0], directions_[1]);
        std::swap(directions_[1], directions_[2]);
        std::swap(directions_[2], directions_[3]);
    }
}

rtm::CoatingObject::CoatingObject(std::string const& filename, int column, int row,
    float angle, float resistance, Directions directions)
    : CoatingObject{ cocos2d::Sprite::create(filename), column, row, angle, resistance, directions }
{}

cocos2d::Sprite* rtm::CoatingObject::GetSprite() const
{
    return sprite_;
}

float rtm::CoatingObject::GetX() const
{
    return x_;
}

float rtm::CoatingObject::GetY() const
{
    return y_;
}

float rtm::CoatingObject::GetResistance() const
{
    return resistance_;
}

bool rtm::CoatingObject::HasDirection(float angle) const
{
    if (IsSameAngles(angle, ANGLE_TOP)) {
        return directions_[0];
    }
    else if (IsSameAngles(angle, ANGLE_RIGHT)) {
        return directions_[1];
    }
    else if (IsSameAngles(angle, ANGLE_BOTTOM)) {
        return directions_[2];
    }
    else if (IsSameAngles(angle, ANGLE_LEFT)) {
        return directions_[3];
    }
    else {
        return false;
    }
}

void rtm::CoatingObject::Update(WorldController* const world)
{}

void rtm::CoatingObject::SetSprite_(cocos2d::Sprite* const sprite)
{
    sprite_ = sprite;
    if (sprite_ != nullptr) {
        sprite_->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
        sprite_->setPositionX(x_);
        sprite_->setPositionY(y_);
    }
}

void rtm::CoatingObject::SetX_(float x)
{
    x_ = x;
    if (sprite_ != nullptr) {
        sprite_->setPositionX(x);
    }
}

void rtm::CoatingObject::SetY_(float y)
{
    y_ = y;
    if (sprite_ != nullptr) {
        sprite_->setPositionY(y);
    }
}
