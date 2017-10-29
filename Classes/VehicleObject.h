#ifndef __VEHICLE_OBJECT_INCLUDED__
#define __VEHICLE_OBJECT_INCLUDED__

#include "DynamicObject.h"

namespace rtm {

    enum StateType {
        NotStarted = 0
        , MustStart
        , Started
        , MustStop
    };

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
        float speed_;
        float maxSpeed_;
        float acceleration_;
        float deceleration_;
        StateType isMovement_;
        StateType isRotation_;
        StateType isLineChanging_;
        float remainingAngle_;
        float remainingOffset_;
        float remainingOffsetAngle_;

        void MaintainSpeed_(float speed);
        
        void CheckRoadAhead(WorldController* const world);
        DynamicObject* const CanMoveForward_(WorldController* const world);
        DynamicObject* const CanRotate_(WorldController* const world);
        DynamicObject* const CanChangeLine_(WorldController* const world);

        void Move_(WorldController* const world);
        void Acceleration_(WorldController* const world);
        void Movement_(WorldController* const world);
        void Rotation_(WorldController* const world);
        void LineChanging_(WorldController* const world);
    };

}

#endif // __VEHICLE_OBJECT_INCLUDED__