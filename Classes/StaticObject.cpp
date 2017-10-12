#include "StaticObject.h"
#include "WorldScene.h"

rtm::StaticObject::StaticObject()
    : WorldObject()
{}

rtm::StaticObject::StaticObject(float x, float y, cocos2d::Sprite* sprite)
    : WorldObject(x, y, sprite)
{}

rtm::StaticObject::StaticObject(float x, float y, std::string const& filename)
    : WorldObject(x, y, filename)
{}