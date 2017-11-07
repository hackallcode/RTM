#ifndef __MAP_CONTROLLER_INCLUDED__
#define __MAP_CONTROLLER_INCLUDED__

#include "General.h"
#include "BuildingObject.h"
#include "CarObject.h"
#include "RoadObject.h"

namespace rtm {
    class WorldController
    {
    public:
        WorldController();
        WorldController(World* const scene);
        WorldController(World* const scene, std::string const& filename);
        WorldController(World* const scene, MapNumber number);

        void Update(float time);

        size_t GetColumnsCount() const;
        size_t GetRowsCount() const;
        float GetDeltaTime() const;

        CoatingUnique& GetCoatingObject(int column, int row);
        StaticUnique& GetStaticObject(int column, int row);
        std::vector<DynamicUnique>& GetDynamicObjects();

        bool IsCorrectColumn(int column);
        bool IsCorrectRow(int row);
        bool IsVisibleColumn(int row);
        bool IsVisibleRow(int row);

        bool LoadMap(std::string const& filename);
        bool LoadMap(MapNumber number);
        void AddCar(CarType type, int column, int row, float angle);
        void Reset();        

        // TODO: Make private
        void RemoveCoatingObjects_();
        void RemoveStaticObjects_();
        void RemoveDynamicObjects_();

    private:
        World* scene_;
        size_t columnsCount_;
        size_t rowsCount_;
        float deltaTime_;

        cocos2d::Sprite* background_;
        std::vector<std::vector<CoatingUnique>> coatingObjects_;
        std::vector<std::vector<StaticUnique>> staticObjects_;
        std::vector<DynamicUnique> dynamicObjects_;

        void SetBackground_(std::string const& filename);
        void SetRoad_(RoadType type, int column, int row, float angle);
        void SetBuilding_(BuildingType type, int column, int row, float angle);
        
        static std::string GetClassFile_(MapNumber number);
    };
}

#endif // __MAP_CONTROLLER_INCLUDED__