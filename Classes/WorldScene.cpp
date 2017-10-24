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

    // Set background
    cocos2d::Sprite* background = cocos2d::Sprite::create("res/background.png");
    background->setAnchorPoint(cocos2d::Vec2(0, 0));
    addChild(background, BACKGROUND_Z_ORDER);

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

    CheckCollisions(getDynamicObjects(), getStaticObjects());

    for (auto& obj : getDynamicObjects()) {
        obj->Update(this);
    }
}

float rtm::World::getMissedTime() const
{
    return missedTime_;
}

float rtm::World::getWidth() const
{
    return getContentSize().width;
}

float rtm::World::getHeight() const
{
    return getContentSize().height;
}

void rtm::World::restart()
{
    removeAllObjects_();
}

void rtm::World::spawnBuilding(BuildingType type, int row, int column, float angle)
{
    addBuilding_(type, row, column, angle);
}

std::vector<std::unique_ptr<rtm::StaticObject>>& rtm::World::getStaticObjects()
{
    return staticObjs_;
}

void rtm::World::spawnCar(CarType type, int row, int column, float angle)
{
    addCar_(type, row, column, angle);
}

std::vector<std::unique_ptr<rtm::DynamicObject>>& rtm::World::getDynamicObjects()
{
    return dynamicObjs_;
}

void rtm::World::addBuilding_(BuildingType type, int row, int column, float angle)
{
    staticObjs_.push_back(std::make_unique<BuildingObject>(type, row, column, angle));
    addChild(staticObjs_.back()->GetSprite(), BUILDING_Z_ORDER);
}

void rtm::World::removeStaticObjects_()
{
    while (staticObjs_.size() > 0) {
        removeChild(staticObjs_.back()->GetSprite());
        staticObjs_.pop_back();
    }
}

void rtm::World::addCar_(CarType type, int row, int column, float angle)
{
    dynamicObjs_.push_back(std::make_unique<CarObject>(type, row, column, angle));
    addChild(dynamicObjs_.back()->GetSprite(), VEHICLE_Z_ORDER);
}

void rtm::World::removeDynamicObjects_()
{
    while (dynamicObjs_.size() > 0) {
        removeChild(dynamicObjs_.back()->GetSprite());
        dynamicObjs_.pop_back();
    }
}

void rtm::World::removeAllObjects_()
{
    removeStaticObjects_();
    removeDynamicObjects_();
}

void rtm::keyListener(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event)
{
    if (GLOBAL_WORLD_SCENE == nullptr) {
        return;
    }

    if (code == cocos2d::EventKeyboard::KeyCode::KEY_F1) {
        GLOBAL_WORLD_SCENE->restart();
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_C) {
        for (size_t i = 1; i <= 5; ++i) {
            size_t w = 25;
            size_t h = 4 + i * 4;
            GLOBAL_WORLD_SCENE->spawnCar((rtm::CarType) i, w, h, ANGLE_RIGHT);
        }
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_B) {
        size_t w = GLOBAL_WORLD_SCENE->getContentSize().width / CELL_SIZE;
        size_t h = GLOBAL_WORLD_SCENE->getContentSize().height / CELL_SIZE;
        GLOBAL_WORLD_SCENE->spawnBuilding((rtm::BuildingType) (rand() % 2 + 1), rand() % w, rand() % h, ANGLE_TOP);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_M) {
        for (size_t i = 0; i < 100; ++i) {
            size_t w = GLOBAL_WORLD_SCENE->getContentSize().width / CELL_SIZE - 2;
            size_t h = GLOBAL_WORLD_SCENE->getContentSize().height / CELL_SIZE - 2;
            GLOBAL_WORLD_SCENE->spawnCar((rtm::CarType) (rand() % 5 + 1), rand() % w, rand() % h, rand() % 360 * DEG_RAD);
        }
    }
}