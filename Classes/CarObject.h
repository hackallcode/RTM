#ifndef __CAR_OBJECT_INCLUDED__
#define __CAR_OBJECT_INCLUDED__

#include "VehicleObject.h"

namespace rtm {
    class CarObject
        : public VehicleObject
    {
    public:
        CarObject();
        CarObject(cocos2d::Sprite* const sprite, int column, int row, float angle, float maxSpeed, float acceleration);
        CarObject(std::string const& filename, int column, int row, float angle, float maxSpeed, float acceleration);
        CarObject(CarType type, int column, int row, float angle);

        virtual ~CarObject() = default;

        virtual void Update(WorldController* const world) override;

    private:
        static std::string GetClassFile_(CarType type);
        static float GetClassMaxSpeed_(CarType type);
        static float GetClassAcceleration_(CarType type);
    };
}

#endif // __CAR_OBJECT_INCLUDED__