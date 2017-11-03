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

    protected:
        virtual bool MovementStart_(WorldController* const world) override;
        virtual bool MovementTick_(WorldController* const world) override;
        virtual bool MovementEnd_(WorldController* const world) override;

        virtual bool RotationStart_(WorldController* const world) override;

        virtual bool LineChangingStart(WorldController* const world) override;

    private:
        float recommendedSpeed_;
        float desiredSpeed_;
        bool hasDesiredSpeed_;
        bool forwardSightEnabled_;
                
        void SetDesiredSpeed_(float speed);
        void ResetDesiredSpeed_();

        void CheckRoadAhead_(WorldController* const world);

        static std::string GetClassFile_(CarType type);
        static float GetClassMaxSpeed_(CarType type);
        static float GetClassAcceleration_(CarType type);
    };
}

#endif // __CAR_OBJECT_INCLUDED__