#ifndef __COATING_OBJECTS_INCLUDED__
#define __COATING_OBJECTS_INCLUDED__

#include "cocos2d.h"
#include <string>
#include <array>

namespace rtm {

    using Directions = std::array<bool, 4>;

    class WorldController;

    class CoatingObject
    {
    public:
        CoatingObject();
        CoatingObject(cocos2d::Sprite* const sprite, int column, int row, 
            float angle, float resistance, Directions directions);
        CoatingObject(std::string const& filename, int column, int row, 
            float angle, float resistance, Directions directions);

        virtual ~CoatingObject() = default;

        cocos2d::Sprite* GetSprite() const;
        float GetX() const;
        float GetY() const;
        float GetResistance() const;
        bool HasDirection(float angle) const;

        virtual void Update(WorldController* const world);

    protected:
        void SetSprite_(cocos2d::Sprite* const sprite);
        void SetX_(float x);
        void SetY_(float y);

    private:
        cocos2d::Sprite* sprite_;
        float x_;
        float y_;
        float resistance_;
        Directions directions_;
    };

}

#endif // __COATING_OBJECTS_INCLUDED__