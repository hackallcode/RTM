#ifndef __MAP_CONTROLLER_INCLUDED__
#define __MAP_CONTROLLER_INCLUDED__

#include "cocos2d.h"
#include <string>
#include "BuildingObject.h"
#include "CarObject.h"
#include "CoatingObject.h"

namespace rtm {

    // PARAMETERS
    size_t const CELL_SIZE = 30;

    // Z ORDERS
    int const BACKGROUND_Z_ORDER = 0;
    int const COATING_Z_ORDER = 1;
    int const VEHICLE_Z_ORDER = 2;
    int const BUILDING_Z_ORDER = 3;

    // MAPS
    enum MapNumber {
        MapNumberNo1 = 1
    };
    std::string const MAP_BACKGROUND_FILE = "res/background.png";
    std::string const MAP_NO_0_FILE = "res/map/MapNo0.rtmm";
    std::string const MAP_NO_1_FILE = "res/map/MapNo1.rtmm";

    using CoatingUnique = std::unique_ptr<CoatingObject>;
    using StaticUnique = std::unique_ptr<StaticObject>;
    using DynamicUnique = std::unique_ptr<DynamicObject>;
    using CoatingVector = std::vector<CoatingUnique>;

    class World;

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
        void AddBuilding(BuildingType type, int column, int row, float angle);
        void AddCar(CarType type, int column, int row, float angle);

        static size_t X2Col(float x);
        static size_t Y2Row(float y);
        static float Col2X(size_t column);
        static float Row2Y(size_t row);

        int caseNum;

    private:
        World* scene_;
        size_t columnsCount_;
        size_t rowsCount_;
        float deltaTime_;
        std::vector<CoatingVector> coatingObjects_;
        std::vector<StaticUnique> staticObjects_;
        std::vector<DynamicUnique> dynamicObjects_;

        void RemoveStaticObjects_();
        void RemoveDynamicObjects_();

        static std::string GetClassFile_(MapNumber number);
    };

}

#endif // __MAP_CONTROLLER_INCLUDED__