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

    bool const LEFT             = false;
    bool const RIGHT            = true;

    float const F_PI_4          = 0.785398163397448309616f;
    float const F_PI_2          = 1.57079632679489661923f;
    float const F_PI            = 3.14159265358979323846f;
    float const F_2_PI          = 6.28318530717958647692f;

    float const DEG_RAD         = F_PI / 180.f;
    float const RAD_DEG         = 180.f / F_PI;

    float const ANGLE_TOP       = 0.f;
    float const ANGLE_RIGHT     = F_PI_2;
    float const ANGLE_BOTTOM    = -F_PI;
    float const ANGLE_LEFT      = -F_PI_2;
    
    float const ANGLE_DELTA     = 1.f * DEG_RAD;
    float const COORD_DELTA     = 1.f;

    class WorldController;

    class WorldObject abstract
    {
    public:
        WorldObject();
        WorldObject(cocos2d::Sprite* const sprite, float x, float y, float a);
        WorldObject(std::string const& filename, float x, float y, float a);
        virtual ~WorldObject() = default;

        cocos2d::Sprite* GetSprite() const;
        float GetX() const;
        float GetY() const;
        float GetAngle() const;
        float GetWidth() const;
        float GetHeight() const;

        virtual void Update(WorldController* const world);

    protected:
        void SetSprite_(cocos2d::Sprite* const sprite);
        void SetX_(float x);
        void SetY_(float y);
        void SetAngle_(float a);
        void SetWidth_(float w);
        void SetHeight_(float h);        
        
        virtual void PositionInit_();
        virtual void PositionUpdate_();
        virtual void OnXUpdate_();
        virtual void OnYUpdate_();
        virtual void OnAngleUpdate_();
        virtual void OnWidthUpdate_();
        virtual void OnHeightUpdate_();
        
    private:
        cocos2d::Sprite* sprite_;
        float x_, prevX_;
        float y_, prevY_;
        float angle_, prevAngle_;
        float width_, prevWidth_;
        float height_, prevHeight_;

        void SetSpriteX_(float x);
        void SetSpriteY_(float y);
        void SetSpriteAngle_(float angle);
        void SetSpriteWidth_(float width);
        void SetSpriteHeight_(float height);
    };

    bool IsSameCoords(float a, float b, float delta = COORD_DELTA);
    float RoundCoord(float coord, float delta = COORD_DELTA);
    bool IsSameAngles(float a, float b, float delta = ANGLE_DELTA);
    float RoundAngle(float angle, float delta = ANGLE_DELTA);
    float NormalizeAngle(float angle);
}

#endif // __WORLD_OBJECT_INCLUDED__