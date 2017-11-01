#ifndef __GLOBAL_INCLUDED__
#define __GLOBAL_INCLUDED__

#include <cmath>
#include <string>
#include <array>
#include <memory>

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

namespace rtm {

    /* CONSTANTS */

    bool const LEFT{ false };
    bool const RIGHT{ true };

    float const F_PI_4{ 0.785398163397448309616f };
    float const F_PI_2{ 1.57079632679489661923f };
    float const F_PI{ 3.14159265358979323846f };
    float const F_2_PI{ 6.28318530717958647692f };

    float const DEG_RAD{ F_PI / 180.f };
    float const RAD_DEG{ 180.f / F_PI };

    /* ANGLES */

    float const ANGLE_TOP{ 0.f };
    float const ANGLE_RIGHT{ F_PI_2 };
    float const ANGLE_BOTTOM{ -F_PI };
    float const ANGLE_LEFT{ -F_PI_2 };

    /* DELTAS */

    float const ANGLE_DELTA{ 1.f * DEG_RAD };
    float const COORD_DELTA{ 1.f };

    /* MAP PARAMETERS */

    size_t const CELL_SIZE{ 30 };

    int const BACKGROUND_Z_ORDER{ 0 };
    int const COATING_Z_ORDER{ 1 };
    int const VEHICLE_Z_ORDER{ 2 };
    int const BUILDING_Z_ORDER{ 3 };

    /* AREA OF VISIBILITY */

    float const VIEW_RADIUS{ 75.f };
    float const VIEW_ANGLE{ 20.5f * DEG_RAD };
    float const VIEW_ANGLE_SHIFT{ 0.f };

    float const ROTATION_VIEW_RADIUS{ 55.f };
    float const ROTATION_VIEW_ANGLE{ 45.f * DEG_RAD };
    float const ROTATION_VIEW_ANGLE_SHIFT{ 30.f * DEG_RAD };

    float const LINE_CHANGING_VIEW_RADIUS{ 60.f };
    float const LINE_CHANGING_VIEW_ANGLE{ 30.f * DEG_RAD };
    float const LINE_CHANGING_VIEW_ANGLE_SHIFT{ 20.f * DEG_RAD };

    /* CLASSES */

    class World;
    class WorldController;
    class CoatingObject;
    class StaticObject;
    class DynamicObject;

    /* TYPES */

    using Directions = std::array<bool, 4>;

    using CoatingUnique = std::unique_ptr<CoatingObject>;
    using StaticUnique = std::unique_ptr<StaticObject>;
    using DynamicUnique = std::unique_ptr<DynamicObject>;

    /* ENUMS */

    enum StateType {
        NotStarted
        , MustStart
        , Started
        , MustStop
    };

    enum MapNumber {
        MapNumberNo1 = 1
    };

    enum RoadType {
        RoadTypeNo1 = 1
        , RoadTypeNo2
    };

    enum BuildingType {
        BuildingTypeNo1 = 1
        , BuildingTypeNo2
    };

    enum CarType {
        CarTypeNo1 = 1
        , CarTypeNo2
    };

    /* MAPS */

    std::string const MAP_BACKGROUND_FILE{ "res/background.png" };

    std::string const MAP_NO_0_FILE{ "res/map/MapNo0.rtmm" };
    std::string const MAP_NO_1_FILE{ "res/map/MapNo1.rtmm" };

    /* ROADS */

    std::string const ROAD_NO_0_FILE{ "res/coating/RoadNo0.png" }; // File path
    float const ROAD_NO_0_RESISTANCE{ 1.f }; // Coefficient of resistance
    Directions const ROAD_NO_0_DIRECTIONS{ false, false, false, false }; // Enabled directions (top, right, bottom, left)

    std::string const ROAD_NO_1_FILE{ "res/coating/RoadNo1.png" };
    float const ROAD_NO_1_RESISTANCE{ 1.f };
    Directions const ROAD_NO_1_DIRECTIONS{ true, false, true, false };

    std::string const ROAD_NO_2_FILE{ "res/coating/RoadNo2.png" };
    float const ROAD_NO_2_RESISTANCE{ 1.f };
    Directions const ROAD_NO_2_DIRECTIONS{ false, true, true, false };

    /* BUILDINGS */

    std::string const BUILDING_NO_0_FILE{ "res/static/BuildingNo0.png" };
    std::string const BUILDING_NO_1_FILE{ "res/static/BuildingNo1.png" };
    std::string const BUILDING_NO_2_FILE{ "res/static/BuildingNo2.png" };

    /* CARS */

    std::string const CAR_NO_0_FILE{ "res/vehicle/CarNo0.png" }; // File path
    float const CAR_NO_0_MAX_SPEED{ 0.f }; // Meters per second
    float const CAR_NO_0_ACCELERATION{ 0.f }; // Meters per second^2

    std::string const CAR_NO_1_FILE{ "res/vehicle/CarNo1.png" };
    float const CAR_NO_1_MAX_SPEED{ 21.f };
    float const CAR_NO_1_ACCELERATION{ 2.f };

    std::string const CAR_NO_2_FILE{ "res/vehicle/CarNo2.png" };
    float const CAR_NO_2_MAX_SPEED{ 36.f };
    float const CAR_NO_2_ACCELERATION{ 9.f };

    /* COMPARATORS */

    bool IsSameCoords(float a, float b, float delta = COORD_DELTA);
    float RoundCoord(float coord, float delta = COORD_DELTA);
    bool IsSameAngles(float a, float b, float delta = ANGLE_DELTA);
    float RoundAngle(float angle, float delta = ANGLE_DELTA);
    float NormalizeAngle(float angle);

    /* CONVERTERS */

    int PixelToCell(float coordinate);
    float CellToPixel(int cellNumber);
    float CellCenterRound(float coordinate);
    bool IsInCenter(float coordinate);

    /* OTHER */
    float CountDeceleration(float maxSpeed);
}

#endif // __GLOBAL_INCLUDED__