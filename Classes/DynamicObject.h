#ifndef __DYNAMIC_OBJECT_INCLUDED__
#define __DYNAMIC_OBJECT_INCLUDED__

#include "WorldObject.h"

namespace rtm {
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

        friend void CheckCollisions(WorldController* const world);

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

    void CheckCollisions(WorldController* const world);
}

#endif // __DYNAMIC_OBJECT_INCLUDED__