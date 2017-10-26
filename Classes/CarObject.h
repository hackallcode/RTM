#ifndef __CAR_OBJECT_INCLUDED__
#define __CAR_OBJECT_INCLUDED__

#include "VehicleObject.h"

namespace rtm {

    enum CarType {
        CarTypeNo1 = 1
        , CarTypeNo2
        , CarTypeNo3
        , CarTypeNo4
        , CarTypeNo5
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
    float const CAR_NO_2_MAX_SPEED = 24.f;
    float const CAR_NO_2_ACCELERATION = 5.f;
    float const CAR_NO_2_DECELERATION = 9.f;

    std::string const CAR_NO_3_FILE = "res/vehicle/CarNo3.png";
    float const CAR_NO_3_MAX_SPEED = 30.f;
    float const CAR_NO_3_ACCELERATION = 5.f;
    float const CAR_NO_3_DECELERATION = 14.f;

    std::string const CAR_NO_4_FILE = "res/vehicle/CarNo4.png";
    float const CAR_NO_4_MAX_SPEED = 33.f;
    float const CAR_NO_4_ACCELERATION = 8.f;
    float const CAR_NO_4_DECELERATION = 17.f;

    std::string const CAR_NO_5_FILE = "res/vehicle/CarNo5.png";
    float const CAR_NO_5_MAX_SPEED = 36.f;
    float const CAR_NO_5_ACCELERATION = 9.f;
    float const CAR_NO_5_DECELERATION = 20.f;

    class CarObject
        : public VehicleObject
    {
    public:
        CarObject();
        CarObject(std::string const& filename, int column, int row, float angle, float maxSpeed, float acceleration, float deceleration);
        CarObject(CarType type, int column, int row, float angle);

        virtual ~CarObject() = default;

        virtual void Update(WorldController* const world) override;

    private:
        static std::string GetClassFile_(CarType type);
        static float GetClassMaxSpeed_(CarType type);
        static float GetClassAcceleration_(CarType type);
        static float GetClassDeceleration_(CarType type);
    };

}

#endif // __CAR_OBJECT_INCLUDED__