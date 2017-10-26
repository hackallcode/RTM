#include "WorldController.h"
#include "CoatingObject.h"

rtm::CoatingObject::CoatingObject()
    : CoatingObject(nullptr, 0.f, 0.f)
{}

rtm::CoatingObject::CoatingObject(cocos2d::Sprite* const sprite, int column, int row)
{
    SetSprite_(sprite); 
    SetX_(WorldController::Col2X(column));
    SetY_(WorldController::Row2Y(row));
}

rtm::CoatingObject::CoatingObject(std::string const& filename, int column, int row)
    : CoatingObject(cocos2d::Sprite::create(filename), row, column)
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

void rtm::CoatingObject::Update(WorldController* const world)
{}

void rtm::CoatingObject::SetSprite_(cocos2d::Sprite* const sprite)
{
    sprite_ = sprite;
    if (sprite_ != nullptr) {
        sprite_->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
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
