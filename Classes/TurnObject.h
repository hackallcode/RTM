#ifndef __TURN_OBJECTS_INCLUDED__
#define __TURN_OBJECTS_INCLUDED__

#include "CoatingUnion.h"

namespace rtm {
    class TurnObject
        : public CoatingUnion
    {
    public:
        TurnObject();
        TurnObject(int column, int row, size_t linesCount, bool isRight, AngleType angle);

        static CoatingMatrix RightTurnMatrix(int column, int row, size_t linesCount, AngleType angle);
        static CoatingMatrix LeftTurnMatrix(int column, int row, size_t linesCount, AngleType angle);

        virtual ~TurnObject() = default;

        AngleType GetAngle() const;

    private:
        AngleType angle_;
    };
}

#endif // __TURN_OBJECTS_INCLUDED__