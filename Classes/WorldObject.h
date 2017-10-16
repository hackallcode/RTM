#ifndef __WORLD_OBJECT_INCLUDED__
#define __WORLD_OBJECT_INCLUDED__

#include "cocos2d.h"
#include <cmath>

namespace rtm {

    #define DEG_RAG 0.01745329251994329577 // PI / 180

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
        float GetDistance(WorldObject* other) const;
        bool IsCrossed(WorldObject* other) const;

        void SetSprite(cocos2d::Sprite* sprite);

    protected:
        void SetX_(float x);
        void SetY_(float y);
        void SetA_(float a);
        void SetW_(float w);
        void SetH_(float h);

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