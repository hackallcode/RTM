#include "WorldScene.h"
#include "WorldController.h"

rtm::WorldController::WorldController()
    : scene_{ nullptr }
    , columnsCount_{ 0 }
    , rowsCount_{ 0 }
    , deltaTime_{ 0.f }
    , coatingObjects_{}
    , staticObjects_{}
    , dynamicObjects_{}
{}

rtm::WorldController::WorldController(World* const scene)
    : scene_{ scene }
    , columnsCount_{ static_cast<size_t>(scene_->getContentSize().width / CELL_SIZE) }
    , rowsCount_{ static_cast<size_t>(scene_->getContentSize().height / CELL_SIZE) }
    , deltaTime_{ 0.f }
    , coatingObjects_{ columnsCount_ }
    , staticObjects_{}
    , dynamicObjects_{}
{
    // Set background
    cocos2d::Sprite* background = cocos2d::Sprite::create(MAP_BACKGROUND_FILE);
    background->setAnchorPoint(cocos2d::Vec2{ 0, 0 });
    scene_->addChild(background, BACKGROUND_Z_ORDER);

    // Init coating objects array
    /*for (auto& col : coatingObjects_) {
        col = CoatingVector{ rowsCount_ };
        for (auto& elem : col) {
            elem.reset(new CoatingObject());
        }
    }*/
    for (int i = 0; i < coatingObjects_.size(); ++i) {
        coatingObjects_[i] = CoatingVector{ rowsCount_ };
        for (int j = 0; j < coatingObjects_[i].size(); ++j) {
            coatingObjects_[i][j].reset(new RoadObject{ RoadTypeNo1, i, j, ANGLE_RIGHT });
            scene_->addChild(coatingObjects_[i][j]->GetSprite(), COATING_Z_ORDER);
        }
    }
}

rtm::WorldController::WorldController(World* const scene, std::string const& filename)
    : WorldController{ scene }
{
    // Spawn objects
    // ...
}

rtm::WorldController::WorldController(World* const scene, MapNumber number)
    : WorldController{ scene, GetClassFile_(number) }
{}

void rtm::WorldController::Update(float time)
{
    deltaTime_ = time;

    CheckCollisions(dynamicObjects_, staticObjects_);
    for (auto& obj : dynamicObjects_) {
        obj->Update(this);
    }
}

size_t rtm::WorldController::GetColumnsCount() const
{
    return columnsCount_;
}

size_t rtm::WorldController::GetRowsCount() const
{
    return rowsCount_;
}

float rtm::WorldController::GetDeltaTime() const
{
    return deltaTime_;
}

rtm::CoatingUnique& rtm::WorldController::GetCoating(int column, int row)
{
    return coatingObjects_.at(column).at(row);
}

std::vector<rtm::StaticUnique>& rtm::WorldController::GetStaticObjects()
{
    return staticObjects_;
}

std::vector<rtm::DynamicUnique>& rtm::WorldController::GetDynamicObjects()
{
    return dynamicObjects_;
}

void rtm::WorldController::Reset()
{
    RemoveDynamicObjects_();
}

void rtm::WorldController::AddRoad(RoadType type, int column, int row, float angle)
{
    coatingObjects_[column][row].reset(new RoadObject{ type, column, row, angle });
    scene_->addChild(coatingObjects_[column][row]->GetSprite(), COATING_Z_ORDER);
}

void rtm::WorldController::AddBuilding(BuildingType type, int column, int row, float angle)
{
    staticObjects_.push_back(std::make_unique<BuildingObject>(type, column, row, angle));
    scene_->addChild(staticObjects_.back()->GetSprite(), BUILDING_Z_ORDER);
}

void rtm::WorldController::AddCar(CarType type, int column, int row, float angle)
{
    dynamicObjects_.push_back(std::make_unique<CarObject>(type, column, row, angle));
    scene_->addChild(dynamicObjects_.back()->GetSprite(), VEHICLE_Z_ORDER);
}

void rtm::WorldController::RemoveStaticObjects_()
{
    while (staticObjects_.size() > 0) {
        scene_->removeChild(staticObjects_.back()->GetSprite());
        staticObjects_.pop_back();
    }
}

void rtm::WorldController::RemoveDynamicObjects_()
{
    while (dynamicObjects_.size() > 0) {
        scene_->removeChild(dynamicObjects_.back()->GetSprite());
        dynamicObjects_.pop_back();
    }
}

std::string rtm::WorldController::GetClassFile_(MapNumber number)
{
    switch (number) {
    case MapNumberNo1:
        return MAP_NO_1_FILE;
    default:
        return MAP_NO_0_FILE;
    }
}

int rtm::PixelToCell(float coordinate)
{
    return static_cast<int>(floor(coordinate / CELL_SIZE));
}

float rtm::CellToPixel(int cellNumber)
{
    return (cellNumber + 0.5f) * CELL_SIZE;
}