#ifndef __BUILDING_OBJECT_INCLUDED__
#define __BUILDING_OBJECT_INCLUDED__

#include "MapObject.h"

namespace rtm {

    enum BuildingType {
        BuildingTypeNo1 = 1
        , BuildingTypeNo2
    };

    std::string const BUILDING_NO_0_FILE = "res/static/BuildingNo0.png";
    std::string const BUILDING_NO_1_FILE = "res/static/BuildingNo1.png";
    std::string const BUILDING_NO_2_FILE = "res/static/BuildingNo2.png";

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