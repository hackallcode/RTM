#include "WorldController.h"
#include "BuildingObject.h"

rtm::BuildingObject::BuildingObject()
    : MapObject{}
{}

rtm::BuildingObject::BuildingObject(cocos2d::Sprite* const sprite, int column, int row, float angle)
    : MapObject{ sprite, column, row, angle }
{}

rtm::BuildingObject::BuildingObject(std::string const& filename, int column, int row, float angle)
    : MapObject{ filename, column, row, angle }
{}

rtm::BuildingObject::BuildingObject(BuildingType type, int column, int row, float angle)
    : BuildingObject{ BuildingObject::GetClassFile_(type), column, row, angle }
{}

std::string rtm::BuildingObject::GetClassFile_(BuildingType type)
{
    switch (type)
    {
    case rtm::BuildingTypeNo1:
        return BUILDING_NO_1_FILE;
    case rtm::BuildingTypeNo2:
        return BUILDING_NO_2_FILE;
    default:
        return BUILDING_NO_0_FILE;
    }
}
