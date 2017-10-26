#ifndef __DYNAMIC_OBJECT_INCLUDED__
#define __DYNAMIC_OBJECT_INCLUDED__

#include "WorldObject.h"

namespace rtm {

    float const VIEW_RADIUS                     = 100.f;
    float const VIEW_ANGLE                      = 10.f * DEG_RAD;
    float const VIEW_ANGLE_SHIFT                = 0.f;
    float const ROTATION_VIEW_RADIUS            = 55.f;
    float const ROTATION_VIEW_ANGLE             = 45.f * DEG_RAD;
    float const ROTATION_VIEW_ANGLE_SHIFT       = 30.f * DEG_RAD;
    float const LINE_CHANGING_VIEW_RADIUS       = 60.f;
    float const LINE_CHANGING_VIEW_ANGLE        = 30.f * DEG_RAD;
    float const LINE_CHANGING_VIEW_ANGLE_SHIFT  = 20.f * DEG_RAD;

    class StaticObject;

    class DynamicObject abstract
        : public WorldObject
    {
    public:
        DynamicObject();
        DynamicObject(cocos2d::Sprite* sprite, float x, float y, float angle, float speed);
        DynamicObject(std::string const& filename, float x, float y, float angle, float speed);
        virtual ~DynamicObject() = default;

        float GetSpeed() const;
        bool HasCollision() const;

        virtual void Update(WorldController* const world) override;

        friend void CheckCollisions(std::vector<std::unique_ptr<DynamicObject>> const& dynamicObjs,
            std::vector<std::unique_ptr<StaticObject>> const& staticObjs);

    protected:
        void SetSpeed_(float speed);
        void SetCollisionFlag_(bool flag);
        
        bool IsBeholding_(WorldObject const* const other, float radius = VIEW_RADIUS, 
            float angle = VIEW_ANGLE, float angleShift = VIEW_ANGLE_SHIFT) const;
        bool IsIntersecting_(WorldObject const* const other) const;

    private:
        float speed_;
        bool hasCollision_;

        bool IsNear_(WorldObject const* const other) const;
    };

    void CheckCollisions(std::vector<std::unique_ptr<DynamicObject>> const& dynamicObjs,
        std::vector<std::unique_ptr<StaticObject>> const& staticObjs = std::vector<std::unique_ptr<StaticObject>>{});
}

#endif // __DYNAMIC_OBJECT_INCLUDED__