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
    , columnsCount_{ static_cast<size_t>(trunc(scene_->getContentSize().width / CELL_SIZE)) }
    , rowsCount_{ static_cast<size_t>(trunc(scene_->getContentSize().height / CELL_SIZE)) }
    , deltaTime_{ 0.f }
    , coatingObjects_{ columnsCount_ + 2 * HIDDEN_AREA_SIZE }
    , staticObjects_{ columnsCount_ + 2 * HIDDEN_AREA_SIZE }
    , dynamicObjects_{}
{
    // Init coating objects array
    for (auto& col : coatingObjects_) {
        col = std::vector<CoatingUnique>{ rowsCount_ + 2 * HIDDEN_AREA_SIZE };
    }

    // Init static objects array
    for (auto& col : staticObjects_) {
        col = std::vector<StaticUnique>{ rowsCount_ + 2 * HIDDEN_AREA_SIZE };
    }
}

rtm::WorldController::WorldController(World* const scene, std::string const& filename)
    : WorldController{ scene }
{
    LoadMap(filename);
}

rtm::WorldController::WorldController(World* const scene, MapNumber number)
    : WorldController{ scene }
{
    LoadMap(number);
}

void rtm::WorldController::Update(float time)
{
    deltaTime_ = time;

    for (auto it = dynamicObjects_.begin(); it != dynamicObjects_.end();) {
        DynamicObject& obj{ **it };

        obj.Update(this);
        if (
            // Not visible
            (!IsVisibleColumn(PixelToCell(obj.GetX())) || !IsVisibleRow(PixelToCell(obj.GetY()))) &&
            // In center of cell
            IsInCenter(obj.GetX()) && IsInCenter(obj.GetY())
        ) {
            scene_->removeChild(obj.GetSprite());
            it = dynamicObjects_.erase(it);
        }
        else {
            ++it;
        }
    }
    CheckCollisions(this);
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

rtm::CoatingUnique& rtm::WorldController::GetCoatingObject(int column, int row)
{
    return coatingObjects_[column + HIDDEN_AREA_SIZE][row + HIDDEN_AREA_SIZE];
}

rtm::StaticUnique& rtm::WorldController::GetStaticObject(int column, int row)
{
    return staticObjects_[column + HIDDEN_AREA_SIZE][row + HIDDEN_AREA_SIZE];
}

std::vector<rtm::DynamicUnique>& rtm::WorldController::GetDynamicObjects()
{
    return dynamicObjects_;
}

bool rtm::WorldController::IsCorrectColumn(int column)
{
    return column >= -static_cast<int>(HIDDEN_AREA_SIZE) &&
        column < static_cast<int>(columnsCount_ + HIDDEN_AREA_SIZE);
}

bool rtm::WorldController::IsCorrectRow(int row)
{
    return row >= -static_cast<int>(HIDDEN_AREA_SIZE) &&
        row < static_cast<int>(columnsCount_ + HIDDEN_AREA_SIZE);
}

bool rtm::WorldController::IsVisibleColumn(int row)
{
    return row >= 0 && row < columnsCount_;
}

bool rtm::WorldController::IsVisibleRow(int row)
{
    return row >= 0 && row < rowsCount_;
}

bool rtm::WorldController::LoadMap(std::string const& filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        return false;
    }

    RemoveCoatingObjects_();
    RemoveStaticObjects_();
    RemoveDynamicObjects_();

    std::string buf{};
    while (std::getline(fin, buf)) {
        auto nums = Split(buf);
        if (nums.empty()) {
            return false;
        }

        switch (nums[0]) {
        case 0:
        {
            if (nums.size() < 2) {
                return false;
            }
            std::string backgroundFile{ MAP_BACKGROUND_FILE };
            auto it{ backgroundFile.find("%No%") };
            backgroundFile.replace(it, 4, std::to_string(nums[1]));

            SetBackground_(backgroundFile);
            break;
        }
        case 1:
            if (nums.size() < 5) {
                return false;
            }
            SetRoad_(static_cast<RoadType>(nums[1]), nums[2], nums[3], GetAngle(nums[4]));
            break;
        case 2:
            if (nums.size() < 5) {
                return false;
            }
            SetBuilding_(static_cast<BuildingType>(nums[1]), nums[2], nums[3], GetAngle(nums[4]));
            break;
        case 3:
            if (nums.size() < 5) {
                return false;
            }
            AddCar(static_cast<CarType>(nums[1]), nums[2], nums[3], GetAngle(nums[4]));
            break;
        }
    }

    fin.close();
    return true;
}

bool rtm::WorldController::LoadMap(MapNumber number)
{
    return LoadMap(WorldController::GetClassFile_(number));
}

void rtm::WorldController::AddCar(CarType type, int column, int row, float angle)
{
    dynamicObjects_.push_back(std::make_unique<CarObject>(type, column, row, angle));
    scene_->addChild(dynamicObjects_.back()->GetSprite(), VEHICLE_OBJECT_Z_ORDER);
}

void rtm::WorldController::Reset()
{
    RemoveDynamicObjects_();
}

void rtm::WorldController::SetBackground_(std::string const & filename)
{
    if (background_ != nullptr) {
        scene_->removeChild(background_);
    }
    background_ = cocos2d::Sprite::create(filename);
    background_->setAnchorPoint(cocos2d::Vec2{ 0, 0 });
    scene_->addChild(background_, BACKGROUND_Z_ORDER);
}

void rtm::WorldController::SetRoad_(RoadType type, int column, int row, float angle)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return;
    }
    size_t vecCol{ column + HIDDEN_AREA_SIZE };
    size_t vecRow{ row + HIDDEN_AREA_SIZE };

    if (coatingObjects_[vecCol][vecRow]) {
        if (coatingObjects_[vecCol][vecRow]->GetSprite() != nullptr) {
            scene_->removeChild(coatingObjects_[vecCol][vecRow]->GetSprite());
        }
        coatingObjects_[vecCol][vecRow].reset(new RoadObject{ type, column, row, angle });
    }
    else {
        coatingObjects_[vecCol][vecRow] = std::make_unique<RoadObject>(type, column, row, angle);
    }

    scene_->addChild(coatingObjects_[vecCol][vecRow]->GetSprite(), COATING_OBJECT_Z_ORDER);
}

void rtm::WorldController::SetBuilding_(BuildingType type, int column, int row, float angle)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return;
    }
    size_t vecCol{ column + HIDDEN_AREA_SIZE };
    size_t vecRow{ row + HIDDEN_AREA_SIZE };

    if (staticObjects_[vecCol][vecRow]) {
        if (staticObjects_[vecCol][vecRow]->GetSprite() != nullptr) {
            scene_->removeChild(staticObjects_[vecCol][vecRow]->GetSprite());
        }
        staticObjects_[vecCol][vecRow].reset(new BuildingObject{ type, column, row, angle });
    }
    else {
        staticObjects_[vecCol][vecRow] = std::make_unique<BuildingObject>(type, column, row, angle);
    }

    scene_->addChild(staticObjects_[vecCol][vecRow]->GetSprite(), MAP_OBJECT_Z_ORDER);
}

void rtm::WorldController::RemoveCoatingObjects_()
{
    for (auto& col : coatingObjects_) {
        for (auto& elem : col) {
            if (elem) {
                if (elem->GetSprite() != nullptr) {
                    scene_->removeChild(elem->GetSprite());
                }
                elem.reset(nullptr);
            }
        }
    }
}

void rtm::WorldController::RemoveStaticObjects_()
{
    for (auto& col : staticObjects_) {
        for (auto& elem : col) {
            if (elem) {
                if (elem->GetSprite() != nullptr) {
                    scene_->removeChild(elem->GetSprite());
                }
                elem.reset(nullptr);
            }
        }
    }
}

void rtm::WorldController::RemoveDynamicObjects_()
{
    for (auto& obj : dynamicObjects_) {
        scene_->removeChild(obj->GetSprite());
    }
    dynamicObjects_.clear();
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
