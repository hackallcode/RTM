#include "AllRtmClasses.h"

rtm::PuddleCoating::PuddleCoating()
    : CoatingObject{}
{}

rtm::PuddleCoating::PuddleCoating(cocos2d::Sprite* const sprite, int column, int row, AngleType angle)
    : CoatingObject{ sprite, column, row, angle, 0.8f, Directions{ true, true, true, true, true, true, true, true } }
{}

rtm::PuddleCoating::PuddleCoating(std::string const& filename, int column, int row, AngleType angle)
    : CoatingObject{ filename, column, row, angle, 0.8f, Directions{ true, true, true, true, true, true, true, true } }
{}

rtm::PuddleCoating::PuddleCoating(size_t id, int column, int row, AngleType angle)
    : PuddleCoating{ GetFilename(PUDDLE_FILENAME_MASK, id), column, row, angle }
{}