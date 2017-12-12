#include "AllRtmClasses.h"

rtm::RoadCoating::RoadCoating()
    : CoatingObject{}
{}

rtm::RoadCoating::RoadCoating(cocos2d::Sprite* const sprite, int column, int row, 
    AngleType angle, float resistance, Directions directions)
    : CoatingObject{ sprite, column, row, angle, resistance, directions }
{}

rtm::RoadCoating::RoadCoating(std::string const& filename, int column, int row, 
    AngleType angle, float resistance, Directions directions)
    : CoatingObject{filename, column, row, angle, resistance, directions}
{}

rtm::RoadCoating::RoadCoating(CoatingType type, size_t id, int column, int row, AngleType angle)
    : RoadCoating{
        GetFilename(ROAD_FILENAME_MASK, COATING_INDEXES[type] + id)
        , column
        , row
        , angle
        , GetClassResistance_(type)
        , GetClassDirections_(id)
    }
{}

float rtm::RoadCoating::GetClassResistance_(CoatingType type)
{
    int number = static_cast<int>(type) + 1;
    if (0 <= number && number < static_cast<int>(ROADS_RESISTANCES.size())) {
        return ROADS_RESISTANCES[number];
    }
    else {
        return ROADS_RESISTANCES[0];
    }
}

rtm::Directions const& rtm::RoadCoating::GetClassDirections_(size_t id)
{
    int number = static_cast<int>(id);
    if (0 <= number && number < static_cast<int>(ROADS_DIRECTIONS.size())) {
        return ROADS_DIRECTIONS[number];
    }
    else {
        return ROADS_DIRECTIONS[0];
    }
}