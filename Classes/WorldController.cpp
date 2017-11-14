#include "WorldController.h"
#include "WorldScene.h"
#include "CoatingObject.h"
#include "DrivewayObject.h"
#include "CrossroadObject.h"
#include "TurnObject.h"
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
    , columnsCount_{ static_cast<size_t>(trunc(scene_->getContentSize().width / CELL_SIZE)) + 2 * HIDDEN_AREA_SIZE }
    , rowsCount_{ static_cast<size_t>(trunc(scene_->getContentSize().height / CELL_SIZE)) + 2 * HIDDEN_AREA_SIZE }
    , deltaTime_{ 0.f }
    , background_{ nullptr }
    , coatingUnions_{ columnsCount_ }
    , staticObjects_{ columnsCount_ }
    , dynamicObjects_{}
{
    // Init coating objects array
    for (auto& col : coatingUnions_) {
        col = std::vector<CoatingUnionShared>{ rowsCount_ };
    }

    // Init static objects array
    for (auto& col : staticObjects_) {
        col = std::vector<StaticShared>{ rowsCount_ };
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

rtm::WorldScene* rtm::WorldController::GetScene() const
{
    return scene_;
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
    CoatingUnionShared& coatingUnion{ coatingUnions_[GetVectorColumn_(column)][GetVectorRow_(row)] };
    if (coatingUnion) {
        return coatingUnion->GetCoatingObject(column, row).get();
    }
    else {
        return nullptr;
    }
}

rtm::StaticObject* rtm::WorldController::GetStaticObject(int column, int row)
{
    StaticShared& staticObject{ staticObjects_[GetVectorColumn_(column)][GetVectorRow_(row)] };
    if (staticObject) {
        return staticObject.get();
    }
    else {
        return nullptr;
    }
}

std::vector<rtm::DynamicShared>& rtm::WorldController::GetDynamicObjects()
{
    return dynamicObjects_;
}

bool rtm::WorldController::IsCorrectColumn(int column)
{
    return 0 <= GetVectorColumn_(column) && GetVectorColumn_(column) < columnsCount_;
}

bool rtm::WorldController::IsCorrectRow(int row)
{
    return 0 <= GetVectorRow_(row) && GetVectorRow_(row) < columnsCount_;
}

bool rtm::WorldController::IsAllowableColumn(int column)
{
    //size_t allowedHiddenArea{ 1 };
    //return HIDDEN_AREA_SIZE - allowedHiddenArea <= GetVectorColumn_(column) &&
    //    GetVectorColumn_(column) < columnsCount_ - HIDDEN_AREA_SIZE + allowedHiddenArea;
    
    // Optimized
    return -1 <= column && column <= static_cast<int>(columnsCount_ - HIDDEN_AREA_SIZE - HIDDEN_AREA_SIZE);
}

bool rtm::WorldController::IsAllowableRow(int row)
{
    //size_t allowedHiddenArea{ 1 };
    //return HIDDEN_AREA_SIZE - allowedHiddenArea <= GetVectorRow_(row) &&
    //    GetVectorRow_(row) < rowsCount_ - HIDDEN_AREA_SIZE + allowedHiddenArea;
    
    // Optimized
    return -1 <= row && row <= static_cast<int>(rowsCount_ - HIDDEN_AREA_SIZE - HIDDEN_AREA_SIZE);
}

bool rtm::WorldController::IsVisibleColumn(int column)
{
    return column >= 0 && column < GetRealColumn_(columnsCount_);
}

bool rtm::WorldController::IsVisibleRow(int row)
{
    return row >= 0 && row < GetRealColumn_(rowsCount_);
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
    if (!fin.read((char*)&objectsCount, sizeof(uint16_t))) {
        return false;
    }

    RemoveCoatingObjects_();
    RemoveStaticObjects_();
    RemoveDynamicObjects_();

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
    AddDriveway_(1, 1, 3, 1, Up);
    AddDriveway_(1, 3, 3, 1, Right);
    AddDriveway_(5, 1, 1, 3, Up);
    AddDriveway_(7, 1, 1, 3, Right);
    AddDriveway_(1, 7, 3, 1, Down);
    AddDriveway_(1, 9, 3, 1, Left);
    AddDriveway_(5, 7, 1, 3, Down);
    AddDriveway_(7, 7, 1, 3, Left);

    AddCrossroad_(9, 1, { 1, 1, 0, 0 });
    AddCrossroad_(13, 1, { 2, 1, 0, 0 });
    AddCrossroad_(18, 1, { 1, 2, 0, 0 });
    AddCrossroad_(22, 1, { 2, 2, 0, 0 });
    AddCrossroad_(27, 1, { 3, 3, 0, 0 });
    AddCrossroad_(9, 7, { 0, 0, 1, 1 });
    AddCrossroad_(13, 7, { 0, 0, 2, 1 });
    AddCrossroad_(18, 7, { 0, 0, 1, 2 });
    AddCrossroad_(22, 7, { 0, 0, 2, 2 });
    AddCrossroad_(27, 7, { 0, 0, 3, 3 });

    AddTCrossroad_(33, 1, { 1, 1, 0, 0 }, Up);
    AddTCrossroad_(37, 1, { 2, 1, 0, 0 }, Up);
    AddTCrossroad_(42, 1, { 1, 2, 0, 0 }, Up);
    AddTCrossroad_(46, 1, { 2, 2, 0, 0 }, Up);
    AddTCrossroad_(51, 1, { 3, 3, 0, 0 }, Up);
    AddTCrossroad_(33, 7, { 0, 0, 1, 1 }, Right);
    AddTCrossroad_(37, 7, { 0, 0, 2, 1 }, Right);
    AddTCrossroad_(42, 7, { 0, 0, 1, 2 }, Right);
    AddTCrossroad_(46, 7, { 0, 0, 2, 2 }, Right);
    AddTCrossroad_(51, 7, { 0, 0, 3, 3 }, Right);

    
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
    for (size_t i = GetVectorColumn_(column); i < GetVectorColumn_(column) + width; ++i) {
        for (size_t j = GetVectorRow_(row); j < GetVectorRow_(row) + height; ++j) {
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
            if (count == 7) {
                AddDriveway_(params[2], params[3], params[4], params[5], static_cast<AngleType>(params[6]));
            }
            break;
        case 2:
            if (count == 7) {
                AddTurt_(params[2], params[3], params[4], params[5], static_cast<AngleType>(params[6]));
            }
            break;
        case 3:
            if (count == 8) {
                LinesCounts linesCount{
                    static_cast<size_t>(params[4]),
                    static_cast<size_t>(params[5]),
                    static_cast<size_t>(params[6]),
                    static_cast<size_t>(params[7])
                };
                AddCrossroad_(params[2], params[3], linesCount);
            }
            else if (count == 9) {
                LinesCounts linesCount{
                    static_cast<size_t>(params[4]),
                    static_cast<size_t>(params[5]),
                    static_cast<size_t>(params[6]),
                    static_cast<size_t>(params[7])
                };
                AddCrossroad_(params[2], params[3], linesCount, static_cast<ControlUnitType>(params[8]));
            }
            break;
        case 4:
            if (count == 9) {
                LinesCounts linesCount{
                    static_cast<size_t>(params[4]),
                    static_cast<size_t>(params[5]),
                    static_cast<size_t>(params[6]),
                    static_cast<size_t>(params[7])
                };
                AddTCrossroad_(params[2], params[3], linesCount, static_cast<AngleType>(params[8]));
            }
            else if (count == 10) {
                LinesCounts linesCount{
                    static_cast<size_t>(params[4]),
                    static_cast<size_t>(params[5]),
                    static_cast<size_t>(params[6]),
                    static_cast<size_t>(params[7])
                };
                AddTCrossroad_(params[2], params[3], linesCount, static_cast<AngleType>(params[8]),
                    static_cast<ControlUnitType>(params[9]));
            }
            break;
        }
        break;
    case 3:
        switch (params[1]) {
        case 1:
            if (count == 6) {
                AddBuilding_(static_cast<BuildingType>(params[2]), params[3], params[4], AngleTypeToAngle(static_cast<AngleType>(params[5])));
            }
            break;
        }
        break;
    case 4:
        switch (params[1]) {
        case 1:
            if (count == 6) {
                AddCar_(static_cast<CarType>(params[2]), params[3], params[4], AngleTypeToAngle(static_cast<AngleType>(params[5])));
            }
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

void rtm::WorldController::AddCoatingUnion_(int column, int row, CoatingUnionShared coatingUnion)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return;
    }

    size_t width{ coatingUnion->GetWidth() };
    size_t height{ coatingUnion->GetHeight() };

    if (IsEmpty(column, row, width, height)) {
        coatingUnion->ShowCoatingObjects(scene_);

        size_t vecCol{ GetVectorColumn_(column) };
        size_t vecRow{ GetVectorRow_(row) };
        for (size_t i = vecCol; i < vecCol + width; ++i) {
            for (size_t j = vecRow; j < vecRow + height; ++j) {
                coatingUnions_[i][j] = coatingUnion;
            }
        }
    }
}

void rtm::WorldController::AddDriveway_(int column, int row, size_t width, size_t height, AngleType angle)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return;
    }

    AddCoatingUnion_(column, row, std::make_shared<DrivewayObject>(column, row, width, height, angle));
}

void rtm::WorldController::AddCrossroad_(int column, int row, LinesCounts linesCounts, ControlUnitType controlUnitType)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return;
    }

    AddCoatingUnion_(column, row, std::make_shared<CrossroadObject>(column, row, linesCounts, controlUnitType));
}

void rtm::WorldController::AddTCrossroad_(int column, int row, LinesCounts linesCounts, AngleType nullDirection, 
    ControlUnitType controlUnitType)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return;
    }

    AddCoatingUnion_(column, row, std::make_shared<CrossroadObject>(column, row, linesCounts, nullDirection, controlUnitType));
}

void rtm::WorldController::AddTurt_(int column, int row, size_t linesCount, bool isRight, AngleType angle)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return;
    }

    AddCoatingUnion_(column, row, std::make_shared<TurnObject>(column, row, linesCount, isRight, angle));
}

void rtm::WorldController::AddStaticObject_(int column, int row, StaticShared staticObject)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return;
    }

    scene_->addChild(staticObject->GetSprite(), MAP_OBJECT_Z_ORDER);

    size_t vecCol{ GetVectorColumn_(column) };
    size_t vecRow{ GetVectorRow_(row) };
    if (staticObjects_[vecCol][vecRow]) {
        if (staticObjects_[vecCol][vecRow]->GetSprite() != nullptr) {
            scene_->removeChild(staticObjects_[vecCol][vecRow]->GetSprite());
        }
    }
    staticObjects_[vecCol][vecRow] = staticObject;
}

void rtm::WorldController::AddBuilding_(BuildingType type, int column, int row, float angle)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return;
    }

    AddStaticObject_(column, row, std::make_shared<BuildingObject>(type, column, row, angle));
}

void rtm::WorldController::AddDynamicObject_(int column, int row, DynamicShared dynamicObject)
{
    if (!IsAllowableColumn(column) || !IsAllowableRow(row)) {
        return;
    }

    scene_->addChild(dynamicObject->GetSprite(), VEHICLE_OBJECT_Z_ORDER);
    dynamicObjects_.push_back(dynamicObject);
}

void rtm::WorldController::AddCar_(CarType type, int column, int row, float angle)
{
    if (!IsAllowableColumn(column) || !IsAllowableRow(row)) {
        return;
    }

    AddDynamicObject_(column, row, std::make_shared<CarObject>(type, column, row, angle));
}

size_t rtm::WorldController::GetVectorColumn_(int column)
{
    return static_cast<size_t>(HIDDEN_AREA_SIZE + column);
}

size_t rtm::WorldController::GetVectorRow_(int row)
{
    return static_cast<size_t>(HIDDEN_AREA_SIZE + row);
}

int rtm::WorldController::GetRealColumn_(size_t column)
{
    return static_cast<int>(column - HIDDEN_AREA_SIZE);
}

int rtm::WorldController::GetRealRow_(size_t row)
{
    return static_cast<int>(row - HIDDEN_AREA_SIZE);
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
