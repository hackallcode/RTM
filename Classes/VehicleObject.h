#ifndef __VEHICLE_OBJECT_INCLUDED__
#define __VEHICLE_OBJECT_INCLUDED__

#include "DynamicObject.h"

namespace rtm {

    class VehicleObject abstract
        : public DynamicObject
    {
    public:
        VehicleObject();
        VehicleObject(std::string const& filename, int column, int row, float angle, float maxSpeed, float acceleration, float deceleration);

        virtual ~VehicleObject() = default;

        virtual void Update(WorldController* const world) override;

    protected:
        void Accelerate_(float deltaTime);
        void Decelerate_(float deltaTime);
        bool Rotate_(float angle = ANGLE_RIGHT);
        bool ChangeLine_(bool isRight = LEFT);

    private:
        float maxSpeed_;
        float acceleration_;
        float deceleration_;
        bool isRotation_;
        bool isLineChanging_;
        float remainingAngle_;
        float remainingOffset_;
        float remainingOffsetAngle_;

        void Move_(float deltaTime);
        void Rotation_(float deltaTime);
        void LineChanging_(float deltaTime);
    };

}

#endif // __VEHICLE_OBJECT_INCLUDED__