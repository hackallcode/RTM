#ifndef __BUILDING_OBJECT_INCLUDED__
#define __BUILDING_OBJECT_INCLUDED__

#include "MapObject.h"

namespace rtm {
    class BuildingObject
        : public MapObject
    {
    public:
        BuildingObject();
        BuildingObject(cocos2d::Sprite* const sprite, int column, int row, float angle);
        BuildingObject(std::string const& filename, int column, int row, float angle);
        BuildingObject(BuildingType type, int column, int row, float angle);

        virtual ~BuildingObject() = default;

    private:
        static std::string GetClassFile_(BuildingType type);
    };
}

#endif // __BUILDING_OBJECT_INCLUDED__