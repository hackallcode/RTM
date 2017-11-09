#include "CoatingObject.h"

rtm::CoatingObject::CoatingObject()
    : CoatingObject{ nullptr, 0.f, 0.f, TopDirection, 0.f, { false, false, false, false } }
{}

rtm::CoatingObject::CoatingObject(cocos2d::Sprite* const sprite, int column, int row,
    DirectionType direction, float resistance, Directions directions)
    : sprite_(nullptr)
    , x_(CellToPixel(column))
    , y_(CellToPixel(row))
    , resistance_(resistance)
    , directions_(directions)
{
    SetSprite_(sprite);

    if (direction == RightDirection) {
        sprite_->setRotation(RAD_DEG * ANGLE_RIGHT);
        
        std::swap(directions_[TopDirection], directions_[LeftDirection]);
        std::swap(directions_[LeftDirection], directions_[BottomDirection]);
        std::swap(directions_[BottomDirection], directions_[RightDirection]);

        std::swap(directions_[TopRightDirection], directions_[TopLeftDirection]);
        std::swap(directions_[TopLeftDirection], directions_[BottomLeftDirection]);
        std::swap(directions_[BottomLeftDirection], directions_[BottomRightDirection]);
    }
    else if (direction == BottomDirection) {
        sprite_->setRotation(RAD_DEG * ANGLE_BOTTOM);

        std::swap(directions_[TopDirection], directions_[BottomDirection]);
        std::swap(directions_[RightDirection], directions_[LeftDirection]);

        std::swap(directions_[TopRightDirection], directions_[BottomLeftDirection]);
        std::swap(directions_[BottomRightDirection], directions_[TopLeftDirection]);
    }
    else if (direction == LeftDirection) {
        sprite_->setRotation(RAD_DEG * ANGLE_LEFT);

        std::swap(directions_[TopDirection], directions_[RightDirection]);
        std::swap(directions_[RightDirection], directions_[BottomDirection]);
        std::swap(directions_[BottomDirection], directions_[LeftDirection]);

        std::swap(directions_[TopRightDirection], directions_[BottomRightDirection]);
        std::swap(directions_[BottomRightDirection], directions_[BottomLeftDirection]);
        std::swap(directions_[BottomLeftDirection], directions_[TopLeftDirection]);
    }

    availableDirections_ = directions_;
}

rtm::CoatingObject::CoatingObject(std::string const& filename, int column, int row,
    DirectionType direction, float resistance, Directions directions)
    : CoatingObject{ cocos2d::Sprite::create(filename), column, row, direction, resistance, directions }
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

bool rtm::CoatingObject::HasDirection(DirectionType direction) const
{
    return directions_[direction];
}

bool rtm::CoatingObject::IsDirectionAvailable(DirectionType direction) const
{
    return availableDirections_[direction];
}

void rtm::CoatingObject::SetDirectionAvailability(DirectionType direction, bool status)
{
    availableDirections_[direction] = status;
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
