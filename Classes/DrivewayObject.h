#ifndef __DRIVEWAY_OBJECTS_INCLUDED__
#define __DRIVEWAY_OBJECTS_INCLUDED__

#include "CoatingUnion.h"

namespace rtm {
    class DrivewayObject
        : public CoatingUnion
    {
    public:
        DrivewayObject();
        DrivewayObject(int column, int row, std::vector<std::vector<CoatingObjectUnique>>&& objects, DirectionType direction);
        DrivewayObject(int column, int row, size_t width, size_t height, DirectionType direction);

        static std::vector<std::vector<CoatingObjectUnique>> CreateMatrix(int column, int row, 
            size_t width, size_t height, DirectionType direction);

        virtual ~DrivewayObject() = default;

        DirectionType GetDirection() const;
        float GetLength() const;

    private:
        DirectionType direction_;
        float length_;

        static float CountLength_(size_t width, size_t height, DirectionType direction);
    };
}

#endif // __DRIVEWAY_OBJECTS_INCLUDED__