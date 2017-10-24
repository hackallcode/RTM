#ifndef __VEHICLE_OBJECT_INCLUDED__
#define __VEHICLE_OBJECT_INCLUDED__

#include "DynamicObject.h"

namespace rtm {

    float const LINE_CHANGING_ZONE = 5.f;

    class VehicleObject abstract
        : public DynamicObject
    {
    public:
        VehicleObject();
        VehicleObject(std::string const& filename, float maxSpeed, float acceleration, float deceleration, int row, int column, float angle);

        virtual ~VehicleObject() = default;

        virtual void Update(World* const scene) override;

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