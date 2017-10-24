#ifndef __MAP_CONTROLLER_INCLUDED__
#define __MAP_CONTROLLER_INCLUDED__

#include "BuildingObject.h"
#include "CarObject.h"

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

    class World;

    class MapController
    {
    public:
        MapController();
        MapController(World* const scene);
        MapController(World* const scene, std::string const& filename);
        MapController(World* const scene, MapNumber number);

        void Update(float time);

        size_t GetRow(float y);
        size_t GetColumn(float x);
        float GetDeltaTime() const;
        std::vector<std::unique_ptr<StaticObject>>& getStaticObjects();
        std::vector<std::unique_ptr<DynamicObject>>& getDynamicObjects();

        void reset();
        void addBuilding(BuildingType type, int row, int column, float angle);
        void addCar(CarType type, int row, int column, float angle);

    private:
        World* scene_;
        size_t rowsCount_;
        size_t columnsCount_;
        float deltaTime_;
        std::vector<std::unique_ptr<StaticObject>> staticObjs_;
        std::vector<std::unique_ptr<DynamicObject>> dynamicObjs_;

        void removeStaticObjects_();
        void removeDynamicObjects_();

        static std::string GetClassFile_(MapNumber number);
    };

}

#endif // __MAP_CONTROLLER_INCLUDED__