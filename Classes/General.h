#ifndef __GLOBAL_INCLUDED__
#define __GLOBAL_INCLUDED__

#include <cstring>
#include <cmath>
#include <string>
#include <array>
#include <memory>
#include <fstream>
#include <vector>
#include "cocos2d.h"
#include "fasttrigo.h"

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

    float const F_PI_8{ 0.392699081698724154808f };
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
    float const ANGLE_TOP_RIGHT{ F_PI_4 };
    float const ANGLE_BOTTOM_RIGHT{ F_PI_2 - F_PI_4 };
    float const ANGLE_BOTTOM_LEFT{ -F_PI + F_PI_4 };
    float const ANGLE_TOP_LEFT{ -F_PI_4 };

    /* DELTAS */

    float const ANGLE_DELTA{ 1.f * DEG_RAD };
    float const COORD_DELTA{ 1.f };

    /* MAP PARAMETERS */

    size_t const CELL_SIZE{ 30 };
    size_t const HIDDEN_AREA_SIZE{ 3 };

    int const BACKGROUND_Z_ORDER{ 0 };
    int const COATING_OBJECT_Z_ORDER{ 1 };
    int const VEHICLE_OBJECT_Z_ORDER{ 2 };
    int const MAP_OBJECT_Z_ORDER{ 3 };

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

    class WorldScene;
    class WorldController;
    class CoatingObject;
    class CoatingUnion;
    class WorldObject;
    class StaticObject;
    class DynamicObject;

    /* TYPES */

    using WorldControllerUnique = std::unique_ptr<WorldController>;
    using CoatingObjectUnique = std::unique_ptr<CoatingObject>;
    using CoatingUnionShared = std::shared_ptr<CoatingUnion>;
    using StaticObjectUnique = std::unique_ptr<StaticObject>;
    using DynamicObjectUnique = std::unique_ptr<DynamicObject>;

    using Directions = std::array<bool, 8>;
    enum DirectionType {
        TopDirection = 0
        , RightDirection
        , BottomDirection
        , LeftDirection
        , TopRightDirection
        , BottomRightDirection
        , BottomLeftDirection
        , TopLeftDirection
    };

    /* FILENAMES */

    std::string const BACKGROUND_FILENAME_MASK{ "res/background/BackgroundNo%No%.png" };
    std::string const MAP_FILENAME_MASK{ "res/map/MapNo%No%.rtmm" };
    std::string const ROAD_FILENAME_MASK{ "res/coating/RoadNo%No%.png" };
    std::string const BUILDING_FILENAME_MASK{ "res/static/BuildingNo%No%.png" };
    std::string const CAR_FILENAME_MASK{ "res/vehicle/CarNo%No%.png" };

    /* MAPS */

    enum MapNumber {
        MapNumberNo1 = 1
    };

    enum BackgroundNumber {
        BackgroundNumberNo1 = 1
    };

    /* COATINGS */

    enum CoatingType {
        CoatingUnionType = 0
        , DrivewayType
        , CrossroadType
        , TCrossroadType
    };

    enum RoadType {
        RoadTypeNo1 = 1
        , RoadTypeNo2
        , RoadTypeNo3
        , RoadTypeNo4
        , RoadTypeNo5
        , RoadTypeNo6
        , RoadTypeNo7
        , RoadTypeNo8
        , RoadTypeNo9
        , RoadTypeNo10
        , RoadTypeNo11
        , RoadTypeNo12
        , RoadTypeNo13
        , RoadTypeNo14
    };

    // Coefficient of resistance
    std::array<float, 15> const ROADS_RESISTANCES = {
          0.f   // [0]
        , 1.f   // [1]
        , 1.f   // [2]
        , 1.f   // [3]
        , 1.f   // [4]
        , 1.f   // [5]
        , 1.f   // [6]
        , 1.f   // [7]
        , 1.f   // [8]
        , 1.f   // [9]
        , 1.f   // [10]
        , 1.f   // [11]
        , 1.f   // [12]
        , 1.f   // [13]
        , 1.f   // [14]
    };

    // Enabled directions (top, right, bottom, left, tor-right, bottom-right, bottom-left, top-left)
    std::array<Directions, 15> const ROADS_DIRECTIONS = {
          Directions{ false, false, false, false, false, false, false, false }  // [0]
        , Directions{ true, false, true, false, false, false, false, false }    // [1]
        , Directions{ true, false, true, false, true, true, false, false }      // [2]
        , Directions{ true, false, true, false, true, true, true, true }        // [3]
        , Directions{ true, true, true, true, false, false, false, false }      // [4]
        , Directions{ true, true, true, true, false, false, false, false }      // [5]
        , Directions{ true, true, true, true, false, false, false, false }      // [6]
        , Directions{ true, true, true, true, false, false, false, false }      // [7]
        , Directions{ false, true, true, true, false, false, false, false }     // [8]
        , Directions{ false, true, true, true, false, false, false, false }     // [9]
        , Directions{ false, true, true, true, false, false, false, false }     // [10]
        , Directions{ false, true, true, true, false, false, false, false }     // [11]
        , Directions{ true, false, false, false, false, false, true, true }     // [12]
        , Directions{ false, false, true, false, false, false, true, true }     // [13]
        , Directions{ false, true, true, false, false, false, false, false }    // [14]
    };

    /* MAP OBJECT */

    enum BuildingType {
        BuildingTypeNo1 = 1
        , BuildingTypeNo2
    };

    /* VEHICLE */

    enum StateType {
        NotStarted
        , MustStart
        , Started
        , MustStop
    };

    enum CarType {
        CarTypeNo1 = 1
        , CarTypeNo2
    };

    std::array<float, 3> const CARS_MAX_SPEEDS = {
          0.f   // [0]
        , 21.f  // [1]
        , 36.f  // [2]
    };

    std::array<float, 3> const CARS_ACCELERATIONS = {
          0.f   // [0]
        , 2.f   // [1]
        , 9.f   // [2]
    };

    /* COMPARATORS AND ROUNDERS */

    bool IsSameCoords(float a, float b, float delta = COORD_DELTA);
    float RoundCoord(float coord, float delta = COORD_DELTA);
    bool IsSameAngles(float a, float b, float delta = ANGLE_DELTA);
    float RoundAngle(float angle, float delta = ANGLE_DELTA);
    float NormalizeAngle(float angle);
    bool IsInCenter(float coordinate);
    float CellCenterRound(float coordinate);

    /* CONVERTERS */

    int PixelToCell(float coordinate);
    float CellToPixel(int cellNumber);
    DirectionType AngleToDirection(float angle);
    float DirectionToAngle(DirectionType direction);

    /* OTHER */

    DirectionType DirectionsSum(DirectionType a, DirectionType b);
    float CountDeceleration(float maxSpeed);

    /* TODO: Delete */

    inline bool IsSeparator(char c);
    inline bool IsNotSeparator(char c);
    std::vector<int> Split(std::string const& str);

    int GetCaseNumber();
    void SetCaseNumber(int number);
}

#endif // __GLOBAL_INCLUDED__