#ifndef __DRIVEWAY_OBJECTS_INCLUDED__
#define __DRIVEWAY_OBJECTS_INCLUDED__

#include "CoatingUnion.h"

namespace rtm {
    class DrivewayObject
        : public CoatingUnion
    {
    public:
        DrivewayObject();
        DrivewayObject(int column, int row, size_t width, size_t height, AngleType angle);

        static CoatingMatrix DrivewayMatrix(int column, int row, size_t width, size_t height, AngleType angle);

        virtual ~DrivewayObject() = default;

        AngleType GetAngle() const;
        virtual float GetLength() const override;

    private:
        AngleType angle_;
        float length_;

        static float CountLength_(size_t width, size_t height, AngleType angle);
    };
}

#endif // __DRIVEWAY_OBJECTS_INCLUDED__