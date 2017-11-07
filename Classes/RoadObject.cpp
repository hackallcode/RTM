#include "WorldController.h"
#include "RoadObject.h"

rtm::RoadObject::RoadObject()
    : CoatingObject{}
{}

rtm::RoadObject::RoadObject(cocos2d::Sprite* const sprite, int column, int row, 
    float angle, float resistance, Directions directions)
    : CoatingObject{ sprite, column, row, angle, resistance, directions }
{}

rtm::RoadObject::RoadObject(std::string const& filename, int column, int row, 
    float angle, float resistance, Directions directions)
    : CoatingObject{filename, column, row, angle, resistance, directions}
{}

rtm::RoadObject::RoadObject(RoadType type, int column, int row, float angle)
    : RoadObject{
        GetClassFile_(type)
        , column
        , row
        , angle
        , GetClassResistance_(type)
        , GetClassDirections_(type)
    }
{}

void rtm::RoadObject::Update(WorldController* const world)
{}

std::string rtm::RoadObject::GetClassFile_(RoadType type)
{
    switch (type)
    {
    case rtm::RoadTypeNo1:
        return ROAD_NO_1_FILE;
    case rtm::RoadTypeNo2:
        return ROAD_NO_2_FILE;
    case rtm::RoadTypeNo3:
        return ROAD_NO_3_FILE;
    case rtm::RoadTypeNo4:
        return ROAD_NO_4_FILE;
    case rtm::RoadTypeNo5:
        return ROAD_NO_5_FILE;
    case rtm::RoadTypeNo6:
        return ROAD_NO_6_FILE;
    case rtm::RoadTypeNo7:
        return ROAD_NO_7_FILE;
    default:
        return ROAD_NO_0_FILE;
    }
}

float rtm::RoadObject::GetClassResistance_(RoadType type)
{
    switch (type)
    {
    case rtm::RoadTypeNo1:
        return ROAD_NO_1_RESISTANCE;
    case rtm::RoadTypeNo2:
        return ROAD_NO_2_RESISTANCE;
    default:
        return ROAD_NO_0_RESISTANCE;
    case rtm::RoadTypeNo3:
        return ROAD_NO_3_RESISTANCE;
    case rtm::RoadTypeNo4:
        return ROAD_NO_4_RESISTANCE;
    case rtm::RoadTypeNo5:
        return ROAD_NO_5_RESISTANCE;
    case rtm::RoadTypeNo6:
        return ROAD_NO_6_RESISTANCE;
    case rtm::RoadTypeNo7:
        return ROAD_NO_7_RESISTANCE;
    }
}

rtm::Directions const& rtm::RoadObject::GetClassDirections_(RoadType type)
{
    switch (type)
    {
    case rtm::RoadTypeNo1:
        return ROAD_NO_1_DIRECTIONS;
    case rtm::RoadTypeNo2:
        return ROAD_NO_2_DIRECTIONS;
    case rtm::RoadTypeNo3:
        return ROAD_NO_3_DIRECTIONS;
    case rtm::RoadTypeNo4:
        return ROAD_NO_4_DIRECTIONS;
    case rtm::RoadTypeNo5:
        return ROAD_NO_5_DIRECTIONS;
    case rtm::RoadTypeNo6:
        return ROAD_NO_6_DIRECTIONS;
    case rtm::RoadTypeNo7:
        return ROAD_NO_7_DIRECTIONS;
    default:
        return ROAD_NO_0_DIRECTIONS;
    }
}