#include "WorldObject.h"
#include "WorldScene.h"

rtm::WorldObject::WorldObject()
    : x_(0.f)
    , y_(0.f)
    , a_(0.f)
{
    SetSprite(nullptr); // Set sprite_, w_, h_
}

rtm::WorldObject::WorldObject(float x, float y, float a, cocos2d::Sprite* sprite)
    : x_(x)
    , y_(y)
    , a_(a)
{
    SetSprite(sprite); // Set sprite_, w_, h_
}

rtm::WorldObject::WorldObject(float x, float y, float a, std::string const& filename)
    : x_(x)
    , y_(y)
    , a_(a)
{
    SetSprite(cocos2d::Sprite::create(filename)); // Set sprite_, w_, h_
}

void rtm::WorldObject::Update(World* const scene)
{}

cocos2d::Sprite* rtm::WorldObject::GetSprite() const
{
    return sprite_;
}

float rtm::WorldObject::GetX() const
{
    return x_;
}

float rtm::WorldObject::GetY() const
{
    return y_;
}

float rtm::WorldObject::GetA() const
{
    return a_;
}

float rtm::WorldObject::GetW() const
{
    return w_;
}

float rtm::WorldObject::GetH() const
{
    return h_;
}

void rtm::WorldObject::SetSprite(cocos2d::Sprite* sprite)
{
    sprite_ = sprite;
    if (sprite_ != nullptr) {
        sprite_->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
        SetW_(sprite_->getContentSize().width);
        SetH_(sprite_->getContentSize().height);
    }
    else {
        SetW_(0.f);
        SetH_(0.f);
    }
    PositionInit_();
}

void rtm::WorldObject::SetX_(float x)
{
    x_ = x;
}

void rtm::WorldObject::SetY_(float y)
{
    y_ = y;
}

void rtm::WorldObject::SetA_(float a)
{
    while (a < 0) a += 360;
    while (a > 359) a -= 360;
    a_ = a;
}

void rtm::WorldObject::SetW_(float w)
{
    w_ = w;
}

void rtm::WorldObject::SetH_(float h)
{
    h_ = h;
}

bool rtm::WorldObject::DoesSee_(WorldObject * other) const
{
    return false;
}

bool rtm::WorldObject::DoesIntersect_(WorldObject* other) const
{
    // If object is near (to shorten time of calculation)
    if (other != this && IsNear_(other)) {
        
        //  A__________E__________ B
        //  |          |          |
        // N|_________O|__________|P 
        //  |          |          |
        //  |__________|__________|
        //  D          K          C

        float radius = GetHypotenuse(GetW(), GetH()) / 2; // segment OB
        float alfa = atan(GetW() / GetH()) * RAD_DEG; // angle EOB without angle of sprite
        
        float angle = alfa + GetA(); // angle EOB
        cocos2d::Point a1(GetX() + radius * sin(angle * DEG_RAD), GetY() + radius * cos(angle * DEG_RAD)); // B
        angle += 180; // angle EOD
        cocos2d::Point a3(GetX() + radius * sin(angle * DEG_RAD), GetY() + radius * cos(angle * DEG_RAD)); // D
        angle = 180 - alfa + GetA(); // angle EOC
        cocos2d::Point a2(GetX() + radius * sin(angle * DEG_RAD), GetY() + radius * cos(angle * DEG_RAD)); // C
        angle += 180; // angle EOA
        cocos2d::Point a4(GetX() + radius * sin(angle * DEG_RAD), GetY() + radius * cos(angle * DEG_RAD)); // A

        // The same for other sprite
        radius = GetHypotenuse(other->GetW(), other->GetH()) / 2;
        alfa = atan(other->GetW() / other->GetH()) * RAD_DEG;

        angle = alfa + other->GetA();
        cocos2d::Point b1(other->GetX() + radius * sin(angle * DEG_RAD), other->GetY() + radius * cos(angle * DEG_RAD));
        angle += 180;
        cocos2d::Point b3(other->GetX() + radius * sin(angle * DEG_RAD), other->GetY() + radius * cos(angle * DEG_RAD));
        angle = 180 - alfa + other->GetA();
        cocos2d::Point b2(other->GetX() + radius * sin(angle * DEG_RAD), other->GetY() + radius * cos(angle * DEG_RAD));
        angle += 180;
        cocos2d::Point b4(other->GetX() + radius * sin(angle * DEG_RAD), other->GetY() + radius * cos(angle * DEG_RAD));

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
    else {
        return false;
    }
}

void rtm::WorldObject::PositionInit_()
{
    OnXUpdate_();
    OnYUpdate_();
    OnAUpdate_();
    OnWUpdate_();
    OnHUpdate_();
}

void rtm::WorldObject::OnPositionUpdate_()
{
    if (x_ != prevX_) OnXUpdate_();
    if (y_ != prevY_) OnYUpdate_();
    if (a_ != prevA_) OnAUpdate_();
    if (w_ != prevW_) OnWUpdate_();
    if (h_ != prevH_) OnHUpdate_();
}

void rtm::WorldObject::OnXUpdate_()
{
    SetSpriteX_(x_);
    prevX_ = x_;
}

void rtm::WorldObject::OnYUpdate_()
{
    SetSpriteY_(y_);
    prevY_ = y_;
}

void rtm::WorldObject::OnAUpdate_()
{
    SetSpriteA_(a_);
    prevA_ = a_;
}

void rtm::WorldObject::OnWUpdate_()
{
    prevW_ = w_;
}

void rtm::WorldObject::OnHUpdate_()
{
    prevH_ = h_;
}

bool rtm::WorldObject::IsNear_(WorldObject * other) const
{
    float distance = GetHypotenuse(GetX() - other->GetX(), GetY() - other->GetY());
    float radius = GetHypotenuse(GetW() / 2, GetH() / 2);
    float otherRadius = GetHypotenuse(other->GetW() / 2, other->GetH() / 2);
    return distance <= radius + otherRadius;
}

void rtm::WorldObject::SetSpriteX_(float x)
{
    if (sprite_ != nullptr) {
        sprite_->setPositionX(x);
    }
}

void rtm::WorldObject::SetSpriteY_(float y)
{
    if (sprite_ != nullptr) {
        sprite_->setPositionY(y);
    }
}

void rtm::WorldObject::SetSpriteA_(float a)
{
    if (sprite_ != nullptr) {
        sprite_->setRotation(a);
    }
}

float rtm::GetHypotenuse(float a, float b)
{
    return sqrt(a*a + b*b);
}

float Area(cocos2d::Point a, cocos2d::Point b, cocos2d::Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool RangesHaveIntersection(float a1, float a2, float b1, float b2) {
    if (a1 > a2) {
        std::swap(a1, a2);
    }
    if (b1 > b2) {
        std::swap(b1, b2);
    }
    return max(a1, b1) <= min(a2, b2);
}

bool rtm::HaveIntersection(cocos2d::Point a1, cocos2d::Point a2, cocos2d::Point b1, cocos2d::Point b2)
{
    return 
        RangesHaveIntersection(a1.x, a2.x, b1.x, b2.x) &&
        RangesHaveIntersection(a1.y, a2.y, b1.y, b2.y) &&
        Area(a1, a2, b1) * Area(a1, a2, b2) <= 0 &&
        Area(b1, b2, a1) * Area(b1, b2, a2) <= 0;
}