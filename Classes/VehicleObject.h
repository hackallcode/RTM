#ifndef __VEHICLE_OBJECT_INCLUDED__
#define __VEHICLE_OBJECT_INCLUDED__

#include "DynamicObject.h"

namespace rtm {
    class VehicleObject abstract
        : public DynamicObject
    {
    public:
        VehicleObject();
        VehicleObject(cocos2d::Sprite* const sprite, int column, int row, 
            float angle, float maxSpeed, float acceleration, float deceleration);
        VehicleObject(std::string const& filename, int column, int row, 
            float angle, float maxSpeed, float acceleration, float deceleration);

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
        
        bool wasCollision_;
        
        StateType isMovement_;
        float finalSpeed_;
        bool hasDesiredSpeed_;
        float desiredSpeed_;
        float breakingDistance_;
        float recommendedSpeed_;
        
        StateType isRotation_;
        float remainingAngle_;
        
        StateType isLineChanging_;
        float remainingOffset_;
        float remainingOffsetAngle_;

        void SetRecommendedSpeed_(float speed);
        void SetDesiredSpeed_(float speed);
        void ResetDesiredSpeed_();
        void SetBreakingDistance_(float distance);
        
        CoatingObject* const GetNextCoating_(WorldController* const world, int delta = 1);
        void CheckRoadAhead_(WorldController* const world);
        DynamicObject* const CanMoveForward_(WorldController* const world);
        DynamicObject* const CanRotate_(WorldController* const world);
        DynamicObject* const CanChangeLine_(WorldController* const world);

        void Move_(WorldController* const world);
        void Acceleration_(WorldController* const world);
        void Movement_(WorldController* const world);
        void Breaking_(WorldController* const world);
        void Rotation_(WorldController* const world);
        void LineChanging_(WorldController* const world);
    };
}

#endif // __VEHICLE_OBJECT_INCLUDED__