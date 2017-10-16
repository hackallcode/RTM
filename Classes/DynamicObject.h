#ifndef __DYNAMIC_OBJECT_INCLUDED__
#define __DYNAMIC_OBJECT_INCLUDED__

#include "WorldObject.h"

namespace rtm {

    class DynamicObject abstract
        : public WorldObject
    {
    public:
        DynamicObject();
        DynamicObject(float x, float y, float a, float speed, cocos2d::Sprite* sprite = nullptr);
        DynamicObject(float x, float y, float a, float speed, std::string const& filename);

        virtual ~DynamicObject() = default;

        virtual void Update(World* const scene) override;

        float GetSpeed() const;

    protected:
        void SetSpeed_(float speed);

    private:
        float speed_;
    };

}

#endif // __DYNAMIC_OBJECT_INCLUDED__