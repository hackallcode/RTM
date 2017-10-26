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
    map_ = std::make_unique<WorldController>(this);
    
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
    getMap()->Reset();
}

std::unique_ptr<rtm::WorldController>& rtm::World::getMap()
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
    
    auto& map = GLOBAL_WORLD_SCENE->getMap();
    if (code == cocos2d::EventKeyboard::KeyCode::KEY_1) {
        map->caseNum = 1;
        map->AddCar(CarTypeNo1, 10, 10, ANGLE_RIGHT);
        map->AddCar(CarTypeNo1, 30, 10, ANGLE_LEFT);
        map->AddCar(CarTypeNo5, 10, 20, ANGLE_RIGHT);
        map->AddCar(CarTypeNo5, 30, 20, ANGLE_LEFT);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_2) {
        map->caseNum = 2;
        map->AddCar(CarTypeNo1, 20, 10, ANGLE_RIGHT);
        map->AddCar(CarTypeNo5, 10, 10, ANGLE_RIGHT);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_3) {
        map->caseNum = 3;
        map->AddCar(CarTypeNo1, 10, 10, ANGLE_RIGHT);
        map->AddCar(CarTypeNo1, 11, 11, ANGLE_RIGHT);
        map->AddCar(CarTypeNo5, 10, 20, ANGLE_RIGHT);
        map->AddCar(CarTypeNo5, 11, 21, ANGLE_RIGHT);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_4) {
        map->caseNum = 4;
        map->AddCar(CarTypeNo1, 10, 11, ANGLE_RIGHT);
        map->AddCar(CarTypeNo1, 11, 10, ANGLE_RIGHT);
        map->AddCar(CarTypeNo5, 10, 21, ANGLE_RIGHT);
        map->AddCar(CarTypeNo5, 11, 20, ANGLE_RIGHT);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_5) {
        map->caseNum = 5;
        map->AddCar(CarTypeNo1, 10, 15, ANGLE_RIGHT);
        map->AddCar(CarTypeNo1, 15, 10, ANGLE_TOP);
        map->AddCar(CarTypeNo5, 25, 15, ANGLE_RIGHT);
        map->AddCar(CarTypeNo5, 30, 10, ANGLE_TOP);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_6) {
        map->caseNum = 6;
        map->AddCar(CarTypeNo1, 15, 15, ANGLE_LEFT);
        map->AddCar(CarTypeNo1, 10, 10, ANGLE_TOP);
        map->AddCar(CarTypeNo5, 30, 15, ANGLE_LEFT);
        map->AddCar(CarTypeNo5, 25, 10, ANGLE_TOP);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_B) {
        size_t cols = GLOBAL_WORLD_SCENE->getMap()->GetColumnsCount();
        size_t rows = GLOBAL_WORLD_SCENE->getMap()->GetRowsCount();
        map->AddBuilding((rtm::BuildingType) (rand() % 2 + 1), rand() % cols, rand() % rows, ANGLE_TOP);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_M) {
        for (size_t i = 0; i < 100; ++i) {
            size_t cols = GLOBAL_WORLD_SCENE->getMap()->GetColumnsCount();
            size_t rows = GLOBAL_WORLD_SCENE->getMap()->GetRowsCount();
            map->AddCar((rtm::CarType) (rand() % 5 + 1), rand() % cols, rand() % rows, rand() % 360 * DEG_RAD);
        }
    }
}