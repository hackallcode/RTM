#ifndef __CROSSROAD_OBJECT_INCLUDED__
#define __CROSSROAD_OBJECT_INCLUDED__

#include "CoatingUnion.h"

namespace rtm {
    class CrossroadObject
        : public CoatingUnion
    {
    public:
        CrossroadObject();
        CrossroadObject(int column, int row, size_t upLines, size_t toRightLines, size_t downLines, size_t toLeftLines);

        static std::vector<std::vector<CoatingObjectUnique>> CreateMatrix(int column, int row, 
            size_t upLines, size_t toRightLines, size_t downLines, size_t toLeftLines);

        virtual ~CrossroadObject() = default;

    private:
        size_t upLines_;
        size_t toRightLines_;
        size_t downLines_;
        size_t toLeftLines_;
    };
}

#endif // __CROSSROAD_OBJECT_INCLUDED__