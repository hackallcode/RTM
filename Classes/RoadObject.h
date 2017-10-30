#ifndef __ROAD_OBJECT_INCLUDED__
#define __ROAD_OBJECT_INCLUDED__

#include "CoatingObject.h"

namespace rtm {

    enum RoadType {
        RoadTypeNo1 = 1
        , RoadTypeNo2
    };

    std::string const ROAD_NO_0_FILE = "res/coating/RoadNo0.png"; // File path
    float const ROAD_NO_0_RESISTANCE = 1.f; // Coefficient of resistance
    Directions const ROAD_NO_0_DIRECTIONS = { false, false, false, false }; // Enabled directions (top, right, bottom, left)

    std::string const ROAD_NO_1_FILE = "res/coating/RoadNo1.png";
    float const ROAD_NO_1_RESISTANCE = 1.f;
    Directions const ROAD_NO_1_DIRECTIONS = { true, false, true, false };

    std::string const ROAD_NO_2_FILE = "res/coating/RoadNo2.png";
    float const ROAD_NO_2_RESISTANCE = 1.f;
    Directions const ROAD_NO_2_DIRECTIONS = { false, true, true, false };

    class RoadObject
        : public CoatingObject
    {
    public:
        RoadObject();
        RoadObject(cocos2d::Sprite* const sprite, int column, int row, 
            float angle, float resistance, Directions directions);
        RoadObject(std::string const& filename, int column, int row, 
            float angle, float resistance, Directions directions);
        RoadObject(RoadType type, int column, int row, float angle);

        virtual ~RoadObject() = default;

        virtual void Update(WorldController* const world);

    private:
        static std::string GetClassFile_(RoadType type);
        static float GetClassResistance_(RoadType type);
        static Directions const& GetClassDirections_(RoadType type);
    };

}

#endif // __ROAD_OBJECT_INCLUDED__