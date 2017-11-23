#ifndef __MAP_CONTROLLER_INCLUDED__
#define __MAP_CONTROLLER_INCLUDED__

#include "General.h"

namespace rtm {
    class WorldController
    {
    public:
        WorldController();
        WorldController(WorldScene* const scene);
        WorldController(WorldScene* const scene, std::string const& filename);
        WorldController(WorldScene* const scene, MapNumber number);

        void Update(float time);

        cocos2d::Layer* GetScene() const;
        size_t GetColumnsCount() const;
        size_t GetRowsCount() const;
        float GetDeltaTime() const;
        float GetTimeFactor() const;

        CoatingObject* GetCoatingObject(int column, int row);
        CoatingUnion* GetCoatingUnion(int column, int row);
        StaticObject* GetStaticObject(int column, int row);
        std::vector<DynamicShared>& GetDynamicObjects();

        bool IsPause();
        bool IsCorrectColumn(int column);
        bool IsCorrectRow(int row);
        bool IsAllowableColumn(int column);
        bool IsAllowableRow(int row);
        bool IsVisibleColumn(int column);
        bool IsVisibleRow(int row);

        void SetTimeFactor(float factor);

        bool LoadMap(std::string const& filename);
        bool LoadMap(MapNumber number);
        void SpawnCar();
        void RemoveAccidents();
        void RemoveVehicles();
        void Play();
        void Pause();
        void Reset();

    private:
        WorldScene* scene_;
        cocos2d::Layer* mainLayer_;
        bool isPause_;
        uint8_t hiddenArea_;
        uint16_t columnsCount_;
        uint16_t rowsCount_;
        SpawnVector spawns_;
        float deltaTime_;
        float spawnTime_;
        float timeFactor_;
        std::string lastMapFile_;

        CoatingUnionMatrix coatingUnions_;
        ControlUnitVector controlUnits_;
        StaticMatrix staticObjects_;
        DynamicVector dynamicObjects_;

        bool IsEmpty(int column, int row, size_t width = 1, size_t height = 1);

        bool GenerateObject_(uint8_t* params, uint8_t count);
        
        bool AddCoatingUnion_(int column, int row, CoatingUnionShared coatingUnion);
        bool AddDriveway_(int column, int row, size_t width, size_t height, AngleType angle);
        bool AddCrossroad_(int column, int row, LinesCounts linesCounts,
            ControlUnitType controlUnitType = NoControlUnit);
        bool AddTCrossroad_(int column, int row, LinesCounts linesCounts, AngleType nullDirection,
            ControlUnitType controlUnitType = NoControlUnit);
        bool AddTurt_(int column, int row, size_t linesCount, AngleType angle, bool isRight);
        
        bool AddControlUnit_(ControlUnitShared controlUnit);

        bool AddStaticObject_(int column, int row, StaticShared staticObject);
        bool AddBuilding_(BuildingType type, int column, int row, float angle);

        bool AddDynamicObject_(int column, int row, DynamicShared dynamicObject, bool isSafe = true);
        bool AddCar_(CarType type, int column, int row, float angle, bool isSafe = true);

        inline size_t GetVectorColumn_(int column);
        inline size_t GetVectorRow_(int row);
        inline int GetRealColumn_(size_t column);
        inline int GetRealRow_(size_t row);

        void CloseMap_();
        void ClearSpawns_();
        void ClearCoatingObjects_();
        void ClearControlUnits_();
        void ClearStaticObjects_();
        void ClearDynamicObjects_();
        
        static std::string GetClassFile_(MapNumber number);
    };
}

#endif // __MAP_CONTROLLER_INCLUDED__