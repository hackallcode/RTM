#include "DynamicObject.h"
#include "WorldScene.h"

rtm::DynamicObject::DynamicObject()
    : WorldObject()
    , speed_(0.f)
    , angle_(0.f)
{}

rtm::DynamicObject::DynamicObject(float x, float y, float speed, float angle, cocos2d::Sprite* sprite)
    : WorldObject(x, y, sprite)
    , speed_(speed)
    , angle_(angle)
{}

rtm::DynamicObject::DynamicObject(float x, float y, float speed, float angle, std::string const& filename)
    : WorldObject(x, y, filename)
    , speed_(speed)
    , angle_(angle)
{}

void rtm::DynamicObject::Update(World* const scene)
{
    WorldObject::Update(scene);

    SetX_(GetX() + speed_ * cos(angle_) * scene->getMissedTime());
    SetY_(GetY() + speed_ * sin(angle_) * scene->getMissedTime());
    OnPositionUpdate_();
}

float rtm::DynamicObject::GetSpeed() const
{
    return speed_;
}

float rtm::DynamicObject::GetAngle() const
{
    return angle_;
}

void rtm::DynamicObject::SetSpeed_(float speed)
{
    speed_ = speed;
}

void rtm::DynamicObject::SetAngle_(float angle)
{
    angle_ = angle;
}