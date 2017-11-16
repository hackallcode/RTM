#ifndef __CONTROL_UNIT_INCLUDED__
#define __CONTROL_UNIT_INCLUDED__

#include "General.h"

namespace rtm {
    class ControlUnit
    {
    public:
        ControlUnit();
        ControlUnit(ControlUnitType type, int column, int row, LinesCounts linesCounts);
        ControlUnit(ControlUnitType type, int column, int row, LinesCounts linesCounts, AngleType nullDirection);
        
        virtual ~ControlUnit() = default;

        void Update(WorldController* const world);

        operator bool() const;
        SignalType GetSignal(DirectionType from, DirectionType to) const;

        void ShowSprites(WorldScene* const scene);
        void ReleaseSprites(WorldScene* const scene);

    private:
        ControlUnitType type_;
        int column_;
        int row_;
        LinesCounts linesCounts_;
        AngleType nullDirection_;
        CrossroadSignals signals_;
        DirectionsSignalSprites sprites_;
        float time_;
        size_t state_;

        void InitSignals_();
        void ResetSprites_();

        static std::string GetSignalFile_(size_t id);
    };
}

#endif // __CONTROL_UNIT_INCLUDED__