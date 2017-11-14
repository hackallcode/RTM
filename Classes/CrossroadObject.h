#ifndef __CROSSROAD_OBJECT_INCLUDED__
#define __CROSSROAD_OBJECT_INCLUDED__

#include "CoatingUnion.h"

namespace rtm {
    class CrossroadObject
        : public CoatingUnion
    {
    public:
        CrossroadObject();
        CrossroadObject(int column, int row, LinesCounts linesCounts, 
            ControlUnitType controlUnitType = NoControlUnit);
        CrossroadObject(int column, int row, LinesCounts linesCounts, AngleType nullDirection, 
            ControlUnitType controlUnitType = NoControlUnit);

        static CoatingMatrix CrossroadMatrix(int column, int row, LinesCounts linesCounts);
        static CoatingMatrix TCrossroadMatrix(int column, int row, LinesCounts linesCounts, AngleType nullDirection);

        virtual ~CrossroadObject() = default;

        ControlUnitShared GetControlUnit() const;

        virtual void ShowSprites(WorldScene* const scene) override;
        virtual void ReleaseSprites(WorldScene* const scene) override;

    private:
        LinesCounts linesCounts_;
        AngleType nullDirection_;
        ControlUnitShared controlUnit_;
    };
}

#endif // __CROSSROAD_OBJECT_INCLUDED__