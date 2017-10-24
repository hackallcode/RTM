#ifndef __DYNAMIC_OBJECT_INCLUDED__
#define __DYNAMIC_OBJECT_INCLUDED__

#include "WorldObject.h"

namespace rtm {

    class StaticObject;

    class DynamicObject abstract
        : public WorldObject
    {
    public:
        DynamicObject();
        DynamicObject(float x, float y, float a, float speed, cocos2d::Sprite* sprite = nullptr);
        DynamicObject(float x, float y, float a, float speed, std::string const& filename);
        virtual ~DynamicObject() = default;

        virtual void Update(MapController* const map) override;

        friend void CheckCollisions(std::vector<std::unique_ptr<DynamicObject>> const& dynamicObjs,
            std::vector<std::unique_ptr<StaticObject>> const& staticObjs);

    protected:
        void SetSpeed_(float speed);
        void SetCollisionFlag_(bool flag);

        float GetSpeed_() const;
        bool HasCollision_() const;

        bool IsBeholding_(WorldObject const* const other) const;
        bool IsIntersecting_(WorldObject const* const other) const;

    private:
        float speed_;
        bool hasCollision_;

        bool IsNear_(WorldObject const* const other) const;
    };

    void CheckCollisions(std::vector<std::unique_ptr<DynamicObject>> const& dynamicObjs,
        std::vector<std::unique_ptr<StaticObject>> const& staticObjs);
}

#endif // __DYNAMIC_OBJECT_INCLUDED__