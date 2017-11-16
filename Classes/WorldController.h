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

        WorldScene* GetScene() const;
        size_t GetColumnsCount() const;
        size_t GetRowsCount() const;
        float GetDeltaTime() const;
        int GetTimeFactor() const;

        CoatingObject* GetCoatingObject(int column, int row);
        CoatingUnion* GetCoatingUnion(int column, int row);
        StaticObject* GetStaticObject(int column, int row);
        std::vector<DynamicShared>& GetDynamicObjects();

        bool IsCorrectColumn(int column);
        bool IsCorrectRow(int row);
        bool IsAllowableColumn(int column);
        bool IsAllowableRow(int row);
        bool IsVisibleColumn(int column);
        bool IsVisibleRow(int row);

        void SetTimeFactor(int factor);

        bool LoadMap(std::string const& filename);
        bool LoadMap(MapNumber number);
        void Reset();

    private:
        WorldScene* scene_;
        size_t columnsCount_;
        size_t rowsCount_;
        float deltaTime_;
        int timeFactor_;

        cocos2d::Sprite* background_;
        std::vector<std::vector<CoatingUnionShared>> coatingUnions_;
        std::vector<ControlUnitShared> controlUnits_;
        std::vector<std::vector<StaticShared>> staticObjects_;
        std::vector<DynamicShared> dynamicObjects_;

        bool IsEmpty(int column, int row, size_t width, size_t height);

        void GenerateObject_(char* params, char count);

        void SetBackground_(std::string const& filename);
        void SetBackground_(BackgroundNumber number);

        void AddCoatingUnion_(int column, int row, CoatingUnionShared coatingUnion);
        void AddDriveway_(int column, int row, size_t width, size_t height, AngleType angle);
        void AddCrossroad_(int column, int row, LinesCounts linesCounts, 
            ControlUnitType controlUnitType = NoControlUnit);
        void AddTCrossroad_(int column, int row, LinesCounts linesCounts, AngleType nullDirection,
            ControlUnitType controlUnitType = NoControlUnit);
        void AddTurt_(int column, int row, size_t linesCount, bool isRight, AngleType angle);
        
        void AddControlUnit_(ControlUnitShared controlUnit);

        void AddStaticObject_(int column, int row, StaticShared staticObject);
        void AddBuilding_(BuildingType type, int column, int row, float angle);

        void AddDynamicObject_(int column, int row, DynamicShared dynamicObject);
        void AddCar_(CarType type, int column, int row, float angle);

        static inline size_t GetVectorColumn_(int column);
        static inline size_t GetVectorRow_(int row);
        static inline int GetRealColumn_(size_t column);
        static inline int GetRealRow_(size_t row);

        void RemoveCoatingObjects_();
        void RemoveStaticObjects_();
        void RemoveDynamicObjects_();
        
        static std::string GetClassFile_(MapNumber number);
        static std::string GetBackgroundFile_(BackgroundNumber number);
    };
}

#endif // __MAP_CONTROLLER_INCLUDED__