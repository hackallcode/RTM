#include "CoatingObject.h"

rtm::CoatingObject::CoatingObject()
    : CoatingObject{ nullptr, 0.f, 0.f, Up, 0.f, { false, false, false, false } }
{}

rtm::CoatingObject::CoatingObject(cocos2d::Sprite* const sprite, int column, int row,
    AngleType angle, float resistance, Directions directions)
    : sprite_(nullptr)
    , x_(CellToPixel(column))
    , y_(CellToPixel(row))
    , resistance_(resistance)
    , directions_(directions)
{
    SetSprite_(sprite);

    if (angle == Right) {
        sprite_->setRotation(RAD_DEG * ANGLE_RIGHT);
        
        std::swap(directions_[Up], directions_[Left]);
        std::swap(directions_[Left], directions_[Down]);
        std::swap(directions_[Down], directions_[Right]);

        std::swap(directions_[UpRight], directions_[UpLeft]);
        std::swap(directions_[UpLeft], directions_[DownLeft]);
        std::swap(directions_[DownLeft], directions_[DownRight]);
    }
    else if (angle == Down) {
        sprite_->setRotation(RAD_DEG * ANGLE_DOWN);

        std::swap(directions_[Up], directions_[Down]);
        std::swap(directions_[Right], directions_[Left]);

        std::swap(directions_[UpRight], directions_[DownLeft]);
        std::swap(directions_[DownRight], directions_[UpLeft]);
    }
    else if (angle == Left) {
        sprite_->setRotation(RAD_DEG * ANGLE_LEFT);

        std::swap(directions_[Up], directions_[Right]);
        std::swap(directions_[Right], directions_[Down]);
        std::swap(directions_[Down], directions_[Left]);

        std::swap(directions_[UpRight], directions_[DownRight]);
        std::swap(directions_[DownRight], directions_[DownLeft]);
        std::swap(directions_[DownLeft], directions_[UpLeft]);
    }

    availableDirections_ = directions_;
}

rtm::CoatingObject::CoatingObject(std::string const& filename, int column, int row,
    AngleType angle, float resistance, Directions directions)
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

bool rtm::CoatingObject::HasDirection(AngleType angle) const
{
    return directions_[angle];
}

bool rtm::CoatingObject::IsDirectionAvailable(AngleType angle) const
{
    return availableDirections_[angle];
}

void rtm::CoatingObject::SetDirectionAvailability(AngleType angle, bool status)
{
    availableDirections_[angle] = status;
}

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
