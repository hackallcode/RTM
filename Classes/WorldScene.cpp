#include "WorldScene.h"
#include "WorldController.h"
#include "StaticObject.h"
#include "DynamicObject.h"

rtm::WorldScene* GLOBAL_WORLD_SCENE = nullptr;

cocos2d::Scene* rtm::WorldScene::createScene()
{
    // Создание сцены со слоем
    cocos2d::Scene* scene = cocos2d::Scene::create();
    WorldScene* layer = WorldScene::create();
    scene->addChild(layer);

    return scene;
}

rtm::WorldScene* rtm::WorldScene::create()
{
    WorldScene* result = new(std::nothrow) WorldScene();
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
bool rtm::WorldScene::init()
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

void rtm::WorldScene::update(float time)
{
    getMap()->Update(time);
}

void rtm::WorldScene::restart()
{
    getMap()->Reset();
}

rtm::WorldControllerUnique& rtm::WorldScene::getMap()
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
    
    auto& map = GLOBAL_WORLD_SCENE->getMap();
    
    static bool isTimeFactorWaiting{ false };    
    if (isTimeFactorWaiting) {
        isTimeFactorWaiting = false;
        int intCode{ static_cast<int>(code) };
        int rangeBegin{ static_cast<int>(cocos2d::EventKeyboard::KeyCode::KEY_0) };
        int rangeEnd{ static_cast<int>(cocos2d::EventKeyboard::KeyCode::KEY_4) };
            
        if (rangeBegin <= intCode && intCode <= rangeEnd) {
            map->SetTimeFactor(intCode - rangeBegin);
        }
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_B) {
        isTimeFactorWaiting = true;
    }

    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_M) {
        map->LoadMap(MapNumberNo1);
    }
    else if (code == cocos2d::EventKeyboard::KeyCode::KEY_T) {
        map->LoadMap(MapNumberNo2);
    }
}