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
    std::string filename{ BUILDING_FILENAME_MASK };
    auto it{ filename.find("%No%") };
    filename.replace(it, 4, std::to_string(static_cast<int>(type)));

    return filename;
}
