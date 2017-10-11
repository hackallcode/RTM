#ifndef __STATIC_OBJECT_INCLUDED__
#define __STATIC_OBJECT_INCLUDED__

#include "WorldObject.h"

namespace rtm {

    class World;

    class StaticObject abstract
        : public WorldObject
    {
    public:
        StaticObject();
        StaticObject(float x, float y, cocos2d::Sprite* sprite = nullptr);
        StaticObject(float x, float y, std::string const& fileName);

        virtual ~StaticObject() = default;
    };

}

#endif // __STATIC_OBJECT_INCLUDED__