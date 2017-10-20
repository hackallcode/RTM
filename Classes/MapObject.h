#ifndef __MAP_OBJECT_INCLUDED__
#define __MAP_OBJECT_INCLUDED__

#include "StaticObject.h"

namespace rtm {

    class MapObject
		: public StaticObject
    {
    public:
        MapObject();
        MapObject(std::string const& filename, int row, int column, float angle);

        virtual ~MapObject() = default;
    };

}

#endif // __MAP_OBJECT_INCLUDED__