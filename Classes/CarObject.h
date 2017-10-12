#ifndef __CAR_OBJECT_INCLUDED__
#define __CAR_OBJECT_INCLUDED__

#include "VehicleObject.h"

namespace rtm {

    enum CarType {
        CarTypeNo1 = 1
        , CarTypeNo2
    };

    std::string const CAR_NO_0_FILE = "res/vehicle/CarNo0.png"; // File path
    float const CAR_NO_0_MAX_SPEED = 0.f; // Meters per second
    float const CAR_NO_0_ACCELERATION = 0.f; // Meters per second^2
    float const CAR_NO_0_DECELERATION = 0.f; // Meters per second^2

    std::string const CAR_NO_1_FILE = "res/vehicle/CarNo1.png";
    float const CAR_NO_1_MAX_SPEED = 21.f;
    float const CAR_NO_1_ACCELERATION = 2.f;
    float const CAR_NO_1_DECELERATION = 7.f;

    std::string const CAR_NO_2_FILE = "res/vehicle/CarNo2.png";
    float const CAR_NO_2_MAX_SPEED = 27.f;
    float const CAR_NO_2_ACCELERATION = 5.f;
    float const CAR_NO_2_DECELERATION = 9.f;

    class CarObject
        : public VehicleObject
    {
    public:
        CarObject();
        CarObject(std::string const& filename, float maxSpeed, float acceleration, float deceleration, size_t row = 0, size_t column = 0, float angle = 0.f);
        CarObject(CarType type, size_t row = 0, size_t column = 0, float angle = 0);

        virtual ~CarObject() = default;

        virtual void Update(World* const scene) override;

    private:
        static std::string GetClassFile_(CarType type);
        static float GetClassMaxSpeed_(CarType type);
        static float GetClassAcceleration_(CarType type);
        static float GetClassDeceleration_(CarType type);
    };

}

#endif // __CAR_OBJECT_INCLUDED__