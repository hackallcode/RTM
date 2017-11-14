#include "RoadCoating.h"

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

rtm::RoadCoating::RoadCoating(RoadType type, int column, int row, AngleType angle)
    : RoadCoating{
        GetClassFile_(type)
        , column
        , row
        , angle
        , GetClassResistance_(type)
        , GetClassDirections_(type)
    }
{}

void rtm::RoadCoating::Update(WorldController* const world)
{}

std::string rtm::RoadCoating::GetClassFile_(RoadType type)
{
    std::string filename{ ROAD_FILENAME_MASK };
    auto it{ filename.find("%No%") };
    filename.replace(it, 4, std::to_string(static_cast<int>(type)));

    return filename;
}

float rtm::RoadCoating::GetClassResistance_(RoadType type)
{
    int number = static_cast<int>(type);
    if (0 <= number && number < static_cast<int>(ROADS_RESISTANCES.size())) {
        return ROADS_RESISTANCES[number];
    }
    else {
        return ROADS_RESISTANCES[0];
    }
}

rtm::Directions const& rtm::RoadCoating::GetClassDirections_(RoadType type)
{
    int number = static_cast<int>(type);
    if (0 <= number && number < static_cast<int>(ROADS_DIRECTIONS.size())) {
        return ROADS_DIRECTIONS[number];
    }
    else {
        return ROADS_DIRECTIONS[0];
    }
}