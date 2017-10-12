#ifndef __DYNAMIC_OBJECT_INCLUDED__
#define __DYNAMIC_OBJECT_INCLUDED__

#include "WorldObject.h"

namespace rtm {

    class DynamicObject abstract
        : public WorldObject
    {
    public:
        DynamicObject();
        DynamicObject(float x, float y, float speed, float angle, cocos2d::Sprite* sprite = nullptr);
        DynamicObject(float x, float y, float speed, float angle, std::string const& filename);

        virtual ~DynamicObject() = default;

        virtual void Update(World* const scene) override;

        float GetSpeed() const;
        float GetAngle() const;

    protected:
        void SetSpeed_(float speed);
        void SetAngle_(float angle);

    private:
        float speed_;
        float angle_;
    };

}

#endif // __DYNAMIC_OBJECT_INCLUDED__