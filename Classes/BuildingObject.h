#ifndef __BUILDING_OBJECT_INCLUDED__
#define __BUILDING_OBJECT_INCLUDED__

#include "StaticObject.h"

namespace rtm {

    enum BuildingType {
        BuildingTypeNo1 = 1
        , BuildingTypeNo2
    };

    std::string const BUILDING_NO_0_FILE = "res/static/BuildingNo0.png";
    std::string const BUILDING_NO_1_FILE = "res/static/BuildingNo1.png";
    std::string const BUILDING_NO_2_FILE = "res/static/BuildingNo2.png";

    class BuildingObject
        : public StaticObject
    {
    public:
        BuildingObject();
        BuildingObject(std::string const& filename, size_t row, size_t column, float angle);
        BuildingObject(BuildingType type, size_t row, size_t column, float angle);

        virtual ~BuildingObject() = default;

    private:
        static std::string GetClassFile_(BuildingType type);
    };

}

#endif // __BUILDING_OBJECT_INCLUDED__