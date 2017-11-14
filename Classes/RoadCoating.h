#ifndef __ROAD_COATING_INCLUDED__
#define __ROAD_COATING_INCLUDED__

#include "CoatingObject.h"

namespace rtm {
    class RoadCoating
        : public CoatingObject
    {
    public:
        RoadCoating();
        RoadCoating(cocos2d::Sprite* const sprite, int column, int row, 
            AngleType angle, float resistance, Directions directions);
        RoadCoating(std::string const& filename, int column, int row,
            AngleType angle, float resistance, Directions directions);
        RoadCoating(RoadType type, int column, int row, AngleType angle);

        virtual ~RoadCoating() = default;

        virtual void Update(WorldController* const world);

    private:
        static std::string GetClassFile_(RoadType type);
        static float GetClassResistance_(RoadType type);
        static Directions const& GetClassDirections_(RoadType type);
    };  
}

#endif // __ROAD_COATING_INCLUDED__