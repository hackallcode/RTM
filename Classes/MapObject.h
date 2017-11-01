#ifndef __MAP_OBJECT_INCLUDED__
#define __MAP_OBJECT_INCLUDED__

#include "StaticObject.h"

namespace rtm {
    class MapObject
        : public StaticObject
    {
    public:
        MapObject();
        MapObject(cocos2d::Sprite* sprite, int column, int row, float angle);
        MapObject(std::string const& filename, int column, int row, float angle);

        virtual ~MapObject() = default;
    };
}

#endif // __MAP_OBJECT_INCLUDED__