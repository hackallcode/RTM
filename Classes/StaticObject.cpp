#include "StaticObject.h"

rtm::StaticObject::StaticObject()
    : WorldObject{}
{}

rtm::StaticObject::StaticObject(cocos2d::Sprite* sprite, float x, float y, float angle)
    : WorldObject{ sprite, x, y, angle }
{}

rtm::StaticObject::StaticObject(std::string const& filename, float x, float y, float angle)
    : WorldObject{ filename, x, y, angle }
{}