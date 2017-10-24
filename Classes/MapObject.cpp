#include "MapController.h"
#include "MapObject.h"

rtm::MapObject::MapObject()
    : StaticObject()
{}

rtm::MapObject::MapObject(std::string const& filename, int row, int column, float angle)
    : StaticObject((row + 0.5) * CELL_SIZE, (column + 0.5) * CELL_SIZE, angle, filename)
{}
