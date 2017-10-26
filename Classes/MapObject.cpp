#include "WorldController.h"
#include "MapObject.h"

rtm::MapObject::MapObject()
    : StaticObject{}
{}

rtm::MapObject::MapObject(cocos2d::Sprite* sprite, int column, int row, float angle)
    : StaticObject{ sprite, WorldController::Col2X(column), WorldController::Row2Y(row), angle }
{}

rtm::MapObject::MapObject(std::string const& filename, int column, int row, float angle)
    : StaticObject{ filename, WorldController::Col2X(column), WorldController::Row2Y(row), angle }
{}
