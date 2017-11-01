#ifndef __ROAD_OBJECT_INCLUDED__
#define __ROAD_OBJECT_INCLUDED__

#include "CoatingObject.h"

namespace rtm {
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