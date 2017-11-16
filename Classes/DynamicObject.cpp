#include "DynamicObject.h"
#include "WorldController.h"
#include "StaticObject.h"

///////////////////////
//  LOCAL FUNCTIONS  //
///////////////////////

float Area(cocos2d::Point a, cocos2d::Point b, cocos2d::Point c)
{
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool RangesHaveIntersection(float a1, float a2, float b1, float b2)
{
    if (a1 > a2) {
        std::swap(a1, a2);
    }
    if (b1 > b2) {
        std::swap(b1, b2);
    }
    return max(a1, b1) <= min(a2, b2);
}

bool HaveIntersection(cocos2d::Point a1, cocos2d::Point a2, cocos2d::Point b1, cocos2d::Point b2)
{
    return
        RangesHaveIntersection(a1.x, a2.x, b1.x, b2.x) &&
        RangesHaveIntersection(a1.y, a2.y, b1.y, b2.y) &&
        Area(a1, a2, b1) * Area(a1, a2, b2) <= 0 &&
        Area(b1, b2, a1) * Area(b1, b2, a2) <= 0;
}

///////////////////////
//  CLASS FUNCTIONS  //
///////////////////////

rtm::DynamicObject::DynamicObject()
    : WorldObject{}
    , speed_{ 0.f }
    , lastDelta_{ 0.f }
    , hasCollision_{ false }
{}

rtm::DynamicObject::DynamicObject(cocos2d::Sprite* sprite, float x, float y, float angle, float speed)
    : WorldObject{ sprite, x, y, angle }
    , speed_{ speed }
    , lastDelta_{ 0.f }
    , hasCollision_{ false }
{}

rtm::DynamicObject::DynamicObject(std::string const& filename, float x, float y, float angle, float speed)
    : WorldObject{ filename, x, y, angle }
    , speed_{ speed }
    , lastDelta_{ 0.f }
    , hasCollision_{ false }
{}

float rtm::DynamicObject::GetSpeed() const
{
    return speed_;
}

bool rtm::DynamicObject::HasCollision() const
{
    return hasCollision_;
}

void rtm::DynamicObject::Update(WorldController* const world)
{
    if (speed_ != 0.f) {
        lastDelta_ = speed_ * world->GetDeltaTime();
        SetX_(GetX() + lastDelta_ * sin(GetAngle()));
        SetY_(GetY() + lastDelta_ * cos(GetAngle()));
        PositionUpdate_();
    }
}

float rtm::DynamicObject::GetLastDelta() const
{
    return lastDelta_;
}

void rtm::DynamicObject::SetSpeed_(float speed)
{
    speed_ = speed;
}

void rtm::DynamicObject::SetCollisionFlag_(bool flag)
{
    hasCollision_ = flag;
}

bool rtm::DynamicObject::IsBeholding_(WorldObject const* const other, float radius, float angle, float angleShift) const
{
    if (other == this) {
        return false;
    }

    float deltaX{ other->GetX() - GetX() };
    float deltaY{ other->GetY() - GetY() };
    // If object is near (to shorten time of calculation)
    if (FT::length(deltaX, deltaY) < radius) {
        if (SameAngles(NormalizeAngle(FT::atan2(deltaX, deltaY) - GetAngle()), angleShift, angle)) {
            return true;
        }
    }

    return false;
}

bool rtm::DynamicObject::IsIntersecting_(WorldObject const* const other) const
{
    if (other == this) {
        return false;
    }

    // If object is near (to shorten time of calculation)
    if (IsNear_(other)) {

        //  A__________E__________ B
        //  |          |          |
        // N|_________O|__________|P 
        //  |          |          |
        //  |__________|__________|
        //  D          K          C

        float radius;
        float alfa;
        float sinVal;
        float cosVal;

        // For this sprite
        radius = FT::length(GetWidth(), GetHeight()) / 2; // segment OB
        alfa = FT::atan2(GetWidth(), GetHeight()); // angle EOB without angle of sprite

        FT::sincos(GetAngle() + alfa, &sinVal, &cosVal); // angle EOB
        cocos2d::Point a1(GetX() + radius * sinVal, GetY() + radius * cosVal); // B
        FT::sincos(GetAngle() + (F_PI - alfa), &sinVal, &cosVal); // angle EOC
        cocos2d::Point a2(GetX() + radius * sinVal, GetY() + radius * cosVal); // C
        FT::sincos(GetAngle() + (F_PI + alfa), &sinVal, &cosVal); // angle EOD
        cocos2d::Point a3(GetX() + radius * sinVal, GetY() + radius * cosVal); // D
        FT::sincos(GetAngle() - alfa, &sinVal, &cosVal); // angle EOC
        cocos2d::Point a4(GetX() + radius * sinVal, GetY() + radius * cosVal); // A

        // The same for other sprite
        radius = FT::length(other->GetWidth(), other->GetHeight()) / 2; // segment OB
        alfa = FT::atan2(other->GetWidth(), other->GetHeight()); // angle EOB without angle of sprite

        FT::sincos(other->GetAngle() + alfa, &sinVal, &cosVal); // angle EOB
        cocos2d::Point b1(other->GetX() + radius * sinVal, other->GetY() + radius * cosVal); // B
        FT::sincos(other->GetAngle() + (F_PI - alfa), &sinVal, &cosVal); // angle EOC
        cocos2d::Point b2(other->GetX() + radius * sinVal, other->GetY() + radius * cosVal); // C
        FT::sincos(other->GetAngle() + (F_PI + alfa), &sinVal, &cosVal); // angle EOD
        cocos2d::Point b3(other->GetX() + radius * sinVal, other->GetY() + radius * cosVal); // D
        FT::sincos(other->GetAngle() - alfa, &sinVal, &cosVal); // angle EOC
        cocos2d::Point b4(other->GetX() + radius * sinVal, other->GetY() + radius * cosVal); // A

        // If one of segment cross other segment
        return
            HaveIntersection(a1, a2, b1, b2) ||
            HaveIntersection(a1, a2, b2, b3) ||
            HaveIntersection(a1, a2, b3, b4) ||
            HaveIntersection(a1, a2, b4, b1) ||
            HaveIntersection(a2, a3, b1, b2) ||
            HaveIntersection(a2, a3, b2, b3) ||
            HaveIntersection(a2, a3, b3, b4) ||
            HaveIntersection(a2, a3, b4, b1) ||
            HaveIntersection(a3, a4, b1, b2) ||
            HaveIntersection(a3, a4, b2, b3) ||
            HaveIntersection(a3, a4, b3, b4) ||
            HaveIntersection(a3, a4, b4, b1) ||
            HaveIntersection(a4, a1, b1, b2) ||
            HaveIntersection(a4, a1, b2, b3) ||
            HaveIntersection(a4, a1, b3, b4) ||
            HaveIntersection(a4, a1, b4, b1);
    }

    return false;
}

bool rtm::DynamicObject::IsNear_(WorldObject const* const other) const
{
    float distance{ FT::length(other->GetX() - GetX(), other->GetY() - GetY()) };
    float radius{ FT::length(GetWidth() / 2, GetHeight() / 2) };
    float otherRadius{ FT::length(other->GetWidth() / 2, other->GetHeight() / 2) };
    return distance <= radius + otherRadius;
}

void rtm::CheckCollisions(WorldController* const world)
{
    auto& dynamicObjs = world->GetDynamicObjects();

    for (auto& obj : dynamicObjs) {
        obj->SetCollisionFlag_(false);
    }
    for (size_t i{ 0 }; i < dynamicObjs.size(); ++i) {
        for (size_t j{ i + 1 }; j < dynamicObjs.size(); ++j) {
            if (i == j) {
                continue;
            }
            else {
                if (dynamicObjs[i]->IsIntersecting_(dynamicObjs[j].get())) {
                    dynamicObjs[i]->SetCollisionFlag_(true);
                    dynamicObjs[j]->SetCollisionFlag_(true);
                }
            }
        }
        if (!dynamicObjs[i]->HasCollision()) {
            int column{ PixelToCell(dynamicObjs[i]->GetX()) };
            int row{ PixelToCell(dynamicObjs[i]->GetY()) };
            for (int k{ column - 1 }; k <= column + 1; ++k) {
                for (int l{ row - 1 }; l <= row + 1; ++l) {
                    if (world->IsCorrectColumn(column) && world->IsCorrectRow(row)) {
                        StaticObject* staticObject{ world->GetStaticObject(k, l) };
                        if (staticObject != nullptr) {
                            if (dynamicObjs[i]->IsIntersecting_(staticObject)) {
                                dynamicObjs[i]->SetCollisionFlag_(true);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}