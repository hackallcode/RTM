#ifndef __WORLD_OBJECT_INCLUDED__
#define __WORLD_OBJECT_INCLUDED__

#include "General.h"

namespace rtm {
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
}

#endif // __WORLD_OBJECT_INCLUDED__