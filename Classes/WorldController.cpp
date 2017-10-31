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
        for (auto& elem : col) {
            elem.reset(new CoatingObject());
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
    RemoveCoatingObjects_();
    RemoveStaticObjects_();
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

void rtm::WorldController::AddTestMap()
{
    coatingObjects_[24][11].reset(new RoadObject{ RoadTypeNo1, 24, 11, ANGLE_TOP });
    scene_->addChild(coatingObjects_[24][11]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[27][11].reset(new RoadObject{ RoadTypeNo1, 27, 11, ANGLE_TOP });
    scene_->addChild(coatingObjects_[27][11]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[33][11].reset(new RoadObject{ RoadTypeNo1, 33, 11, ANGLE_TOP });
    scene_->addChild(coatingObjects_[33][11]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[36][11].reset(new RoadObject{ RoadTypeNo1, 36, 11, ANGLE_TOP });
    scene_->addChild(coatingObjects_[36][11]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[20][13].reset(new RoadObject{ RoadTypeNo1, 20, 13, ANGLE_TOP });
    scene_->addChild(coatingObjects_[20][13]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[20][14].reset(new RoadObject{ RoadTypeNo1, 20, 14, ANGLE_TOP });
    scene_->addChild(coatingObjects_[20][14]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[20][15].reset(new RoadObject{ RoadTypeNo1, 20, 15, ANGLE_TOP });
    scene_->addChild(coatingObjects_[20][15]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[40][13].reset(new RoadObject{ RoadTypeNo1, 40, 13, ANGLE_TOP });
    scene_->addChild(coatingObjects_[40][13]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[40][14].reset(new RoadObject{ RoadTypeNo1, 40, 14, ANGLE_TOP });
    scene_->addChild(coatingObjects_[40][14]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[40][15].reset(new RoadObject{ RoadTypeNo1, 40, 15, ANGLE_TOP });
    scene_->addChild(coatingObjects_[40][15]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[26][17].reset(new RoadObject{ RoadTypeNo1, 26, 17, ANGLE_TOP });
    scene_->addChild(coatingObjects_[26][17]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[26][18].reset(new RoadObject{ RoadTypeNo1, 26, 18, ANGLE_TOP });
    scene_->addChild(coatingObjects_[26][18]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[26][19].reset(new RoadObject{ RoadTypeNo1, 26, 19, ANGLE_TOP });
    scene_->addChild(coatingObjects_[26][19]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[34][17].reset(new RoadObject{ RoadTypeNo1, 34, 17, ANGLE_TOP });
    scene_->addChild(coatingObjects_[34][17]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[34][18].reset(new RoadObject{ RoadTypeNo1, 34, 18, ANGLE_TOP });
    scene_->addChild(coatingObjects_[34][18]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[34][19].reset(new RoadObject{ RoadTypeNo1, 34, 19, ANGLE_TOP });
    scene_->addChild(coatingObjects_[34][19]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[25][10].reset(new RoadObject{ RoadTypeNo1, 25, 10, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[25][10]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[26][10].reset(new RoadObject{ RoadTypeNo1, 26, 10, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[26][10]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[34][10].reset(new RoadObject{ RoadTypeNo1, 34, 10, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[34][10]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[35][10].reset(new RoadObject{ RoadTypeNo1, 35, 10, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[35][10]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[21][12].reset(new RoadObject{ RoadTypeNo1, 21, 12, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[21][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[22][12].reset(new RoadObject{ RoadTypeNo1, 22, 12, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[22][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[23][12].reset(new RoadObject{ RoadTypeNo1, 23, 12, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[23][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[28][12].reset(new RoadObject{ RoadTypeNo1, 28, 12, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[28][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[29][12].reset(new RoadObject{ RoadTypeNo1, 29, 12, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[29][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[30][12].reset(new RoadObject{ RoadTypeNo1, 30, 12, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[30][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[31][12].reset(new RoadObject{ RoadTypeNo1, 31, 12, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[31][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[32][12].reset(new RoadObject{ RoadTypeNo1, 32, 12, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[32][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[37][12].reset(new RoadObject{ RoadTypeNo1, 37, 12, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[37][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[38][12].reset(new RoadObject{ RoadTypeNo1, 38, 12, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[38][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[39][12].reset(new RoadObject{ RoadTypeNo1, 39, 12, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[39][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[21][16].reset(new RoadObject{ RoadTypeNo1, 21, 16, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[21][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[22][16].reset(new RoadObject{ RoadTypeNo1, 22, 16, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[22][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[23][16].reset(new RoadObject{ RoadTypeNo1, 23, 16, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[23][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[24][16].reset(new RoadObject{ RoadTypeNo1, 24, 16, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[24][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[25][16].reset(new RoadObject{ RoadTypeNo1, 25, 16, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[25][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[35][16].reset(new RoadObject{ RoadTypeNo1, 35, 16, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[35][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[36][16].reset(new RoadObject{ RoadTypeNo1, 36, 16, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[36][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[37][16].reset(new RoadObject{ RoadTypeNo1, 37, 16, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[37][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[38][16].reset(new RoadObject{ RoadTypeNo1, 38, 16, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[38][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[39][16].reset(new RoadObject{ RoadTypeNo1, 39, 16, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[39][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[27][20].reset(new RoadObject{ RoadTypeNo1, 27, 20, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[27][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[28][20].reset(new RoadObject{ RoadTypeNo1, 28, 20, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[28][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[29][20].reset(new RoadObject{ RoadTypeNo1, 29, 20, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[29][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[30][20].reset(new RoadObject{ RoadTypeNo1, 30, 20, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[30][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[31][20].reset(new RoadObject{ RoadTypeNo1, 31, 20, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[31][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[32][20].reset(new RoadObject{ RoadTypeNo1, 32, 20, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[32][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[33][20].reset(new RoadObject{ RoadTypeNo1, 33, 20, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[33][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[27][12].reset(new RoadObject{ RoadTypeNo2, 27, 12, ANGLE_TOP });
    scene_->addChild(coatingObjects_[27][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[36][12].reset(new RoadObject{ RoadTypeNo2, 36, 12, ANGLE_TOP });
    scene_->addChild(coatingObjects_[36][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[20][16].reset(new RoadObject{ RoadTypeNo2, 20, 16, ANGLE_TOP });
    scene_->addChild(coatingObjects_[20][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[26][20].reset(new RoadObject{ RoadTypeNo2, 26, 20, ANGLE_TOP });
    scene_->addChild(coatingObjects_[26][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[24][12].reset(new RoadObject{ RoadTypeNo2, 24, 12, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[24][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[33][12].reset(new RoadObject{ RoadTypeNo2, 33, 12, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[33][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[40][16].reset(new RoadObject{ RoadTypeNo2, 40, 16, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[40][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[34][20].reset(new RoadObject{ RoadTypeNo2, 34, 20, ANGLE_RIGHT });
    scene_->addChild(coatingObjects_[34][20]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[27][10].reset(new RoadObject{ RoadTypeNo2, 27, 10, ANGLE_BOTTOM });
    scene_->addChild(coatingObjects_[27][10]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[36][10].reset(new RoadObject{ RoadTypeNo2, 36, 10, ANGLE_BOTTOM });
    scene_->addChild(coatingObjects_[36][10]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[40][12].reset(new RoadObject{ RoadTypeNo2, 40, 12, ANGLE_BOTTOM });
    scene_->addChild(coatingObjects_[40][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[26][16].reset(new RoadObject{ RoadTypeNo2, 26, 16, ANGLE_BOTTOM });
    scene_->addChild(coatingObjects_[26][16]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[24][10].reset(new RoadObject{ RoadTypeNo2, 24, 10, ANGLE_LEFT });
    scene_->addChild(coatingObjects_[24][10]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[33][10].reset(new RoadObject{ RoadTypeNo2, 33, 10, ANGLE_LEFT });
    scene_->addChild(coatingObjects_[33][10]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[20][12].reset(new RoadObject{ RoadTypeNo2, 20, 12, ANGLE_LEFT });
    scene_->addChild(coatingObjects_[20][12]->GetSprite(), COATING_Z_ORDER);
    coatingObjects_[34][16].reset(new RoadObject{ RoadTypeNo2, 34, 16, ANGLE_LEFT });
    scene_->addChild(coatingObjects_[34][16]->GetSprite(), COATING_Z_ORDER);
}

void rtm::WorldController::RemoveCoatingObjects_()
{
    for (auto& col : coatingObjects_) {
        for (auto& elem : col) {
            if (elem) {
                if (elem->GetSprite() != nullptr) {
                    scene_->removeChild(elem->GetSprite());
                    elem.reset(new CoatingObject());
                }
            }
        }
    }
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