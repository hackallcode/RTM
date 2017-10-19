#include "WorldScene.h"
#include "WorldObject.h"

rtm::WorldObject::WorldObject()
    : x_(0.f)
    , y_(0.f)
    , a_(0.f)
{
    SetSprite(nullptr); // Set sprite_, w_, h_
}

rtm::WorldObject::WorldObject(float x, float y, float a, cocos2d::Sprite* const sprite)
    : x_(x)
    , y_(y)
    , a_(a)
{
    SetSprite(sprite); // Set sprite_, w_, h_
}

rtm::WorldObject::WorldObject(float x, float y, float a, std::string const& filename)
    : x_(x)
    , y_(y)
    , a_(a)
{
    SetSprite(cocos2d::Sprite::create(filename)); // Set sprite_, w_, h_
}

void rtm::WorldObject::SetSprite(cocos2d::Sprite* const sprite)
{
    sprite_ = sprite;
    if (sprite_ != nullptr) {
        sprite_->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
        SetW_(sprite_->getContentSize().width);
        SetH_(sprite_->getContentSize().height);
    }
    else {
        SetW_(0.f);
        SetH_(0.f);
    }
    PositionInit_();
}

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

float rtm::WorldObject::GetA() const
{
    return a_;
}

float rtm::WorldObject::GetW() const
{
    return w_;
}

float rtm::WorldObject::GetH() const
{
    return h_;
}

void rtm::WorldObject::Update(World* const scene)
{}

void rtm::WorldObject::SetX_(float x)
{
    x_ = x;
}

void rtm::WorldObject::SetY_(float y)
{
    y_ = y;
}

void rtm::WorldObject::SetA_(float a)
{
    while (a < -F_PI) a += F_PI_2;
    while (a >= F_PI) a -= F_PI_2;
    a_ = a;
}

void rtm::WorldObject::SetW_(float w)
{
    w_ = w;
}

void rtm::WorldObject::SetH_(float h)
{
    h_ = h;
}

void rtm::WorldObject::PositionInit_()
{
    OnXUpdate_();
    OnYUpdate_();
    OnAUpdate_();
    OnWUpdate_();
    OnHUpdate_();
}

void rtm::WorldObject::OnPositionUpdate_()
{
    if (x_ != prevX_) OnXUpdate_();
    if (y_ != prevY_) OnYUpdate_();
    if (a_ != prevA_) OnAUpdate_();
    if (w_ != prevW_) OnWUpdate_();
    if (h_ != prevH_) OnHUpdate_();
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

void rtm::WorldObject::OnAUpdate_()
{
    SetSpriteA_(a_);
    prevA_ = a_;
}

void rtm::WorldObject::OnWUpdate_()
{
    prevW_ = w_;
}

void rtm::WorldObject::OnHUpdate_()
{
    prevH_ = h_;
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

void rtm::WorldObject::SetSpriteA_(float a)
{
    if (sprite_ != nullptr) {
        sprite_->setRotation(a * RAD_DEG);
    }
}