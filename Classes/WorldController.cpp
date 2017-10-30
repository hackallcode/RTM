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
    for (auto& col : coatingObjects_) {
        col = CoatingVector{ rowsCount_ };
    }

    // Temp map
    coatingObjects_[5][20].reset(new RoadObject{ RoadTypeNo2, 5, 20, ANGLE_TOP });
    scene_->addChild(coatingObjects_[5][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[6][20].reset(new RoadObject{ RoadTypeNo1, 6, 20, ANGLE_RIGHT });	
    scene_->addChild(coatingObjects_[6][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[7][20].reset(new RoadObject{ RoadTypeNo1, 7, 20, ANGLE_RIGHT });	
    scene_->addChild(coatingObjects_[7][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[8][20].reset(new RoadObject{ RoadTypeNo1, 8, 20, ANGLE_RIGHT });	
    scene_->addChild(coatingObjects_[8][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[9][20].reset(new RoadObject{ RoadTypeNo1, 9, 20, ANGLE_RIGHT });	
    scene_->addChild(coatingObjects_[9][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[10][20].reset(new RoadObject{ RoadTypeNo1, 10, 20, ANGLE_RIGHT });	
    scene_->addChild(coatingObjects_[10][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[11][20].reset(new RoadObject{ RoadTypeNo2, 11, 20, ANGLE_RIGHT });	
    scene_->addChild(coatingObjects_[11][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[11][19].reset(new RoadObject{ RoadTypeNo1, 11, 19, ANGLE_BOTTOM });	
    scene_->addChild(coatingObjects_[11][19]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[11][18].reset(new RoadObject{ RoadTypeNo1, 11, 18, ANGLE_BOTTOM });	
    scene_->addChild(coatingObjects_[11][18]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[11][17].reset(new RoadObject{ RoadTypeNo1, 11, 17, ANGLE_BOTTOM });	
    scene_->addChild(coatingObjects_[11][17]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[11][16].reset(new RoadObject{ RoadTypeNo1, 11, 16, ANGLE_BOTTOM });	
    scene_->addChild(coatingObjects_[11][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[11][15].reset(new RoadObject{ RoadTypeNo1, 11, 15, ANGLE_BOTTOM });	
    scene_->addChild(coatingObjects_[11][15]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[11][14].reset(new RoadObject{ RoadTypeNo2, 11, 14, ANGLE_BOTTOM });	
    scene_->addChild(coatingObjects_[11][14]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[10][14].reset(new RoadObject{ RoadTypeNo1, 10, 14, ANGLE_LEFT });	
    scene_->addChild(coatingObjects_[10][14]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[9][14].reset(new RoadObject{ RoadTypeNo1, 9, 14, ANGLE_LEFT });	
    scene_->addChild(coatingObjects_[9][14]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[8][14].reset(new RoadObject{ RoadTypeNo1, 8, 14, ANGLE_LEFT });	
    scene_->addChild(coatingObjects_[8][14]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[7][14].reset(new RoadObject{ RoadTypeNo1, 7, 14, ANGLE_LEFT });	
    scene_->addChild(coatingObjects_[7][14]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[6][14].reset(new RoadObject{ RoadTypeNo1, 6, 14, ANGLE_LEFT });	
    scene_->addChild(coatingObjects_[6][14]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[5][14].reset(new RoadObject{ RoadTypeNo2, 5, 14, ANGLE_LEFT });	
    scene_->addChild(coatingObjects_[5][14]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[5][15].reset(new RoadObject{ RoadTypeNo1, 5, 15, ANGLE_TOP });	
    scene_->addChild(coatingObjects_[5][15]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[5][16].reset(new RoadObject{ RoadTypeNo1, 5, 16, ANGLE_TOP });	
    scene_->addChild(coatingObjects_[5][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[5][17].reset(new RoadObject{ RoadTypeNo1, 5, 17, ANGLE_TOP });	
    scene_->addChild(coatingObjects_[5][17]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[5][18].reset(new RoadObject{ RoadTypeNo1, 5, 18, ANGLE_TOP });	
    scene_->addChild(coatingObjects_[5][18]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[5][19].reset(new RoadObject{ RoadTypeNo1, 5, 19, ANGLE_TOP });	
    scene_->addChild(coatingObjects_[5][19]->GetSprite(), COATING_Z_ORDER);

    for (auto& col : coatingObjects_) {
        for (auto& elem : col) {
            if (!elem) {
                elem.reset(new CoatingObject());
            }
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