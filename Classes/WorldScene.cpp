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
    map_ = std::make_unique<MapController>(this);
    
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
    getMap()->Update(time);
}

void rtm::World::restart()
{
    getMap()->reset();
}

std::unique_ptr<rtm::MapController>& rtm::World::getMap()
{
    return map_;
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
        for (size_t i = 5; i <= 5; ++i) {
            size_t w = 25;
            size_t h = 4 + i * 4;
            GLOBAL_WORLD_SCENE->getMap()->addCar((rtm::CarType) i, w, h, ANGLE_RIGHT);
        }
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_B) {
        size_t w = GLOBAL_WORLD_SCENE->getContentSize().width / CELL_SIZE;
        size_t h = GLOBAL_WORLD_SCENE->getContentSize().height / CELL_SIZE;
        GLOBAL_WORLD_SCENE->getMap()->addBuilding((rtm::BuildingType) (rand() % 2 + 1), rand() % w, rand() % h, ANGLE_TOP);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_M) {
        for (size_t i = 0; i < 100; ++i) {
            size_t w = GLOBAL_WORLD_SCENE->getContentSize().width / CELL_SIZE - 2;
            size_t h = GLOBAL_WORLD_SCENE->getContentSize().height / CELL_SIZE - 2;
            GLOBAL_WORLD_SCENE->getMap()->addCar((rtm::CarType) (rand() % 5 + 1), rand() % w, rand() % h, rand() % 360 * DEG_RAD);
        }
    }
}