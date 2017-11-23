#ifndef __GLOBAL_INCLUDED__
#define __GLOBAL_INCLUDED__

#include <cstring>
#include <cmath>
#include <string>
#include <array>
#include <memory>
#include <fstream>
#include <vector>
#include <algorithm>
#include "cocos2d.h"
#include "fasttrigo.h"

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

    float const ANGLE_UP{ 0.f };
    float const ANGLE_RIGHT{ F_PI_2 };
    float const ANGLE_DOWN{ -F_PI };
    float const ANGLE_LEFT{ -F_PI_2 };
    float const ANGLE_UP_RIGHT{ F_PI_4 };
    float const ANGLE_DOWN_RIGHT{ F_PI - F_PI_4 };
    float const ANGLE_DOWN_LEFT{ -F_PI + F_PI_4 };
    float const ANGLE_UP_LEFT{ -F_PI_4 };

    /* DELTAS */

    float const ANGLE_DELTA{ 1.f * DEG_RAD };
    float const COORD_DELTA{ 1.f };

    /* MAP PARAMETERS */

    size_t const CELL_SIZE{ 30 };
    size_t const ROTATION_RADIUS{ CELL_SIZE };
    float const MIN_TIME_FACTOR{ 0.5f };
    float const MAX_TIME_FACTOR{ 4.f };

    int const BACKGROUND_Z_ORDER{ -1 };
    int const MAIN_Z_ORDER{ 0 };
    int const COATING_OBJECT_Z_ORDER{ 1 };
    int const LEFTWARD_SIGNAL_Z_ORDER{ 2 };
    int const RIGHTWARD_SIGNAL_Z_ORDER{ 3 };
    int const FORWARD_SIGNAL_Z_ORDER{ 4 };
    int const VEHICLE_OBJECT_Z_ORDER{ 5 };
    int const MAP_OBJECT_Z_ORDER{ 6 };

    /* AREA OF VISIBILITY */

    float const VIEW_RADIUS{ 60.f };
    float const VIEW_ANGLE{ 25.f * DEG_RAD };
    float const VIEW_ANGLE_SHIFT{ 0.f };

    float const ROTATION_VIEW_RADIUS{ 50.f };
    float const ROTATION_VIEW_ANGLE{ 30.5f * DEG_RAD };
    float const ROTATION_VIEW_ANGLE_SHIFT{ 29.5f * DEG_RAD };

    float const TURN_VIEW_RADIUS{ 65.f };
    float const TURN_VIEW_ANGLE{ 22.5f * DEG_RAD };
    float const TURN_VIEW_ANGLE_SHIFT{ -52.5f * DEG_RAD };

    float const LINE_CHANGING_VIEW_RADIUS{ 60.f };
    float const LINE_CHANGING_VIEW_ANGLE{ 30.f * DEG_RAD };
    float const LINE_CHANGING_VIEW_ANGLE_SHIFT{ 20.f * DEG_RAD };

    /* CLASSES */

    class WorldScene;
    class WorldController;
    class CoatingObject;
    class CoatingUnion;
    class ControlUnit;
    class WorldObject;
    class StaticObject;
    class DynamicObject;

    struct SpawnType {
        int column;
        int row;
        float angle;
    };

    /* TYPES */

    enum AngleType {
        NullAngle = -1
        , Up = 0
        , Right
        , Down
        , Left
        , UpRight
        , DownRight
        , DownLeft
        , UpLeft
    };

    enum DirectionType {
        NullDirection = -1
        , Upward = 0
        , Rightward
        , Downward
        , Leftward
    };

    enum CoatingUnionType {
        NoCoatingUnion = -1
        , DrivewayType
        , CrossroadType
        , TCrossroadType
        , TurnType
    };

    enum DirectionSignalIndex {
        ForwardSignalIndex = 0
        , LeftwardSignalIndex
        , RightwardSignalIndex
    };

    enum SignalType {
        NotWorking = 0
        , Allowed
        , Warning
        , Forbidden
        , Closed
    };

    enum StateType {
        NotStarted
        , MustStart
        , Started
        , MustStop
    };

    using WorldControllerUnique = std::unique_ptr<WorldController>;
    using SpawnVector = std::vector<SpawnType>;

    using CoatingUnique = std::unique_ptr<CoatingObject>;
    using CoatingVector = std::vector<CoatingUnique>;
    using CoatingMatrix = std::vector<CoatingVector>;

    using CoatingUnionShared = std::shared_ptr<CoatingUnion>;
    using CoatingUnionVector = std::vector<CoatingUnionShared>;
    using CoatingUnionMatrix = std::vector<CoatingUnionVector>;

    using ControlUnitShared = std::shared_ptr<ControlUnit>;
    using ControlUnitVector = std::vector<ControlUnitShared>;

    using StaticShared = std::shared_ptr<StaticObject>;
    using StaticVector = std::vector<StaticShared>;
    using StaticMatrix = std::vector<StaticVector>;

    using DynamicShared = std::shared_ptr<DynamicObject>;
    using DynamicVector = std::vector<DynamicShared>;

    using Directions = std::array<bool, 8>;
    using LinesCounts = std::array<size_t, 4>;

    using DirectionSignals = std::array<SignalType, 4>;
    using CrossroadSignals = std::array<DirectionSignals, 4>;
    
    DirectionSignals const DEFAULT_DIRECTIONS_SIGNALS = { NotWorking, NotWorking, NotWorking };
    CrossroadSignals const DEFAULT_CROSSROAD_SIGNALS = {
        DEFAULT_DIRECTIONS_SIGNALS
        , DEFAULT_DIRECTIONS_SIGNALS
        , DEFAULT_DIRECTIONS_SIGNALS
        , DEFAULT_DIRECTIONS_SIGNALS
    };

    using SignalSprites = std::array<cocos2d::Sprite*, 5>;
    using SignalsSprites = std::array<SignalSprites, 3>;
    using DirectionsSignalSprites = std::array<SignalsSprites, 4>;

    SignalSprites const DEFAULT_SIGNAL_SPRITES = { nullptr, nullptr, nullptr, nullptr, nullptr };
    SignalsSprites const DEFAULT_SIGNALS_SPRITES = { DEFAULT_SIGNAL_SPRITES, DEFAULT_SIGNAL_SPRITES, DEFAULT_SIGNAL_SPRITES };
    DirectionsSignalSprites const DEFAULT_DIRECTIONS_SIGNAL_SPRITES = {
        DEFAULT_SIGNALS_SPRITES
        , DEFAULT_SIGNALS_SPRITES
        , DEFAULT_SIGNALS_SPRITES
        , DEFAULT_SIGNALS_SPRITES
    };

    /* FILENAMES */

    std::string const BACKGROUND_FILENAME_MASK{ "res/background/BackgroundNo%No%.png" };
    std::string const MAP_FILENAME_MASK{ "res/map/MapNo%No%.rtmm" };
    std::string const ROAD_FILENAME_MASK{ "res/coating/RoadNo%No%.png" };
    std::string const SIGNAL_FILENAME_MASK{ "res/signal/SignalNo%No%.png" };
    std::string const BUILDING_FILENAME_MASK{ "res/static/BuildingNo%No%.png" };
    std::string const CAR_FILENAME_MASK{ "res/vehicle/CarNo%No%.png" };

    /* MAPS */

    enum MapNumber {
        MapNumberNo1 = 1
    };

    enum BackgroundNumber {
        BackgroundNumberNo1 = 1
        , BackgroundNumberNo2
    };

    /* COATINGS */

    enum CoatingType {
        AsphaltCoating = 0
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
        , RoadTypeNo15
        , RoadTypeNo16
        , RoadTypeNo17
        , RoadTypeNo18
    };

    // Coefficient of resistance
    std::array<float, 19> const ROADS_RESISTANCES = {
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
        , 1.f   // [15]
        , 1.f   // [16]
        , 1.f   // [17]
        , 1.f   // [18]
    };

    // Enabled directions (top, right, bottom, left, tor-right, bottom-right, bottom-left, top-left)
    std::array<Directions, 19> const ROADS_DIRECTIONS = {
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
        , Directions{ false, true, true, false, false, false, false, false }    // [15]
        , Directions{ false, true, true, false, false, false, false, false }    // [16]
        , Directions{ false, true, true, false, false, false, false, false }    // [17]
        , Directions{ false, false, false, false, false, false, false, false }  // [18]
    };

    /* CONTROL UNITS */

    enum ControlUnitType {
        NoControlUnit = 0
        , ControlUnitNo1 = 1
    };

    enum SignalFileId {
        ForwardSignalId = 1
        , LeftwardSignalId = 6
        , RightwardSignalId = 11
    };

    /* MAP OBJECT */

    enum BuildingType {
        BuildingTypeNo1 = 1
        , BuildingTypeNo2
        , BuildingTypeNo3
        , BuildingTypeNo4
        , BuildingTypeNo5
    };

    /* VEHICLE */

    enum CarType {
        CarTypeNo1 = 1
        , CarTypeNo2
        , CarTypeNo3
        , CarTypeNo4
        , CarTypeNo5
    };

    std::array<float, 6> const CARS_MAX_SPEEDS = {
          0.f   // [0]
        , 21.f  // [1]
        , 24.f  // [2]
        , 30.f  // [3]
        , 33.f  // [4]
        , 36.f  // [5]
    };

    std::array<float, 6> const CARS_ACCELERATIONS = {
          0.f   // [0]
        , 3.f   // [1]
        , 4.f   // [2]
        , 6.f   // [3]
        , 8.25f // [4]
        , 12.f  // [5]
    };

    /* COMPARATORS AND ROUNDERS */

    bool SameCoordinates(float a, float b, float delta = COORD_DELTA);
    float RoundCoordinate(float coordinate, float delta = COORD_DELTA);
    float RoundToCenter(float coordinate);
    bool InCenter(float coordinate, float delta = COORD_DELTA);
    float DistanceToNextCenter(float x, float y, float angle);
    bool CenterIsCrossed(float x, float y, float angle, float lastDelta);

    bool SameAngles(float a, float b, float delta = ANGLE_DELTA);
    float RoundAngle(float angle, float delta = ANGLE_DELTA);
    float NormalizeAngle(float angle);

    /* CONVERTERS */

    int PixelToCell(float coordinate);
    float CellToPixel(int cellNumber);
    AngleType AngleToAngleType(float angle);
    DirectionType AngleToDirection(float angle);
    float AngleTypeToAngle(AngleType angle);
    DirectionType AngleTypeToDirection(AngleType angle);
    float DirectionToAngle(DirectionType direction);
    AngleType DirectionToAngleType(DirectionType direction);

    /* OTHER */

    AngleType SumAngleTypes(AngleType a, AngleType b);
    float CountDeceleration(float maxSpeed);
}

#endif // __GLOBAL_INCLUDED__