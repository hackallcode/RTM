#include "WorldObject.h"

rtm::WorldObject::WorldObject()
    : WorldObject{ nullptr, 0.f, 0.f, 0.f }
{}

rtm::WorldObject::WorldObject(cocos2d::Sprite* const sprite, float x, float y, float angle)
    : x_{ x }
    , y_{ y }
    , angle_{ angle }
{
    SetSprite_(sprite); // Set sprite_, w_, h_
}

rtm::WorldObject::WorldObject(std::string const& filename, float x, float y, float angle)
    : WorldObject{ cocos2d::Sprite::create(filename), x, y, angle }
{}

cocos2d::Sprite* rtm::WorldObject::GetSprite() const
{
    return sprite_;
}

float rtm::WorldObject::GetX() const
{
    return x_;
}

float rtm::WorldObject::GetY() const
{
    return y_;
}

float rtm::WorldObject::GetAngle() const
{
    return angle_;
}

float rtm::WorldObject::GetWidth() const
{
    return width_;
}

float rtm::WorldObject::GetHeight() const
{
    return height_;
}

void rtm::WorldObject::SetSprite_(cocos2d::Sprite* const sprite)
{
    sprite_ = sprite;
    if (sprite_ != nullptr) {
        sprite_->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
        SetWidth_(sprite_->getContentSize().width);
        SetHeight_(sprite_->getContentSize().height);
    }
    else {
        SetWidth_(0.f);
        SetHeight_(0.f);
    }
    PositionInit_();
}

void rtm::WorldObject::SetX_(float x)
{
    x_ = x;
}

void rtm::WorldObject::SetY_(float y)
{
    y_ = y;
}

void rtm::WorldObject::SetAngle_(float angle)
{
    angle_ = NormalizeAngle(angle);
}

void rtm::WorldObject::SetWidth_(float width)
{
    width_ = width;
}

void rtm::WorldObject::SetHeight_(float height)
{
    height_ = height;
}

void rtm::WorldObject::PositionInit_()
{
    OnXUpdate_();
    OnYUpdate_();
    OnAngleUpdate_();
    OnWidthUpdate_();
    OnHeightUpdate_();
}

void rtm::WorldObject::PositionUpdate_()
{
    if (x_ != prevX_) OnXUpdate_();
    if (y_ != prevY_) OnYUpdate_();
    if (angle_ != prevAngle_) OnAngleUpdate_();
    if (width_ != prevWidth_) OnWidthUpdate_();
    if (height_ != prevHeight_) OnHeightUpdate_();
}

void rtm::WorldObject::OnXUpdate_()
{
    SetSpriteX_(x_);
    prevX_ = x_;
}

void rtm::WorldObject::OnYUpdate_()
{
    SetSpriteY_(y_);
    prevY_ = y_;
}

void rtm::WorldObject::OnAngleUpdate_()
{
    SetSpriteAngle_(angle_);
    prevAngle_ = angle_;
}

void rtm::WorldObject::OnWidthUpdate_()
{
    SetSpriteWidth_(width_);
    prevWidth_ = width_;
}

void rtm::WorldObject::OnHeightUpdate_()
{
    SetSpriteHeight_(height_);
    prevHeight_ = height_;
}

void rtm::WorldObject::SetSpriteX_(float x)
{
    if (sprite_ != nullptr) {
        sprite_->setPositionX(x);
    }
}

void rtm::WorldObject::SetSpriteY_(float y)
{
    if (sprite_ != nullptr) {
        sprite_->setPositionY(y);
    }
}

void rtm::WorldObject::SetSpriteAngle_(float angle)
{
    if (sprite_ != nullptr) {
        sprite_->setRotation(angle * RAD_DEG);
    }
}

void rtm::WorldObject::SetSpriteWidth_(float width)
{
    if (sprite_ != nullptr) {
        sprite_->setContentSize(cocos2d::Size{ width, sprite_->getContentSize().height });
    }
}

void rtm::WorldObject::SetSpriteHeight_(float height)
{
    if (sprite_ != nullptr) {
        sprite_->setContentSize(cocos2d::Size{ sprite_->getContentSize().width, height });
    }
}