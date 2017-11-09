#include "WorldController.h"
#include "WorldScene.h"
#include "CoatingObject.h"
#include "DrivewayObject.h"
#include "CrossroadObject.h"
#include "TCrossroadObject.h"
#include "BuildingObject.h"
#include "CarObject.h"

rtm::WorldController::WorldController()
    : scene_{ nullptr }
    , columnsCount_{ 0 }
    , rowsCount_{ 0 }
    , deltaTime_{ 0.f }
    , background_{ nullptr }
    , coatingUnions_{}
    , staticObjects_{}
    , dynamicObjects_{}
{}

rtm::WorldController::WorldController(WorldScene* const scene)
    : scene_{ scene }
    , columnsCount_{ static_cast<size_t>(trunc(scene_->getContentSize().width / CELL_SIZE)) }
    , rowsCount_{ static_cast<size_t>(trunc(scene_->getContentSize().height / CELL_SIZE)) }
    , deltaTime_{ 0.f }
    , background_{ nullptr }
    , coatingUnions_{ columnsCount_ + 2 * HIDDEN_AREA_SIZE }
    , staticObjects_{ columnsCount_ + 2 * HIDDEN_AREA_SIZE }
    , dynamicObjects_{}
{
    // Init coating objects array
    for (auto& col : coatingUnions_) {
        col = std::vector<CoatingUnionShared>{ rowsCount_ + 2 * HIDDEN_AREA_SIZE };
    }

    // Init static objects array
    for (auto& col : staticObjects_) {
        col = std::vector<StaticObjectUnique>{ rowsCount_ + 2 * HIDDEN_AREA_SIZE };
    }
}

rtm::WorldController::WorldController(WorldScene* const scene, std::string const& filename)
    : WorldController{ scene }
{
    LoadMap(filename);
}

rtm::WorldController::WorldController(WorldScene* const scene, MapNumber number)
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
            // Not allowable position
            (!IsAllowableColumn(PixelToCell(obj.GetX())) || !IsAllowableRow(PixelToCell(obj.GetY()))) &&
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

rtm::CoatingObject* rtm::WorldController::GetCoatingObject(int column, int row)
{
    CoatingUnionShared& coatingUnion{ coatingUnions_[HIDDEN_AREA_SIZE + column][HIDDEN_AREA_SIZE + row] };
    if (coatingUnion) {
        return coatingUnion->GetCoatingObject(column, row).get();
    }
    else {
        return nullptr;
    }
}

rtm::StaticObject* rtm::WorldController::GetStaticObject(int column, int row)
{
    StaticObjectUnique& staticObject{ staticObjects_[HIDDEN_AREA_SIZE + column][HIDDEN_AREA_SIZE + row] };
    if (staticObject) {
        return staticObject.get();
    }
    else {
        return nullptr;
    }
}

std::vector<rtm::DynamicObjectUnique>& rtm::WorldController::GetDynamicObjects()
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

bool rtm::WorldController::IsAllowableColumn(int column)
{
    return column >= -1 && column <= static_cast<int>(columnsCount_);
}

bool rtm::WorldController::IsAllowableRow(int row)
{
    return row >= -1 && row <= static_cast<int>(rowsCount_);
}

bool rtm::WorldController::IsVisibleColumn(int column)
{
    return column >= 0 && column < static_cast<int>(columnsCount_);
}

bool rtm::WorldController::IsVisibleRow(int row)
{
    return row >= 0 && row < static_cast<int>(rowsCount_);
}

bool rtm::WorldController::LoadMap(std::string const& filename)
{
    std::ifstream fin{ filename, std::ios::in | std::ios::binary };
    if (!fin.is_open()) {
        return false;
    }

    // Check file format
    {
        char type[4];
        fin.read(type, 4 * sizeof(char));
        if (type[0] != 'R' || type[1] != 'T' || type[2] != 'M' || type[3] != 'M') {
            return false;
        }
    }

    // Objects count
    uint16_t objectsCount;
    if (!fin.read((char*) &objectsCount, sizeof(uint16_t))) {
        return false;
    }

    for (uint16_t i = 0; i < objectsCount && !fin.eof(); ++i) {
        // Object params count
        char paramsCount;
        fin.read(&paramsCount, sizeof(char));

        // Object params
        char* params = new char[paramsCount];
        fin.read(params, paramsCount * sizeof(char));

        // Generating objects
        GenerateObject_(params, paramsCount);

        delete[] params;
    }

    fin.close();
    return true;
}

bool rtm::WorldController::LoadMap(MapNumber number)
{
    return LoadMap(WorldController::GetClassFile_(number));
}

void rtm::WorldController::Reset()
{
    RemoveDynamicObjects_();
}

void rtm::WorldController::AddTestObjects()
{
    AddCrossroad_(1, 1, 1, 1, 0, 0);
    AddCrossroad_(3, 1, 2, 1, 0, 0);
    AddCrossroad_(6, 1, 1, 2, 0, 0);
    AddCrossroad_(8, 1, 2, 2, 0, 0);
    AddCrossroad_(11, 1, 3, 3, 0, 0);
    AddCrossroad_(1, 5, 0, 0, 1, 1);
    AddCrossroad_(3, 5, 0, 0, 2, 1);
    AddCrossroad_(6, 5, 0, 0, 1, 2);
    AddCrossroad_(8, 5, 0, 0, 2, 2);
    AddCrossroad_(11, 5, 0, 0, 3, 3);

    AddDriveway_(15, 1, 3, 1, TopDirection);
    AddDriveway_(15, 3, 3, 1, RightDirection);
    AddDriveway_(19, 1, 1, 3, TopDirection);
    AddDriveway_(21, 1, 1, 3, RightDirection);
    AddDriveway_(15, 5, 3, 1, BottomDirection);
    AddDriveway_(15, 7, 3, 1, LeftDirection);
    AddDriveway_(19, 5, 1, 3, BottomDirection);
    AddDriveway_(21, 5, 1, 3, LeftDirection);

    AddTCrossroad_(23, 1, 1, 1, 0, 0, TopDirection);
    AddTCrossroad_(25, 1, 2, 1, 0, 0, TopDirection);
    AddTCrossroad_(28, 1, 1, 2, 0, 0, TopDirection);
    AddTCrossroad_(30, 1, 2, 2, 0, 0, TopDirection);
    AddTCrossroad_(33, 1, 3, 3, 0, 0, TopDirection);
    AddTCrossroad_(23, 5, 0, 0, 1, 1, RightDirection);
    AddTCrossroad_(25, 5, 0, 0, 2, 1, RightDirection);
    AddTCrossroad_(28, 5, 0, 0, 1, 2, RightDirection);
    AddTCrossroad_(30, 5, 0, 0, 2, 2, RightDirection);
    AddTCrossroad_(33, 5, 0, 0, 3, 3, RightDirection);
}

void rtm::WorldController::AddCar(CarType type, int column, int row, float angle)
{
    AddCar_(type, column, row, angle);
}

void rtm::WorldController::RemoveCoatingObjects()
{
    RemoveCoatingObjects_();
}

void rtm::WorldController::RemoveStaticObjects()
{
    RemoveStaticObjects_();
}

void rtm::WorldController::RemoveDynamicObjects()
{
    RemoveDynamicObjects_();
}

bool rtm::WorldController::IsEmpty(int column, int row, size_t width, size_t height)
{
    size_t vecCol{ HIDDEN_AREA_SIZE + column };
    size_t vecRow{ HIDDEN_AREA_SIZE + row };

    for (size_t i = vecCol; i < vecCol + width; ++i) {
        for (size_t j = vecRow; j < vecRow + height; ++j) {
            if (coatingUnions_[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void rtm::WorldController::GenerateObject_(char* params, char count)
{
    if (count < 2) return;

    switch (params[0]) {
    case 1:
        SetBackground_(static_cast<BackgroundNumber>(params[1]));
        break;
    case 2:
        switch (params[1]) {
        case 1:
            if (count < 7) return;
            AddDriveway_(params[2], params[3], params[4], params[5], static_cast<DirectionType>(params[6]));
            break;
        case 2:
            if (count < 8) return;
            AddCrossroad_(params[2], params[3], params[4], params[5], params[6], params[7]);
            break;
        case 3:
            if (count < 9) return;
            AddTCrossroad_(params[2], params[3], params[4], params[5], params[6], params[7], static_cast<DirectionType>(params[8]));
            break;
        }
        break;
    case 3:
        switch (params[1]) {
        case 1:
            if (count < 6) return;
            AddBuilding_(static_cast<BuildingType>(params[2]), params[3], params[4], DirectionToAngle(static_cast<DirectionType>(params[5])));
            break;
        }
        break;
    case 4:
        switch (params[1]) {
        case 1:
            if (count < 6) return;
            AddCar_(static_cast<CarType>(params[2]), params[3], params[4], DirectionToAngle(static_cast<DirectionType>(params[5])));
            break;
        }
        break;
    }
}

void rtm::WorldController::SetBackground_(std::string const& filename)
{
    if (background_ != nullptr) {
        scene_->removeChild(background_);
    }
    background_ = cocos2d::Sprite::create(filename);
    background_->setAnchorPoint(cocos2d::Vec2{ 0, 0 });
    scene_->addChild(background_, BACKGROUND_Z_ORDER);
}

void rtm::WorldController::SetBackground_(BackgroundNumber number)
{
    SetBackground_(GetBackgroundFile_(number));
}

void rtm::WorldController::AddDriveway_(int column, int row, size_t width, size_t height, DirectionType direction)
{
    if (IsEmpty(column, row, width, height)) {
        CoatingUnionShared coatingUnion{ new DrivewayObject{column, row, width, height, direction } };
        coatingUnion->ShowCoatingObjects(scene_);

        size_t vecCol{ HIDDEN_AREA_SIZE + column };
        size_t vecRow{ HIDDEN_AREA_SIZE + row };

        for (size_t i = vecCol; i < vecCol + width; ++i) {
            for (size_t j = vecRow; j < vecRow + height; ++j) {
                coatingUnions_[i][j] = coatingUnion;
            }
        }
    }
}

void rtm::WorldController::AddCrossroad_(int column, int row, size_t upLines, 
    size_t toRightLines, size_t downLines, size_t toLeftLines)
{
    size_t width{ upLines + downLines };
    size_t height{ toRightLines + toLeftLines };

    if (IsEmpty(column, row, width, height)) {
        CoatingUnionShared coatingUnion{ new CrossroadObject{ column, row, upLines, toRightLines, downLines, toLeftLines } };
        coatingUnion->ShowCoatingObjects(scene_);

        size_t vecCol{ HIDDEN_AREA_SIZE + column };
        size_t vecRow{ HIDDEN_AREA_SIZE + row };

        for (size_t i = vecCol; i < vecCol + width; ++i) {
            for (size_t j = vecRow; j < vecRow + height; ++j) {
                coatingUnions_[i][j] = coatingUnion;
            }
        }
    }
}

void rtm::WorldController::AddTCrossroad_(int column, int row, size_t upLines, size_t toRightLines, 
    size_t downLines, size_t toLeftLines, DirectionType nullDirection)
{
    size_t width{ upLines + downLines };
    size_t height{ toRightLines + toLeftLines };

    if (IsEmpty(column, row, width, height)) {
        CoatingUnionShared coatingUnion{ new TCrossroadObject{ column, row, upLines, toRightLines, downLines, toLeftLines, nullDirection } };
        coatingUnion->ShowCoatingObjects(scene_);

        size_t vecCol{ HIDDEN_AREA_SIZE + column };
        size_t vecRow{ HIDDEN_AREA_SIZE + row };

        for (size_t i = vecCol; i < vecCol + width; ++i) {
            for (size_t j = vecRow; j < vecRow + height; ++j) {
                coatingUnions_[i][j] = coatingUnion;
            }
        }
    }
}

void rtm::WorldController::AddBuilding_(BuildingType type, int column, int row, float angle)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return;
    }
    size_t vecCol{ HIDDEN_AREA_SIZE + column };
    size_t vecRow{ HIDDEN_AREA_SIZE + row };

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

void rtm::WorldController::AddCar_(CarType type, int column, int row, float angle)
{
    dynamicObjects_.push_back(std::make_unique<CarObject>(type, column, row, angle));
    scene_->addChild(dynamicObjects_.back()->GetSprite(), VEHICLE_OBJECT_Z_ORDER);
}

void rtm::WorldController::RemoveCoatingObjects_()
{
    for (auto& col : coatingUnions_) {
        for (auto& elem : col) {
            if (elem) {
                elem->ReleaseCoatingObjects(scene_);
                elem.reset();
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
                elem.reset();
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
    std::string filename{ MAP_FILENAME_MASK };
    auto it{ filename.find("%No%") };
    filename.replace(it, 4, std::to_string(static_cast<int>(number)));

    return filename;
}

std::string rtm::WorldController::GetBackgroundFile_(BackgroundNumber number)
{
    std::string filename{ BACKGROUND_FILENAME_MASK };
    auto it{ filename.find("%No%") };
    filename.replace(it, 4, std::to_string(number));

    return filename;
}
