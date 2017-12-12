#include "AllRtmClasses.h"

rtm::BuildingObject::BuildingObject()
    : MapObject{}
{}

rtm::BuildingObject::BuildingObject(cocos2d::Sprite* const sprite, int column, int row, float angle)
    : MapObject{ sprite, column, row, angle }
{}

rtm::BuildingObject::BuildingObject(std::string const& filename, int column, int row, float angle)
    : MapObject{ filename, column, row, angle }
{}

rtm::BuildingObject::BuildingObject(size_t type, int column, int row, float angle)
    : BuildingObject{ GetFilename(BUILDING_FILENAME_MASK, type), column, row, angle }
{}
