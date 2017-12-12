#include "AllRtmClasses.h"

rtm::MapObject::MapObject()
    : StaticObject{}
{}

rtm::MapObject::MapObject(cocos2d::Sprite* sprite, int column, int row, float angle)
    : StaticObject{ sprite, CellToPixel(column), CellToPixel(row), angle }
{}

rtm::MapObject::MapObject(std::string const& filename, int column, int row, float angle)
    : StaticObject{ filename, CellToPixel(column), CellToPixel(row), angle }
{}
