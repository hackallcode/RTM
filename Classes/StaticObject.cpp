#include "WorldScene.h"
#include "StaticObject.h"

rtm::StaticObject::StaticObject()
    : WorldObject()
{}

rtm::StaticObject::StaticObject(float x, float y, float a, cocos2d::Sprite* sprite)
    : WorldObject(x, y, a, sprite)
{}

rtm::StaticObject::StaticObject(float x, float y, float a, std::string const& filename)
    : WorldObject(x, y, a, filename)
{}