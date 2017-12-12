#include "AllRtmClasses.h"

rtm::BushObject::BushObject()
    : MapObject{}
{}

rtm::BushObject::BushObject(cocos2d::Sprite* const sprite, int column, int row, float angle)
    : MapObject{ sprite, column, row, angle }
{}

rtm::BushObject::BushObject(std::string const& filename, int column, int row, float angle)
    : MapObject{ filename, column, row, angle }
{}

rtm::BushObject::BushObject(size_t type, int column, int row, float angle)
    : BushObject{ GetFilename(BUSH_FILENAME_MASK, type), column, row, angle }
{}
