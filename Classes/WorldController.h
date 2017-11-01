#ifndef __MAP_CONTROLLER_INCLUDED__
#define __MAP_CONTROLLER_INCLUDED__

#include "cocos2d.h"
#include <string>
#include "Global.h"
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
        CoatingUnique& GetCoating(int column, int row);
        std::vector<StaticUnique>& GetStaticObjects();
        std::vector<DynamicUnique>& GetDynamicObjects();

        void Reset();
        void AddRoad(RoadType type, int column, int row, float angle);
        void AddBuilding(BuildingType type, int column, int row, float angle);
        void AddCar(CarType type, int column, int row, float angle);
        void AddTestMap();

        int caseNum;
    private:
        World* scene_;
        size_t columnsCount_;
        size_t rowsCount_;
        float deltaTime_;
        std::vector<std::vector<CoatingUnique>> coatingObjects_;
        std::vector<StaticUnique> staticObjects_;
        std::vector<DynamicUnique> dynamicObjects_;

        void RemoveCoatingObjects_();
        void RemoveStaticObjects_();
        void RemoveDynamicObjects_();

        static std::string GetClassFile_(MapNumber number);
    };
}

#endif // __MAP_CONTROLLER_INCLUDED__