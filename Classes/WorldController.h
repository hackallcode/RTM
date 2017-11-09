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

        size_t GetColumnsCount() const;
        size_t GetRowsCount() const;
        float GetDeltaTime() const;

        CoatingObject* GetCoatingObject(int column, int row);
        StaticObject* GetStaticObject(int column, int row);
        std::vector<DynamicObjectUnique>& GetDynamicObjects();

        bool IsCorrectColumn(int column);
        bool IsCorrectRow(int row);
        bool IsAllowableColumn(int column);
        bool IsAllowableRow(int row);
        bool IsVisibleColumn(int column);
        bool IsVisibleRow(int row);

        bool LoadMap(std::string const& filename);
        bool LoadMap(MapNumber number);
        void Reset();        

        // TODO: Delete (only for test)
        void AddTestObjects();
        void AddCar(CarType type, int column, int row, float angle);
        void RemoveCoatingObjects();
        void RemoveStaticObjects();
        void RemoveDynamicObjects();

    private:
        WorldScene* scene_;
        size_t columnsCount_;
        size_t rowsCount_;
        float deltaTime_;

        cocos2d::Sprite* background_;
        std::vector<std::vector<CoatingUnionShared>> coatingUnions_;
        std::vector<std::vector<StaticObjectUnique>> staticObjects_;
        std::vector<DynamicObjectUnique> dynamicObjects_;

        bool IsEmpty(int column, int row, size_t width, size_t height);

        void GenerateObject_(char* params, char count);
        void SetBackground_(std::string const& filename);
        void SetBackground_(BackgroundNumber number);
        void AddDriveway_(int column, int row, size_t width, size_t height, DirectionType direction);
        void AddCrossroad_(int column, int row, size_t upLines,
            size_t toRightLines, size_t downLines, size_t toLeftLines);
        void AddTCrossroad_(int column, int row, size_t upLines,
            size_t toRightLines, size_t downLines, size_t toLeftLines, DirectionType nullDirection);
        void AddBuilding_(BuildingType type, int column, int row, float angle);
        void AddCar_(CarType type, int column, int row, float angle);

        void RemoveCoatingObjects_();
        void RemoveStaticObjects_();
        void RemoveDynamicObjects_();
        
        static std::string GetClassFile_(MapNumber number);
        static std::string GetBackgroundFile_(BackgroundNumber number);
    };
}

#endif // __MAP_CONTROLLER_INCLUDED__