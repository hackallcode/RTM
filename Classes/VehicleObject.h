#ifndef __VEHICLE_OBJECT_INCLUDED__
#define __VEHICLE_OBJECT_INCLUDED__

#include "DynamicObject.h"

namespace rtm {

    class VehicleObject abstract
        : public DynamicObject
    {
    public:
        VehicleObject();
        VehicleObject(std::string const& filename, float maxSpeed, float acceleration, float deceleration, size_t row = 0, size_t column = 0, float angle = 0.f);

        virtual ~VehicleObject() = default;

        virtual void Update(World* const scene) override;

    private:
        float maxSpeed_;
        float acceleration_;
        float deceleration_;

        void Accelerate_(float deltaTime);
        void Decelerate_(float deltaTime);
    };

}

#endif // __VEHICLE_OBJECT_INCLUDED__