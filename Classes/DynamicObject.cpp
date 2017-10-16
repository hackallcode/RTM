#include "DynamicObject.h"
#include "WorldScene.h"

rtm::DynamicObject::DynamicObject()
    : WorldObject()
    , speed_(0.f)
{}

rtm::DynamicObject::DynamicObject(float x, float y, float a, float speed, cocos2d::Sprite* sprite)
    : WorldObject(x, y, a, sprite)
    , speed_(speed)
{}

rtm::DynamicObject::DynamicObject(float x, float y, float a, float speed, std::string const& filename)
    : WorldObject(x, y, a, filename)
    , speed_(speed)
{}

void rtm::DynamicObject::Update(World* const scene)
{
    WorldObject::Update(scene);

    SetX_(GetX() + speed_ * sin(GetA() * DEG_RAG) * scene->getMissedTime());
    SetY_(GetY() + speed_ * cos(GetA() * DEG_RAG) * scene->getMissedTime());
    OnPositionUpdate_();
}

float rtm::DynamicObject::GetSpeed() const
{
    return speed_;
}

void rtm::DynamicObject::SetSpeed_(float speed)
{
    speed_ = speed;
}