#ifndef __VEHICLE_OBJECT_INCLUDED__
#define __VEHICLE_OBJECT_INCLUDED__

#include "DynamicObject.h"

namespace rtm {

    enum StateType {
        NotStarted = 0
        , MustStart
        , Started
    };

    class VehicleObject abstract
        : public DynamicObject
    {
    public:
        VehicleObject();
        VehicleObject(std::string const& filename, int column, int row, float angle, float maxSpeed, float acceleration, float deceleration);

        virtual ~VehicleObject() = default;

        virtual void Update(WorldController* const world) override;

    protected:
        bool MoveForward_();
        bool Stop_();
        bool Rotate_(float angle = ANGLE_RIGHT);
        bool ChangeLine_(bool isRight = LEFT);

    private:
        float maxSpeed_;
        float acceleration_;
        float deceleration_;
        StateType isMovement_;
        StateType isRotation_;
        StateType isLineChanging_;
        float remainingAngle_;
        float remainingOffset_;
        float remainingOffsetAngle_;

        void Move_(WorldController* const world);
        void Accelerate_(float deltaTime);
        void Decelerate_(float deltaTime);
        void Movement_(WorldController* const world);
        void Rotation_(WorldController* const world);
        void LineChanging_(WorldController* const world);
    };

}

#endif // __VEHICLE_OBJECT_INCLUDED__