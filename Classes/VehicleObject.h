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

        float GetRecommendedSpeed_();
        float GetDesiredSpeed_();
        float GetFinalSpeed_();
        bool IsForwardSightEnabled_();

        void SetRecommendedSpeed_(float speed);
        void SetDesiredSpeed_(float speed);
        void SetFinalSpeed_(float speed);
        void StopAtDistance_(float distance);
        void ResetDesiredSpeed_();
        void EnableForwardSight_();
        void DisableForwardSight_();
        void SetSimpleBrakes_();
        void SetDoubleBrakes_();

        // Sight
        CoatingObject* const GetNextCoating_(WorldController* const world, int delta = 1);
        DynamicObject* const CanMoveForward_(WorldController* const world);
        DynamicObject* const CanRotate_(WorldController* const world);
        DynamicObject* const CanChangeLine_(WorldController* const world);

        void CheckRoadAhead_(WorldController* const world);

    private:
        StateType isMovement_;
        StateType isRotation_;
        StateType isLineChanging_;
        float const maxSpeed_;
        float const acceleration_;
        float const deceleration_;
        float recommendedSpeed_;
        float desiredSpeed_;
        float finalSpeed_;
        float brakingDistance_;
        float remainingAngle_;
        float remainingOffset_;
        float remainingOffsetAngle_;
        bool wasCollision_;
        bool hasDesiredSpeed_;
        bool forwardSightEnabled_;
        bool doubleBrakes_;

        void Move_(WorldController* const world);
        void SmoothBraking_(WorldController* const world);
        void LineChanging_(WorldController* const world);
        void Rotation_(WorldController* const world);
        void Movement_(WorldController* const world);
        void Acceleration_(WorldController* const world);
    };
}

#endif // __VEHICLE_OBJECT_INCLUDED__