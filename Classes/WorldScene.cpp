#include "WorldScene.h"
#include "SimpleAudioEngine.h"

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

    cocos2d::EventListenerKeyboard * listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyPressed = keyListener;

    this->scheduleUpdate();
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void rtm::World::update(float time)
{
    missedTime_ = time;
}

float rtm::World::getMissedTime() const
{
    return missedTime_;
}

//std::vector<std::unique_ptr<rtm::VehicleObject>>& rtm::World::getVehicles()
//{
//    return vehicles_;
//}
//
//void rtm::World::addVehicle_(VehicleType id)
//{
//    vehicles_.push_back(std::unique_ptr<VehicleObject>(new VehicleObject(id)));
//
//    if (isRight) {
//        vehicles_.back()->SetRightAlignment(getContentSize().width);
//    }
//
//    addChild(vehicles_.back()->GetSprite(), ATTACKER_Z_ORDER);
//}
//
//void rtm::World::deleteAllVehicles_()
//{
//    while (vehicles_.size() > 0) {
//        removeChild(vehicles_.back()->GetSprite());
//        vehicles_.pop_back();
//    }
//}

void rtm::World::initNewGame()
{
    //deleteAllVehicles_();
}

void rtm::World::initMap_()
{
    // Set background
    cocos2d::CCSprite * background = cocos2d::CCSprite::create("res/background.png");
    background->setAnchorPoint(cocos2d::Vec2(0, 0));
    addChild(background, -1);

    // Vehicle
    //for (float i = 0; i < 24; ++i) {
    //    objects_.push_back(std::unique_ptr<BlockObject>(new BlockObject(80 * i, 0, BlockType::BasicBlockType)));
    //    addChild(objects_[i]->GetSprite(), BLOCK_Z_ORDER);
    //}
}

void rtm::keyListener(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * event)
{
    if (GLOBAL_WORLD_SCENE == nullptr) {
        return;
    }

    if (code == cocos2d::EventKeyboard::KeyCode::KEY_F1) {
        GLOBAL_WORLD_SCENE->initNewGame();
    }
}