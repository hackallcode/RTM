#include "WorldScene.h"
#include "MapObject.h"

rtm::MapObject::MapObject()
    : StaticObject()
{}

rtm::MapObject::MapObject(std::string const& filename, size_t row, size_t column, float angle)
    : StaticObject((row + 0.5) * CELL_SIZE, (column + 0.5) * CELL_SIZE, angle, filename)
{}
