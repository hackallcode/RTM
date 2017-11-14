#ifndef __CONTROL_UNIT_INCLUDED__
#define __CONTROL_UNIT_INCLUDED__

#include "General.h"

namespace rtm {
    class ControlUnit
    {
    public:
        ControlUnit();
        ControlUnit(ControlUnitType controlUnitType, int column, int row, LinesCounts linesCounts);
        ControlUnit(ControlUnitType controlUnitType, int column, int row, LinesCounts linesCounts, AngleType nullDirection);
        
        virtual ~ControlUnit() = default;

        void Update(WorldController* const world);

        operator bool() const;
        TrafficLightSignal GetSignal(DirectionType from, DirectionType to) const;

    private:
        ControlUnitType type_;
        int column_;
        int row_;
        LinesCounts linesCounts_;
        AngleType nullDirection_;
        CrossroadSignals signals_;
        DirectionsSignalSprites sprites_;
        bool isSpritesInit_;
        float time_;

        void InitSignals_();
        void InitSprites_(WorldScene* const scene);
        void ResetSprites_();

        static std::string GetSignalFile_(size_t id);
    };
}

#endif // __CONTROL_UNIT_INCLUDED__