#ifndef __WORLD_OBJECT_INCLUDED__
#define __WORLD_OBJECT_INCLUDED__

#include "cocos2d.h"
#include <cmath>

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
    #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

namespace rtm {

    float const DEG_RAD = M_PI / 180;
    float const RAD_DEG = 180 / M_PI;
    
    float const ANGLE_TOP = 0.f;
    float const ANGLE_RIGHT = 90.f;
    float const ANGLE_BOTTOM = 180.f;
    float const ANGLE_LEFT = 270.f;

    class World;

    class WorldObject abstract
    {
    public:
        WorldObject();
        WorldObject(float x, float y, float a, cocos2d::Sprite* sprite = nullptr);
        WorldObject(float x, float y, float a, std::string const& filename);

        virtual ~WorldObject() = default;

        virtual void Update(World* const scene);
        
        cocos2d::Sprite* GetSprite() const;
        float GetX() const;
        float GetY() const;
        float GetA() const; // Angle
        float GetW() const; // Width
        float GetH() const; // Height

        void SetSprite(cocos2d::Sprite* sprite);

    protected:
        void SetX_(float x);
        void SetY_(float y);
        void SetA_(float a);
        void SetW_(float w);
        void SetH_(float h);

        bool DoesSee_(WorldObject* other) const;
        bool DoesIntersect_(WorldObject* other) const;

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

        bool IsNear_(WorldObject* other) const;

        void SetSpriteX_(float x);
        void SetSpriteY_(float y);
        void SetSpriteA_(float a);
    };

    float GetHypotenuse(float a, float b);
    bool HaveIntersection(cocos2d::Point a1, cocos2d::Point a2, cocos2d::Point b1, cocos2d::Point b2);
}

#endif // __WORLD_OBJECT_INCLUDED__