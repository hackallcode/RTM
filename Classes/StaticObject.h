#ifndef __STATIC_OBJECT_INCLUDED__
#define __STATIC_OBJECT_INCLUDED__

#include "WorldObject.h"

namespace rtm {
    class StaticObject abstract
        : public WorldObject
    {
    public:
        StaticObject();
        StaticObject(cocos2d::Sprite* sprite, float x, float y, float angle);
        StaticObject(std::string const& filename, float x, float y, float angle);

        virtual ~StaticObject() = default;
    };
}

#endif // __STATIC_OBJECT_INCLUDED__