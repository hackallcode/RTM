#include "WorldScene.h"
#include "WorldController.h"
#include "StaticObject.h"
#include "DynamicObject.h"

rtm::WorldScene* rtm::WorldScene::globalScene_{ nullptr };

rtm::WorldScene* rtm::WorldScene::Create()
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

bool rtm::WorldScene::init()
{
    //////////////////////////////
    // 1. Super init first

    if (!Scene::init())
    {
        return false;
    }

    //////////////////////////////
    // 2. Variables

    backgroundLayer_ = cocos2d::Layer::create();
    background_ = nullptr;
    mainLayer_ = cocos2d::Layer::create();
    map_ = std::make_unique<WorldController>(this);
    clickTime_ = 0.f;
    viewColumn_ = 0;
    viewRow_ = 0;
    isCtrlPressed_ = false;
    isAltPressed_ = false;
    isUpArrowPressed_ = false;
    isRightArrowPressed_ = false;
    isDownArrowPressed_ = false;
    isLeftArrowPressed_ = false;
    OpenMap_();

    addChild(backgroundLayer_, BACKGROUND_Z_ORDER);
    addChild(mainLayer_, MAIN_Z_ORDER);
    WorldScene::globalScene_ = this;

    //////////////////////////////
    // 3. Listeners 

    cocos2d::EventListenerKeyboard* listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyPressed = WorldScene::KeyPressed_;
    listener->onKeyReleased = WorldScene::KeyReleased_;
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->scheduleUpdate();

    return true;
}

void rtm::WorldScene::update(float time)
{
    map_->Update(time);

    clickTime_ += time;
    if (clickTime_ > 0.15f) {
        if (isUpArrowPressed_) ShiftUp_();
        if (isRightArrowPressed_) ShiftRight_();
        if (isDownArrowPressed_) ShiftDown_();
        if (isLeftArrowPressed_) ShiftLeft_();
    }
}

void rtm::WorldScene::OpenMap_()
{
    map_->LoadMap(MapNumberNo1);
    SetDefaultPosition_();
    SetDefaultScale_();
    SetDefaultSpeed_();
}

void rtm::WorldScene::Restart_()
{
    map_->Reset();
}

void rtm::WorldScene::SetDefaultPosition_()
{
    viewColumn_ = 0;
    viewRow_ = 0;
    UpdatePosition_();
}

void rtm::WorldScene::ShiftUp_()
{
    clickTime_ = 0.f;
    ++viewRow_;
    UpdatePosition_();
}

void rtm::WorldScene::ShiftRight_()
{
    clickTime_ = 0.f;
    ++viewColumn_;
    UpdatePosition_();
}

void rtm::WorldScene::ShiftDown_()
{
    clickTime_ = 0.f;
    --viewRow_;
    UpdatePosition_();
}

void rtm::WorldScene::ShiftLeft_()
{
    clickTime_ = 0.f;
    --viewColumn_;
    UpdatePosition_();
}

void rtm::WorldScene::UpdatePosition_()
{
    float scaleFactor{ mainLayer_->getScale() };
    float cellSize{ static_cast<int>(CELL_SIZE) * scaleFactor };

    // X Shift
    float left{ getBoundingBox().getMidX() * (scaleFactor - 1.f) };
    float right{ left + getContentSize().width };
    float maxX{ map_->GetColumnsCount() * cellSize };

    float colShift{ viewColumn_ * cellSize };
    if (left + colShift < 0.f) {
        viewColumn_ += (0 - (left + colShift)) / cellSize;
    }
    else if (right + colShift > maxX) {
        viewColumn_ -= (right + colShift - maxX) / cellSize;
    }
    mainLayer_->setPositionX(-viewColumn_ * cellSize);

    // Y Shift
    float bottom{ getBoundingBox().getMidY() * (scaleFactor - 1.f) };
    float top{ bottom + getContentSize().height };
    float maxY{ map_->GetRowsCount() * cellSize };

    float rowShift{ viewRow_ * cellSize };
    if (bottom + rowShift < 0.f) {
        viewRow_ += (0 - (bottom + rowShift)) / cellSize;
    }
    else if (top + rowShift > maxY) {
        viewRow_ -= (top + rowShift - maxY) / cellSize;
    }
    mainLayer_->setPositionY(-viewRow_ * cellSize);
}

void rtm::WorldScene::SetDefaultScale_()
{
    mainLayer_->setScale(1.f);
    UpdatePosition_();
}

void rtm::WorldScene::IncreaseScale_()
{
    mainLayer_->setScale(mainLayer_->getScale() * 2.f);
    UpdatePosition_();
}

void rtm::WorldScene::DecreaseScale_()
{
    mainLayer_->setScale(mainLayer_->getScale() * 0.5f);
    UpdatePosition_();
}

void rtm::WorldScene::SetDefaultSpeed_()
{
    map_->SetTimeFactor(1.f);
}

void rtm::WorldScene::IncreaseSpeed_()
{
    map_->SetTimeFactor(std::min(MAX_TIME_FACTOR, map_->GetTimeFactor() * 2.f));
}

void rtm::WorldScene::DecreaseSpeed_()
{
    map_->SetTimeFactor(std::max(MIN_TIME_FACTOR, map_->GetTimeFactor() * 0.5f));
}

rtm::WorldControllerUnique& rtm::WorldScene::GetMap_()
{
    return map_;
}

cocos2d::Layer* rtm::WorldScene::GetMainLayer() const
{
    return mainLayer_;
}

void rtm::WorldScene::SetBackground(std::string const& filename)
{
    if (background_ != nullptr) {
        backgroundLayer_->removeChild(background_);
    }
    background_ = cocos2d::Sprite::create(filename);
    background_->setAnchorPoint(cocos2d::Vec2{ 0, 0 });
    backgroundLayer_->addChild(background_);
}

void rtm::WorldScene::SetBackground(BackgroundNumber number)
{
    SetBackground(GetBackgroundFile_(number));
}

std::string rtm::WorldScene::GetBackgroundFile_(BackgroundNumber number)
{
    std::string filename{ BACKGROUND_FILENAME_MASK };
    auto it{ filename.find("%No%") };
    filename.replace(it, 4, std::to_string(number));

    return filename;
}

void rtm::WorldScene::KeyPressed_(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event)
{
    if (globalScene_ == nullptr) {
        return;
    }
    auto& map = globalScene_->GetMap_();

    switch (code)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE:
        cocos2d::Director::getInstance()->popScene();
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_CTRL:
        globalScene_->isCtrlPressed_ = true;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_ALT:
        globalScene_->isAltPressed_ = true;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        globalScene_->ShiftUp_();
        globalScene_->isUpArrowPressed_ = true;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        globalScene_->ShiftRight_();
        globalScene_->isRightArrowPressed_ = true;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        globalScene_->ShiftDown_();
        globalScene_->isDownArrowPressed_ = true;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        globalScene_->ShiftLeft_();
        globalScene_->isLeftArrowPressed_ = true;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_0:
        if (globalScene_->isCtrlPressed_) {
            globalScene_->SetDefaultScale_();
        }
        else if (globalScene_->isAltPressed_) {
            globalScene_->SetDefaultSpeed_();
        }
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_MINUS:
        if (globalScene_->isCtrlPressed_) {
            globalScene_->DecreaseScale_();
        }
        else if (globalScene_->isAltPressed_) {
            globalScene_->DecreaseSpeed_();
        }
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_EQUAL:
        if (globalScene_->isCtrlPressed_) {
            globalScene_->IncreaseScale_();
        }
        else if (globalScene_->isAltPressed_) {
            globalScene_->IncreaseSpeed_();
        }
        break;

    case cocos2d::EventKeyboard::KeyCode::KEY_O:
        if (globalScene_->isCtrlPressed_) {
            globalScene_->OpenMap_();
        }
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_P:
        if (map->IsPause()) {
            map->Play();
        }
        else {
            map->Pause();
        }
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_R:
        globalScene_->Restart_();
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
        map->SpawnCar();
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
        map->RemoveAccidents();
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_F1:
        map->RemoveVehicles();
        break;
    }
}

void rtm::WorldScene::KeyReleased_(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * event)
{
    if (globalScene_ == nullptr) {
        return;
    }

    switch (code)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_CTRL:
        globalScene_->isCtrlPressed_ = false;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_ALT:
        globalScene_->isAltPressed_ = false;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        globalScene_->isUpArrowPressed_ = false;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        globalScene_->isRightArrowPressed_ = false;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        globalScene_->isDownArrowPressed_ = false;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        globalScene_->isLeftArrowPressed_ = false;
        break;
    }
}
