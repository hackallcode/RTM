#include "BuildingObject.h"
#include "WorldScene.h"

rtm::BuildingObject::BuildingObject()
    : StaticObject()
{}

rtm::BuildingObject::BuildingObject(std::string const& filename, size_t row, size_t column)
    : StaticObject((row + 0.5) * CELL_SIZE, (column + 0.5) * CELL_SIZE, filename)
{}

rtm::BuildingObject::BuildingObject(BuildingType type, size_t row, size_t column)
    : BuildingObject(
        BuildingObject::GetClassFile_(type)
        , row
        , column
    )
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
