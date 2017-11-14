#ifndef __COATING_OBJECTS_INCLUDED__
#define __COATING_OBJECTS_INCLUDED__

#include "General.h"

namespace rtm {
    class CoatingObject abstract
    {
    public:
        CoatingObject();
        CoatingObject(cocos2d::Sprite* const sprite, int column, int row,
            AngleType angle, float resistance, Directions directions);
        CoatingObject(std::string const& filename, int column, int row, 
            AngleType angle, float resistance, Directions directions);

        virtual ~CoatingObject() = default;

        cocos2d::Sprite* GetSprite() const;
        float GetX() const;
        float GetY() const;
        float GetResistance() const;
        bool HasDirection(AngleType angle) const;
        bool IsDirectionAvailable(AngleType angle) const;

        void SetDirectionAvailability(AngleType angle, bool status);

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
        Directions availableDirections_;
    };
}

#endif // __COATING_OBJECTS_INCLUDED__