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
    map_ = std::make_unique<WorldController>(this, MapNumberNo1);

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

    if (code == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE) {
        cocos2d::Director::getInstance()->popScene();
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_R) {
        GLOBAL_WORLD_SCENE->restart();
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_F1) {
        GLOBAL_WORLD_SCENE->getMap()->RemoveCoatingObjects_();
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_F2) {
        GLOBAL_WORLD_SCENE->getMap()->RemoveStaticObjects_();
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_F3) {
        GLOBAL_WORLD_SCENE->getMap()->RemoveDynamicObjects_();
    }
    
    auto& map = GLOBAL_WORLD_SCENE->getMap();
    if (code == cocos2d::EventKeyboard::KeyCode::KEY_0) {
        SetCaseNumber(0);
        map->AddCar(CarTypeNo2, 30, 30, ANGLE_BOTTOM);
        map->AddCar(CarTypeNo2, 30, 15, ANGLE_TOP);
        map->AddCar(CarTypeNo2, 20, 10, ANGLE_RIGHT);
        map->AddCar(CarTypeNo2, 40, 10, ANGLE_LEFT);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_1) {
        SetCaseNumber(1);

        map->AddCar(CarTypeNo2, 20, 10, ANGLE_TOP);
        map->AddCar(CarTypeNo1, 20, 15, ANGLE_TOP);
        map->AddCar(CarTypeNo2, 20, 30, ANGLE_TOP);
        map->AddCar(CarTypeNo2, 20, 30, ANGLE_TOP);

        map->AddCar(CarTypeNo2, 25, 15, ANGLE_TOP);
        map->AddCar(CarTypeNo2, 25, 30, ANGLE_TOP);
        map->AddCar(CarTypeNo2, 25, 30, ANGLE_TOP);

        map->AddCar(CarTypeNo2, 30, 30, ANGLE_BOTTOM);
        map->AddCar(CarTypeNo1, 30, 25, ANGLE_BOTTOM);
        map->AddCar(CarTypeNo2, 30, 10, ANGLE_BOTTOM);
        map->AddCar(CarTypeNo2, 30, 10, ANGLE_BOTTOM);

        map->AddCar(CarTypeNo2, 35, 25, ANGLE_BOTTOM);
        map->AddCar(CarTypeNo2, 35, 10, ANGLE_BOTTOM);
        map->AddCar(CarTypeNo2, 35, 10, ANGLE_BOTTOM);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_2) {
        SetCaseNumber(2);

        map->AddCar(CarTypeNo2, 25, 10, ANGLE_RIGHT);
        map->AddCar(CarTypeNo1, 30, 10, ANGLE_RIGHT);
        map->AddCar(CarTypeNo2, 45, 10, ANGLE_RIGHT);
        map->AddCar(CarTypeNo2, 45, 10, ANGLE_RIGHT);

        map->AddCar(CarTypeNo2, 30, 15, ANGLE_RIGHT);
        map->AddCar(CarTypeNo2, 45, 15, ANGLE_RIGHT);
        map->AddCar(CarTypeNo2, 45, 15, ANGLE_RIGHT);

        map->AddCar(CarTypeNo2, 45, 20, ANGLE_LEFT);
        map->AddCar(CarTypeNo1, 40, 20, ANGLE_LEFT);
        map->AddCar(CarTypeNo2, 25, 20, ANGLE_LEFT);
        map->AddCar(CarTypeNo2, 25, 20, ANGLE_LEFT);

        map->AddCar(CarTypeNo2, 40, 25, ANGLE_LEFT);
        map->AddCar(CarTypeNo2, 25, 25, ANGLE_LEFT);
        map->AddCar(CarTypeNo2, 25, 25, ANGLE_LEFT);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_3) {
        SetCaseNumber(3);

        map->AddCar(CarTypeNo1, 15, 10, ANGLE_RIGHT);
        map->AddCar(CarTypeNo1, 16, 11, ANGLE_RIGHT);
        map->AddCar(CarTypeNo2, 15, 20, ANGLE_RIGHT);
        map->AddCar(CarTypeNo2, 16, 21, ANGLE_RIGHT);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_4) {
        SetCaseNumber(4);

        map->AddCar(CarTypeNo1, 15, 11, ANGLE_RIGHT);
        map->AddCar(CarTypeNo1, 16, 10, ANGLE_RIGHT);
        map->AddCar(CarTypeNo2, 15, 21, ANGLE_RIGHT);
        map->AddCar(CarTypeNo2, 16, 20, ANGLE_RIGHT);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_5) {
        SetCaseNumber(5);

        map->AddCar(CarTypeNo1, 15, 15, ANGLE_RIGHT);
        map->AddCar(CarTypeNo1, 20, 10, ANGLE_TOP);
        map->AddCar(CarTypeNo2, 30, 15, ANGLE_RIGHT);
        map->AddCar(CarTypeNo2, 35, 10, ANGLE_TOP);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_6) {
        SetCaseNumber(6);

        map->AddCar(CarTypeNo1, 20, 15, ANGLE_LEFT);
        map->AddCar(CarTypeNo1, 15, 10, ANGLE_TOP);
        map->AddCar(CarTypeNo2, 35, 15, ANGLE_LEFT);
        map->AddCar(CarTypeNo2, 30, 10, ANGLE_TOP);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_7) {
        SetCaseNumber(7);

        map->AddCar(CarTypeNo2, 30, 10, ANGLE_TOP);
        map->AddCar(CarTypeNo2, 15, 14, ANGLE_RIGHT);
        map->AddCar(CarTypeNo2, 30, 25, ANGLE_BOTTOM);
        map->AddCar(CarTypeNo2, 45, 14, ANGLE_LEFT);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_8) {
        SetCaseNumber(8);

        map->AddCar(CarTypeNo2, 27, 20, ANGLE_RIGHT);
        map->AddCar(CarTypeNo2, 33, 20, ANGLE_RIGHT);
        map->AddCar(CarTypeNo2, 33, 20, ANGLE_RIGHT);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_9) {
        SetCaseNumber(9);

        map->AddCar(CarTypeNo2, 32, 34, ANGLE_TOP);
        map->AddCar(CarTypeNo2, 61, 15, ANGLE_RIGHT);
        map->AddCar(CarTypeNo2, 32, 3, ANGLE_BOTTOM);
        map->AddCar(CarTypeNo2, 4, 15, ANGLE_LEFT);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_M) {
        map->LoadMap(MapNumberNo1);
    }
}