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
        size_t GetLinesCount() const;

        bool isRightLine(int column, int row) const;
        bool isRightLine(float x, float y) const;
        bool isLeftLine(int column, int row) const;
        bool isLeftLine(float x, float y) const;

    private:
        int column_;
        int row_;
        size_t width_;
        size_t height_;
        AngleType angle_;
        float length_;
        size_t linesCount_;

        static float CountLength_(size_t width, size_t height, AngleType angle);
        static size_t CountLines_(size_t width, size_t height, AngleType angle);
    };
}

#endif // __DRIVEWAY_OBJECTS_INCLUDED__