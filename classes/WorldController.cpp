#include "AllRtmClasses.h"

///////////////////////
//  LOCAL FUNCTIONS  //
///////////////////////

class EofException
    : public std::exception
{};

template<typename T>
void ReadBytes(std::ifstream& fin, T* out, size_t count = 1)
{
    fin.read((char*)(out), count);
    if (fin.eof()) {
        throw EofException();
    }
}

uint16_t BytesUnion(uint8_t lo, uint8_t hi)
{
    union {
        uint16_t value;
        struct { uint8_t lo; uint8_t hi; };
    } result;
    result.lo = lo;
    result.hi = hi;
    return result.value;
}

uint32_t BytesUnion(uint8_t lo, uint8_t lo_mid, uint8_t hi_mid, uint8_t hi)
{
    union {
        uint16_t value;
        struct { uint8_t lo; uint8_t lo_mid; uint8_t hi_mid; uint8_t hi; };
    } result;
    result.lo = lo;
    result.lo_mid = lo_mid;
    result.hi_mid = hi_mid;
    result.hi = hi;
    return result.value;
}

///////////////////////
//  CLASS FUNCTIONS  //
///////////////////////

rtm::WorldController::WorldController()
    : scene_{ nullptr }
    , mainLayer_{ nullptr }
    , isPause_{ false }
    , hiddenArea_{ 0 }
    , columnsCount_{ 0 }
    , rowsCount_{ 0 }
    , spawns_{}
    , deltaTime_{ 0.f }
    , spawnTime_{ 0.f }
    , cleanTime_{ 0.f }
    , timeFactor_{ 0 }
    , lastMapFile_{}
    , coatingUnions_{}
    , controlUnits_{}
    , staticObjects_{}
    , dynamicObjects_{}
{}

rtm::WorldController::WorldController(WorldScene* const scene)
    : scene_{ scene }
    , mainLayer_{ scene->GetMainLayer() }
    , isPause_{ true }
    , hiddenArea_{ 0 }
    , columnsCount_{ 0 }
    , rowsCount_{ 0 }
    , spawns_{}
    , deltaTime_{ 0.f }
    , spawnTime_{ 0.f }
    , cleanTime_{ 0.f }
    , timeFactor_{ 1 }
    , lastMapFile_{}
    , coatingUnions_{}
    , controlUnits_{}
    , staticObjects_{}
    , dynamicObjects_{}
{}

rtm::WorldController::WorldController(WorldScene* const scene, std::string const& filename)
    : WorldController{ scene }
{
    LoadMap(filename);
}

rtm::WorldController::WorldController(WorldScene* const scene, size_t mapNumber)
    : WorldController{ scene }
{
    LoadMap(mapNumber);
}

void rtm::WorldController::Update(float time)
{
    if (isPause_) {
        return;
    }

    deltaTime_ = time * timeFactor_;
    spawnTime_ += deltaTime_;
    cleanTime_ += deltaTime_;

    for (auto& controlUnit : controlUnits_) {
        controlUnit->Update(this);
    }

    if (spawnTime_ > 10.f) {
        SpawnCar();
        spawnTime_ = 0.f;
    }
    if (cleanTime_ > 10.f) {
        RemoveAccidents();
        cleanTime_ = 0.f;
    }

    for (auto& it{ dynamicObjects_.begin() }; it != dynamicObjects_.end();) {
        DynamicObject& obj{ **it };
        obj.Update(this);

        // Not allowable position
        if (!IsAllowableColumn(PixelToCell(obj.GetX_())) || !IsAllowableRow(PixelToCell(obj.GetY_()))) {
            mainLayer_->removeChild(obj.GetSprite());
            it = dynamicObjects_.erase(it);
        }
        else {
            ++it;
        }
    }
    CheckCollisions(this);
}

cocos2d::Layer* rtm::WorldController::GetLayer() const
{
    return mainLayer_;
}

size_t rtm::WorldController::GetColumnsCount() const
{
    return columnsCount_ - 2 * hiddenArea_;
}

size_t rtm::WorldController::GetRowsCount() const
{
    return rowsCount_ - 2 * hiddenArea_;
}

float rtm::WorldController::GetDeltaTime() const
{
    return deltaTime_;
}

float rtm::WorldController::GetTimeFactor() const
{
    return timeFactor_;
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

rtm::CoatingUnion* rtm::WorldController::GetCoatingUnion(int column, int row)
{
    return coatingUnions_[GetVectorColumn_(column)][GetVectorRow_(row)].get();
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

bool rtm::WorldController::IsPause()
{
    return isPause_;
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
    return -1 <= column && column <= static_cast<int>(columnsCount_ - 2 * hiddenArea_);
}

bool rtm::WorldController::IsAllowableRow(int row)
{
    //size_t allowedHiddenArea{ 1 };
    //return HIDDEN_AREA_SIZE - allowedHiddenArea <= GetVectorRow_(row) &&
    //    GetVectorRow_(row) < rowsCount_ - HIDDEN_AREA_SIZE + allowedHiddenArea;

    // Optimized
    return -1 <= row && row <= static_cast<int>(rowsCount_ - 2 * hiddenArea_);
}

bool rtm::WorldController::IsVisibleColumn(int column)
{
    return column >= 0 && column < GetRealColumn_(columnsCount_);
}

bool rtm::WorldController::IsVisibleRow(int row)
{
    return row >= 0 && row < GetRealColumn_(rowsCount_);
}

void rtm::WorldController::SetTimeFactor(float factor)
{
    timeFactor_ = factor;
}

bool rtm::WorldController::LoadMap(std::string const& filename)
{
    std::ifstream fin{ filename, std::ios::in | std::ios::binary };
    if (!fin.is_open()) {
        return false;
    }

    bool result{ true };
    try {
        // Check file format and remember last file
        char type[4];
        ReadBytes(fin, type, 4);
        if (type[0] != 'R' || type[1] != 'T' || type[2] != 'M' || type[3] != 'M') return false;
        lastMapFile_ = filename;

        // Clear old map
        CloseMap_();

        // Background
        uint8_t backgroundId{ 0 };
        ReadBytes(fin, &backgroundId);
        scene_->SetBackground(backgroundId);

        // Sizes
        ReadBytes(fin, &columnsCount_, 2);
        ReadBytes(fin, &rowsCount_, 2);
        ReadBytes(fin, &hiddenArea_);

        columnsCount_ += 2 * hiddenArea_;
        rowsCount_ += 2 * hiddenArea_;

        // Init coating objects array
        coatingUnions_ = CoatingUnionMatrix{ columnsCount_ };
        for (auto& col : coatingUnions_) {
            col = CoatingUnionVector{ rowsCount_ };
        }

        // Init static objects array
        staticObjects_ = StaticMatrix{ columnsCount_ };
        for (auto& col : staticObjects_) {
            col = StaticVector{ rowsCount_ };
        }

        // Spawns
        uint16_t spawnsCount{ 0 };
        ReadBytes(fin, &spawnsCount, 2);
        for (uint16_t i = 0; i < spawnsCount; ++i) {
            uint8_t spawnParams[5];
            ReadBytes(fin, spawnParams, 5);

            SpawnType newSpawn;
            newSpawn.column = BytesUnion(spawnParams[0], spawnParams[1]);
            newSpawn.row = BytesUnion(spawnParams[2], spawnParams[3]);
            newSpawn.angle = AngleTypeToAngle(static_cast<AngleType>(spawnParams[4]));

            spawns_.push_back(newSpawn);
        }

        // Objects
        uint32_t objectsCount{ 0 };
        ReadBytes(fin, &objectsCount, 4);
        for (uint32_t i = 0; i < objectsCount; ++i) {
            // Object params count
            uint8_t paramsCount{ 0 };
            ReadBytes(fin, &paramsCount);

            // Object params
            std::unique_ptr<uint8_t> params{ new uint8_t[paramsCount] };
            ReadBytes(fin, params.get(), paramsCount);

            // Generating objects
            if (!GenerateObject_(params.get(), paramsCount)) {
                result = false;
            }
        }

        // Continue playing
        Play();
    }
    catch (EofException&) {
        result = false;
    }

    fin.close();
    return result;
}

bool rtm::WorldController::LoadMap(size_t number)
{
    return LoadMap(GetFilename(MAP_FILENAME_MASK, number));
}

void rtm::WorldController::SpawnCar()
{
    for (auto& spawn : spawns_) {
        AddCar_(rand() % 5 + 1, spawn.column, spawn.row, spawn.angle);
    }
}

void rtm::WorldController::RemoveAccidents()
{
    for (auto& it{ dynamicObjects_.begin() }; it != dynamicObjects_.end();) {
        DynamicObject& obj{ **it };

        if (obj.HasCollision()) {
            mainLayer_->removeChild(obj.GetSprite());
            it = dynamicObjects_.erase(it);
        }
        else {
            ++it;
        }
    }
}

void rtm::WorldController::RemoveVehicles()
{
    ClearDynamicObjects_();
}

void rtm::WorldController::Play()
{
    isPause_ = false;
}

void rtm::WorldController::Pause()
{
    isPause_ = true;
}

void rtm::WorldController::Reset()
{
    LoadMap(lastMapFile_);
}

bool rtm::WorldController::IsEmpty_(int column, int row, size_t width, size_t height)
{
    for (size_t i = GetVectorColumn_(column); i < GetVectorColumn_(column) + width; ++i) {
        for (size_t j = GetVectorRow_(row); j < GetVectorRow_(row) + height; ++j) {
            if (coatingUnions_[i][j] || staticObjects_[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool rtm::WorldController::GenerateObject_(uint8_t* params, uint8_t count)
{
    if (count < 2) return false;

    switch (params[0]) {
    case 1:
        switch (params[1]) {
        case 1:
            if (count == 12) {
                return AddDriveway_(static_cast<CoatingType>(params[2]), BytesUnion(params[3], params[4]), BytesUnion(params[5], params[6])
                    , BytesUnion(params[7], params[8]), BytesUnion(params[9], params[10]), static_cast<AngleType>(params[11]));
            }
            break;
        case 2:
            if (count == 10) {
                return AddLeftTurt_(static_cast<CoatingType>(params[2]), BytesUnion(params[3], params[4]), BytesUnion(params[5], params[6])
                    , BytesUnion(params[7], params[8]), static_cast<AngleType>(params[9]));
            }
            break;
        case 3:
            if (count == 10) {
                return AddRightTurt_(static_cast<CoatingType>(params[2]), BytesUnion(params[3], params[4]), BytesUnion(params[5], params[6])
                    , BytesUnion(params[7], params[8]), static_cast<AngleType>(params[9]));
            }
            break;
        case 4:
            if (count == 15) {
                LinesCounts linesCount{
                    static_cast<size_t>(BytesUnion(params[7], params[8])),
                    static_cast<size_t>(BytesUnion(params[9], params[10])),
                    static_cast<size_t>(BytesUnion(params[11], params[12])),
                    static_cast<size_t>(BytesUnion(params[13], params[14]))
                };
                return AddCrossroad_(static_cast<CoatingType>(params[2]), BytesUnion(params[3], params[4]), BytesUnion(params[5], params[6]), linesCount);
            }
            else if (count == 16) {
                LinesCounts linesCount{
                    static_cast<size_t>(BytesUnion(params[7], params[8])),
                    static_cast<size_t>(BytesUnion(params[9], params[10])),
                    static_cast<size_t>(BytesUnion(params[11], params[12])),
                    static_cast<size_t>(BytesUnion(params[13], params[14]))
                };
                return AddCrossroad_(static_cast<CoatingType>(params[2]), BytesUnion(params[3], params[4]), BytesUnion(params[5], params[6])
                    , linesCount, params[15]);
            }
            break;
        case 5:
            if (count == 16) {
                LinesCounts linesCount{
                    static_cast<size_t>(BytesUnion(params[7], params[8])),
                    static_cast<size_t>(BytesUnion(params[9], params[10])),
                    static_cast<size_t>(BytesUnion(params[11], params[12])),
                    static_cast<size_t>(BytesUnion(params[13], params[14]))
                };
                return AddTCrossroad_(static_cast<CoatingType>(params[2]), BytesUnion(params[3], params[4]), BytesUnion(params[5], params[6])
                    , linesCount, static_cast<AngleType>(params[15]));
            }
            else if (count == 17) {
                LinesCounts linesCount{
                    static_cast<size_t>(BytesUnion(params[7], params[8])),
                    static_cast<size_t>(BytesUnion(params[9], params[10])),
                    static_cast<size_t>(BytesUnion(params[11], params[12])),
                    static_cast<size_t>(BytesUnion(params[13], params[14]))
                };
                return AddTCrossroad_(static_cast<CoatingType>(params[2]), BytesUnion(params[3], params[4]), BytesUnion(params[5], params[6])
                    , linesCount, static_cast<AngleType>(params[15]), static_cast<size_t>(params[16]));
            }
            break;
        }
        break;
    case 2:
        switch (params[1]) {
        case 1:
            if (count == 8) {
                return AddBuilding_(params[2], BytesUnion(params[3], params[4])
                    , BytesUnion(params[5], params[6]), AngleTypeToAngle(static_cast<AngleType>(params[7])));
            }
            break;
        case 2:
            if (count == 8) {
                return AddBush_(params[2], BytesUnion(params[3], params[4])
                    , BytesUnion(params[5], params[6]), AngleTypeToAngle(static_cast<AngleType>(params[7])));
            }
            break;
        }
        break;
    case 3:
        switch (params[1]) {
        case 1:
            if (count == 8) {
                return AddCar_(params[2], BytesUnion(params[3], params[4])
                    , BytesUnion(params[5], params[6]), AngleTypeToAngle(static_cast<AngleType>(params[7])));
            }
            break;
        }
        break;
    }

    return false;
}

bool rtm::WorldController::AddCoatingUnion_(int column, int row, CoatingUnionShared coatingUnion)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return false;
    }

    size_t width{ coatingUnion->GetWidth() };
    size_t height{ coatingUnion->GetHeight() };

    if (IsEmpty_(column, row, width, height)) {
        coatingUnion->ShowSprites(mainLayer_);

        size_t vecCol{ GetVectorColumn_(column) };
        size_t vecRow{ GetVectorRow_(row) };
        for (size_t i = vecCol; i < vecCol + width; ++i) {
            for (size_t j = vecRow; j < vecRow + height; ++j) {
                coatingUnions_[i][j] = coatingUnion;
            }
        }
        return true;
    }
    else {
        return false;
    }
}

bool rtm::WorldController::AddDriveway_(CoatingType type, int column, int row, size_t width, size_t height, AngleType angle)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return false;
    }

    return AddCoatingUnion_(column, row, std::make_shared<DrivewayObject>(type, column, row, width, height, angle));
}

bool rtm::WorldController::AddCrossroad_(CoatingType type, int column, int row, LinesCounts linesCounts, size_t controlUnitType)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return false;
    }

    CrossroadObject* coatingUnion{ new CrossroadObject{ type, column, row, linesCounts, controlUnitType } };
    AddControlUnit_(coatingUnion->GetControlUnit());

    return AddCoatingUnion_(column, row, CoatingUnionShared{ coatingUnion });
}

bool rtm::WorldController::AddTCrossroad_(CoatingType type, int column, int row, LinesCounts linesCounts, AngleType nullDirection,
    size_t controlUnitType)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return false;
    }

    CrossroadObject* coatingUnion{ new CrossroadObject{ type, column, row, linesCounts, nullDirection, controlUnitType } };
    AddControlUnit_(coatingUnion->GetControlUnit());

    return AddCoatingUnion_(column, row, CoatingUnionShared{ coatingUnion });
}

bool rtm::WorldController::AddLeftTurt_(CoatingType type, int column, int row, size_t linesCount, AngleType angle)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return false;
    }

    return AddCoatingUnion_(column, row, std::make_shared<TurnObject>(false, type, column, row, linesCount, angle));
}

bool rtm::WorldController::AddRightTurt_(CoatingType type, int column, int row, size_t linesCount, AngleType angle)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return false;
    }

    return AddCoatingUnion_(column, row, std::make_shared<TurnObject>(true, type, column, row, linesCount, angle));
}

bool rtm::WorldController::AddControlUnit_(ControlUnitShared controlUnit)
{
    controlUnits_.push_back(controlUnit);
    return true;
}

bool rtm::WorldController::AddStaticObject_(int column, int row, StaticShared staticObject)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return false;
    }

    if (IsEmpty_(column, row)) {
        mainLayer_->addChild(staticObject->GetSprite(), MAP_OBJECT_Z_ORDER);
        staticObjects_[GetVectorColumn_(column)][GetVectorRow_(row)] = staticObject;
        return true;
    }
    else {
        return false;
    }
}

bool rtm::WorldController::AddBuilding_(size_t type, int column, int row, float angle)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return false;
    }

    return AddStaticObject_(column, row, std::make_shared<BuildingObject>(type, column, row, angle));
}

bool rtm::WorldController::AddBush_(size_t type, int column, int row, float angle)
{
    if (!IsCorrectColumn(column) || !IsCorrectRow(row)) {
        return false;
    }

    return AddStaticObject_(column, row, std::make_shared<BushObject>(type, column, row, angle));
}

bool rtm::WorldController::AddDynamicObject_(int column, int row, DynamicShared dynamicObject)
{
    if (!IsAllowableColumn(column) || !IsAllowableRow(row)) {
        return false;
    }

    if (!dynamicObject->IsNearOthers(this)) {
        mainLayer_->addChild(dynamicObject->GetSprite(), VEHICLE_OBJECT_Z_ORDER);
        dynamicObjects_.push_back(dynamicObject);
        return true;
    }
    else {
        return false;
    }
}

bool rtm::WorldController::AddCar_(size_t type, int column, int row, float angle)
{
    if (!IsAllowableColumn(column) || !IsAllowableRow(row)) {
        return false;
    }

    return AddDynamicObject_(column, row, std::make_shared<CarObject>(type, column, row, angle));
}

size_t rtm::WorldController::GetVectorColumn_(int column)
{
    return static_cast<size_t>(hiddenArea_ + column);
}

size_t rtm::WorldController::GetVectorRow_(int row)
{
    return static_cast<size_t>(hiddenArea_ + row);
}

int rtm::WorldController::GetRealColumn_(size_t column)
{
    return static_cast<int>(column - hiddenArea_);
}

int rtm::WorldController::GetRealRow_(size_t row)
{
    return static_cast<int>(row - hiddenArea_);
}

void rtm::WorldController::CloseMap_()
{
    Pause();
    ClearSpawns_();
    ClearCoatingObjects_();
    ClearControlUnits_();
    ClearStaticObjects_();
    ClearDynamicObjects_();
}

void rtm::WorldController::ClearSpawns_()
{
    spawns_.clear();
}

void rtm::WorldController::ClearCoatingObjects_()
{
    for (auto& col : coatingUnions_) {
        for (auto& elem : col) {
            if (elem) {
                elem->ReleaseSprites(mainLayer_);
                elem.reset();
            }
        }
    }
}

void rtm::WorldController::ClearControlUnits_()
{
    controlUnits_.clear();
}

void rtm::WorldController::ClearStaticObjects_()
{
    for (auto& col : staticObjects_) {
        for (auto& elem : col) {
            if (elem) {
                if (elem->GetSprite() != nullptr) {
                    mainLayer_->removeChild(elem->GetSprite());
                }
                elem.reset();
            }
        }
    }
}

void rtm::WorldController::ClearDynamicObjects_()
{
    for (auto& obj : dynamicObjects_) {
        mainLayer_->removeChild(obj->GetSprite());
    }
    dynamicObjects_.clear();
}
