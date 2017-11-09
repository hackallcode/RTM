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

        bool IsMovement_() const;
        bool IsRotation_() const;
        bool IsLineChanging_() const;
        float GetMaxSpeed_() const;
        float GetFinalSpeed_() const;

        void SetFinalSpeed_(float speed);
        void SetBrakingFactor_(float factor);
        void StopAtDistance_(float distance);

        // Sight
        CoatingObject* CheckForwardCoating_(WorldController* const world, int delta = 1);
        DynamicObject* CheckForwardArea_(WorldController* const world, float radius, float angle, float angleShift);
        DynamicObject* CheckMovingArea_(WorldController* const world);
        DynamicObject* CheckRotationArea_(WorldController* const world);
        DynamicObject* CheckLineChangingArea_(WorldController* const world);

        // Maneuver
        virtual void Move_(WorldController* const world);
        
        virtual bool MovementStart_(WorldController* const world);
        virtual bool MovementTick_(WorldController* const world);
        virtual bool MovementEnd_(WorldController* const world);
        
        virtual bool RotationStart_(WorldController* const world);
        virtual bool RotationTick_(WorldController* const world);
        virtual bool RotationEnd_(WorldController* const world);

        virtual bool LineChangingStart(WorldController* const world);
        virtual bool LineChangingTick_(WorldController* const world);
        virtual bool LineChangingEnd_(WorldController* const world);

    private:
        StateType isMovement_;
        StateType isRotation_;
        StateType isLineChanging_;
        float const maxSpeed_;
        float const acceleration_;
        float const deceleration_;
        float finalSpeed_;
        float brakingFactor_;
        float brakingDistance_;
        float remainingAngle_;
        float remainingOffset_;
        float remainingOffsetAngle_;
        bool wasCollision_;

        void LineChanging_(WorldController* const world);
        void Rotation_(WorldController* const world);
        void Movement_(WorldController* const world);
        void SpeedChanging_(WorldController* const world);
    };
}

#endif // __VEHICLE_OBJECT_INCLUDED__