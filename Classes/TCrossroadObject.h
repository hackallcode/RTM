#ifndef __TCROSSROAD_OBJECT_INCLUDED__
#define __TCROSSROAD_OBJECT_INCLUDED__

#include "CoatingUnion.h"

namespace rtm {
    class TCrossroadObject
        : public CoatingUnion
    {
    public:
        TCrossroadObject();
        TCrossroadObject(int column, int row, size_t upLines, size_t toRightLines,
            size_t downLines, size_t toLeftLines, DirectionType nullDirection);

        static std::vector<std::vector<CoatingObjectUnique>> CreateMatrix(int column, int row,
            size_t upLines, size_t toRightLines, size_t downLines, size_t toLeftLines, DirectionType nullDirection);

        virtual ~TCrossroadObject() = default;

    private:
        size_t upLines_;
        size_t toRightLines_;
        size_t downLines_;
        size_t toLeftLines_;
        DirectionType nullDirection_;
    };
}

#endif // __TCROSSROAD_OBJECT_INCLUDED__