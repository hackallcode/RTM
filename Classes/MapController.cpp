#include "WorldScene.h"
#include "MapController.h"

rtm::MapController::MapController()
    : scene_(nullptr)
    , rowsCount_(0)
    , columnsCount_(0)
    , deltaTime_(0.f)
    , staticObjs_()
    , dynamicObjs_()
{}

rtm::MapController::MapController(World* const scene)
    : scene_(scene)
    , rowsCount_(scene_->getContentSize().height / CELL_SIZE)
    , columnsCount_(scene_->getContentSize().width / CELL_SIZE)
    , deltaTime_(0.f)
    , staticObjs_()
    , dynamicObjs_()
{
    // Set background
    cocos2d::Sprite* background = cocos2d::Sprite::create(MAP_BACKGROUND_FILE);
    background->setAnchorPoint(cocos2d::Vec2(0, 0));
    scene_->addChild(background, BACKGROUND_Z_ORDER);
}

rtm::MapController::MapController(World* const scene, std::string const& filename)
    : MapController(scene)
{
    // Spawn objects
    // ...
}

rtm::MapController::MapController(World* const scene, MapNumber number)
    : MapController(scene, GetClassFile_(number))
{}

void rtm::MapController::Update(float time)
{
    deltaTime_ = time;

    CheckCollisions(getDynamicObjects(), getStaticObjects());

    for (auto& obj : getDynamicObjects()) {
        obj->Update(this);
    }
}

size_t rtm::MapController::GetRow(float y)
{
    return static_cast<size_t>(round(y / CELL_SIZE));
}

size_t rtm::MapController::GetColumn(float x)
{
    return static_cast<size_t>(round(x / CELL_SIZE));
}

float rtm::MapController::GetDeltaTime() const
{
    return deltaTime_;
}

std::vector<std::unique_ptr<rtm::StaticObject>>& rtm::MapController::getStaticObjects()
{
    return staticObjs_;
}

std::vector<std::unique_ptr<rtm::DynamicObject>>& rtm::MapController::getDynamicObjects()
{
    return dynamicObjs_;
}

void rtm::MapController::reset()
{
    removeDynamicObjects_();
}

void rtm::MapController::addBuilding(BuildingType type, int row, int column, float angle)
{
    staticObjs_.push_back(std::make_unique<BuildingObject>(type, row, column, angle));
    scene_->addChild(staticObjs_.back()->GetSprite(), BUILDING_Z_ORDER);
}

void rtm::MapController::addCar(CarType type, int row, int column, float angle)
{
    dynamicObjs_.push_back(std::make_unique<CarObject>(type, row, column, angle));
    scene_->addChild(dynamicObjs_.back()->GetSprite(), VEHICLE_Z_ORDER);
}

void rtm::MapController::removeStaticObjects_()
{
    while (staticObjs_.size() > 0) {
        scene_->removeChild(staticObjs_.back()->GetSprite());
        staticObjs_.pop_back();
    }
}

void rtm::MapController::removeDynamicObjects_()
{
    while (dynamicObjs_.size() > 0) {
        scene_->removeChild(dynamicObjs_.back()->GetSprite());
        dynamicObjs_.pop_back();
    }
}

std::string rtm::MapController::GetClassFile_(MapNumber number)
{
    switch (number) {
    case MapNumberNo1:
        return MAP_NO_1_FILE;
    default:
        return MAP_NO_0_FILE;
    }
}