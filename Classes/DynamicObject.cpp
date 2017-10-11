#include "DynamicObject.h"
#include "WorldScene.h"

rtm::DynamicObject::DynamicObject()
    : WorldObject()
    , hSpeed_(0.f)
    , vSpeed_(0.f)
{}

rtm::DynamicObject::DynamicObject(float x, float y, float hSpeed, float vSpeed, cocos2d::Sprite* sprite)
    : WorldObject(x, y, sprite)
    , hSpeed_(hSpeed)
    , vSpeed_(vSpeed)
{}

rtm::DynamicObject::DynamicObject(float x, float y, float hSpeed, float vSpeed, std::string const& fileName)
    : WorldObject(x, y, fileName)
    , hSpeed_(hSpeed)
    , vSpeed_(vSpeed)
{}

void rtm::DynamicObject::Update(World* const scene)
{
    WorldObject::Update(scene);

    SetY_(GetY() + vSpeed_ * scene->getMissedTime());
    SetX_(GetX() + hSpeed_ * scene->getMissedTime());
    OnPositionUpdate_();
}

float rtm::DynamicObject::GetHSpeed() const
{
    return hSpeed_;
}

float rtm::DynamicObject::GetVSpeed() const
{
    return vSpeed_;
}

void rtm::DynamicObject::SetHSpeed_(float speed)
{
    hSpeed_ = speed;
}

void rtm::DynamicObject::SetVSpeed_(float speed)
{
    vSpeed_ = speed;
}