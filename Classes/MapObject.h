#ifndef __MAP_OBJECT_INCLUDED__
#define __MAP_OBJECT_INCLUDED__

#include "StaticObject.h"

namespace rtm {

    class MapObject
		: public StaticObject
    {
    public:
        MapObject();
        MapObject(std::string const& filename, size_t row, size_t column, float angle);

        virtual ~MapObject() = default;
    };

}

#endif // __MAP_OBJECT_INCLUDED__