#ifndef __WORLD_OBJECT_INCLUDED__
#define __WORLD_OBJECT_INCLUDED__

#include "cocos2d.h"
#include "fasttrigo.h"
#include <string>

#ifndef min
    #define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
    #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

namespace rtm {

    float const F_PI_2 = 1.570796f;
    float const F_PI = 3.141593f;
    float const F_2_PI = 6.283185f;
    float const DEG_RAD = F_PI / 180;
    float const RAD_DEG = 180 / F_PI;
    float const ANGLE_DELTA = 0.01745f; // PI / 180 for rounding
    
    bool const LEFT = false;
    bool const RIGHT = true;

    float const ANGLE_TOP = 0.f;
    float const ANGLE_RIGHT = F_PI_2;
    float const ANGLE_BOTTOM = -F_PI;
    float const ANGLE_LEFT = -F_PI_2;

    class World;

    class WorldObject abstract
    {
    public:
        WorldObject();
        WorldObject(float x, float y, float a, cocos2d::Sprite* const sprite = nullptr);
        WorldObject(float x, float y, float a, std::string const& filename);
        virtual ~WorldObject() = default;

        void SetSprite(cocos2d::Sprite* const sprite);

        cocos2d::Sprite* GetSprite() const;
        float GetX() const;
        float GetY() const;
        float GetAngle() const;
        float GetWidth() const;
        float GetHeight() const;

        virtual void Update(World* const scene);

    protected:
        void SetX_(float x);
        void SetY_(float y);
        void SetAngle_(float a);
        void SetWidth_(float w);
        void SetHeight_(float h);

        virtual void PositionInit_();
        virtual void OnPositionUpdate_();
        virtual void OnXUpdate_();
        virtual void OnYUpdate_();
        virtual void OnAUpdate_();
        virtual void OnWUpdate_();
        virtual void OnHUpdate_();

    private:
        cocos2d::Sprite* sprite_;
        float x_, prevX_;
        float y_, prevY_;
        float a_, prevA_;
        float w_, prevW_;
        float h_, prevH_;

        void SetSpriteX_(float x);
        void SetSpriteY_(float y);
        void SetSpriteA_(float a);
    };
}

#endif // __WORLD_OBJECT_INCLUDED__