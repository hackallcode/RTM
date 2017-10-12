#include "WorldScene.h"

rtm::World* GLOBAL_WORLD_SCENE = nullptr;

cocos2d::Scene* rtm::World::createScene()
{
    // Создание сцены со слоем
    cocos2d::Scene* scene = cocos2d::Scene::create();
    World* layer = World::create();
    scene->addChild(layer);

    return scene;
}

rtm::World* rtm::World::create()
{
    World* result = new(std::nothrow) World();
    if (result && result->init())
    {
        result->autorelease();
        return result;
    }
    else
    {
        delete result;
        return nullptr;
    }
}

// on "init" you need to initialize your instance
bool rtm::World::init()
{
    //////////////////////////////
    // 1. Super init first

    if (!Layer::init())
    {
        return false;
    }

    //////////////////////////////
    // 2. Objects

    initMap_();

    //////////////////////////////
    // 3. Listeners 

    GLOBAL_WORLD_SCENE = this;

    cocos2d::EventListenerKeyboard* listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyPressed = keyListener;

    this->scheduleUpdate();
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void rtm::World::update(float time)
{
    missedTime_ = time;

    for (auto& obj : objects_) {
        obj->Update(this);
    }
}

float rtm::World::getMissedTime() const
{
    return missedTime_;
}

std::vector<std::unique_ptr<rtm::WorldObject>>& rtm::World::getObjects()
{
    return objects_;
}

void rtm::World::spawnCar(CarType type, size_t row, size_t column)
{
    addCar_(type, row, column);
}

void rtm::World::spawnBuilding(BuildingType type, size_t row, size_t column)
{
    addBuilding_(type, row, column);
}

void rtm::World::initNewGame()
{
    removeAllObjects_();
}

void rtm::World::removeAllObjects_()
{
    while (objects_.size() > 0) {
        removeChild(objects_.back()->GetSprite());
        objects_.pop_back();
    }
}

void rtm::World::addBuilding_(BuildingType type, size_t row, size_t column)
{
    objects_.push_back(std::make_unique<BuildingObject>(type, row, column));
    addChild(objects_.back()->GetSprite(), BUILDING_Z_ORDER);
}

void rtm::World::addCar_(CarType type, size_t row, size_t column)
{
    objects_.push_back(std::make_unique<CarObject>(type, row, column));
    addChild(objects_.back()->GetSprite(), VEHICLE_Z_ORDER);
}

void rtm::World::initMap_()
{
    // Set background
    cocos2d::CCSprite* background = cocos2d::CCSprite::create("res/background.png");
    background->setAnchorPoint(cocos2d::Vec2(0, 0));
    addChild(background, -1);
}

void rtm::keyListener(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event)
{
    if (GLOBAL_WORLD_SCENE == nullptr) {
        return;
    }

    if (code == cocos2d::EventKeyboard::KeyCode::KEY_F1) {
        GLOBAL_WORLD_SCENE->initNewGame();
    }
    if (code == cocos2d::EventKeyboard::KeyCode::KEY_C) {
        GLOBAL_WORLD_SCENE->spawnCar(rtm::CarTypeNo1, 1, 1);
    }
    if (code == cocos2d::EventKeyboard::KeyCode::KEY_B) {
        GLOBAL_WORLD_SCENE->spawnBuilding(rtm::BuildingTypeNo1, 2, 2);
    }
}