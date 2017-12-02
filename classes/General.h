﻿#ifndef __GLOBAL_INCLUDED__
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
#include "lib\fasttrigo.h"

/// @mainpage 
/// @brief Этот проект представляет из себя систему для моделирования дорожного движения.<br>
/// В нём можно тестировать системы управления светофорами, машинами (в ближайшем будущем это понадобится) и многое другое.<br>
/// А также можно просто позалипать на машинки, интересно катающиеся по дорогами, которые может создать любой человек! с:
/// @version 0.1.0
/// @author Владимир Северов (Vladimir Severov)
/// @todo Предстоит ещё много работы для серьезного использования данной системы, однако в рамках курсового проекта этот проект можно считать успешным.

/// Пространство имен для проекта RTM
namespace rtm {
    class WorldScene;
    class WorldController;
    class CoatingObject;
    class CoatingUnion;
    class ControlUnit;
    class WorldObject;
    class StaticObject;
    class DynamicObject;

    /// @name Константы для флага isRight
    /// @{

    /// Влево
    bool const LEFT{ false };
    /// Вправо
    bool const RIGHT{ true };

    /// @}

    /// @name Заранее посчитанные операции над &pi;
    /// @{

    /// &pi; / 8
    float const F_PI_8{ 0.392699081698724154808f };
    /// &pi; / 4
    float const F_PI_4{ 0.785398163397448309616f };
    /// &pi; / 2
    float const F_PI_2{ 1.57079632679489661923f };
    /// &pi;
    float const F_PI{ 3.14159265358979323846f };
    /// 2 * &pi;
    float const F_2_PI{ 6.28318530717958647692f };

    /// @}

    /// @name Константы для конвертации углов из радиан в градусы и обратно
    /// @{

    /// Коэффициент для перевода из градусов в радианы
    float const DEG_RAD{ F_PI / 180.f };
    /// Коэффициент для перевода из радиан в градусы
    float const RAD_DEG{ 180.f / F_PI };

    /// @}

    /// @name Заранее посчитанные углы
    /// @{

    /// Угол вверх
    float const ANGLE_UP{ 0.f };
    /// Угол вправо
    float const ANGLE_RIGHT{ F_PI_2 };
    /// Угол вниз
    float const ANGLE_DOWN{ -F_PI };
    /// Угол влево
    float const ANGLE_LEFT{ -F_PI_2 };
    /// Угол по диагонали вверх вправо
    float const ANGLE_UP_RIGHT{ F_PI_4 };
    /// Угол по диагонали вниз вправо
    float const ANGLE_DOWN_RIGHT{ F_PI - F_PI_4 };
    /// Угол по диагонали вниз влево
    float const ANGLE_DOWN_LEFT{ -F_PI + F_PI_4 };
    /// Угол по диагонали вверх влево
    float const ANGLE_UP_LEFT{ -F_PI_4 };

    /// @}

    /// @name Допустимые погрешности
    /// @{

    /// Погрешность для углов
    float const ANGLE_DELTA{ 1.f * DEG_RAD };
    /// Погрешность для координат
    float const COORD_DELTA{ 1.f };
    /// Максимальное расстояние до объектов, которые недалеко
    /// @see DynamicObject
    float const NEAR_DELTA{ 1.f };

    /// @}

    /// @name Парамметры карт
    /// @{

    /// Длина (ширина) ячейки карты
    size_t const CELL_SIZE{ 30 };
    /// Желаемый радиус поворота транспорта
    size_t const ROTATION_RADIUS{ CELL_SIZE };
    /// Минимальный коэффициент ускорения времени. Если меньше 1, то замедлениие
    float const MIN_TIME_FACTOR{ 0.5f };
    /// Максимальный коэффициент ускорения времени
    float const MAX_TIME_FACTOR{ 4.f };

    /// @}

    /// @name Номера слоев для разных объектов. Чем больше, тем выше (ближе к нам)
    /// @{

    /// Номер слоя для слоя фона
    int const BACKGROUND_Z_ORDER{ -1 };
    /// Номер слоя для главного слоя (на нем все объекты)
    int const MAIN_Z_ORDER{ 0 };
    /// Номер слоя для покрытий (дорог)
    int const COATING_OBJECT_Z_ORDER{ 1 };
    /// Номер слоя для левых стрелок светофора
    int const LEFTWARD_SIGNAL_Z_ORDER{ 2 };
    /// Номер слоя для правых стрелок светофора
    int const RIGHTWARD_SIGNAL_Z_ORDER{ 3 };
    /// Номер слоя для сфетофора в прямом направлении
    int const FORWARD_SIGNAL_Z_ORDER{ 4 };
    /// Номер слоя для транспорта
    int const VEHICLE_OBJECT_Z_ORDER{ 5 };
    /// Номер слоя для статичных объектов карты
    int const MAP_OBJECT_Z_ORDER{ 6 };

    /// @}

    /// @name Область видимости при движении вперед
    /// @{

    /// Радиус
    float const VIEW_RADIUS{ 60.f };
    /// Ширина угла в каждую сторону
    float const VIEW_ANGLE{ 24.f * DEG_RAD };
    /// Сдвиг области обзора
    float const VIEW_ANGLE_SHIFT{ 0.f };

    /// @}

    /// @name Область видимости при повороте
    /// @{

    /// Радиус
    float const ROTATION_VIEW_RADIUS{ 50.f };
    /// Ширина угла в каждую сторону
    float const ROTATION_VIEW_ANGLE{ 30.5f * DEG_RAD };
    /// Сдвиг области обзора
    float const ROTATION_VIEW_ANGLE_SHIFT{ 29.5f * DEG_RAD };

    /// @}

    /// @name Область видимости незадолго до поворота
    /// @{

    /// Радиус
    float const TURN_VIEW_RADIUS{ 75.f };
    /// Ширина угла в каждую сторону
    float const TURN_VIEW_ANGLE{ 30.f * DEG_RAD };
    /// Сдвиг области обзора
    float const TURN_VIEW_ANGLE_SHIFT{ 10.f * DEG_RAD };

    /// @}

    /// @name Область видимости на нерегулируемом перекрестке
    /// @{

    /// Радиус
    float const CROSSROAD_VIEW_RADIUS{ 65.f };
    /// Ширина угла в каждую сторону
    float const CROSSROAD_VIEW_ANGLE{ 22.5f * DEG_RAD };
    /// Сдвиг области обзора
    float const CROSSROAD_VIEW_ANGLE_SHIFT{ -52.5f * DEG_RAD };

    /// @}

    /// @name Область видимости до перестроения
    /// @{

    /// Радиус
    float const LINE_CHANGING_VIEW_RADIUS{ 60.f };
    /// Ширина угла в каждую сторону
    float const LINE_CHANGING_VIEW_ANGLE{ 30.f * DEG_RAD };
    /// Сдвиг области обзора
    float const LINE_CHANGING_VIEW_ANGLE_SHIFT{ 20.f * DEG_RAD };

    /// @}

    /// Структура, описывающая параметры точки генерации объектов
    struct SpawnType {
        int column; ///< Номер столбца
        int row; ///< Номер строки  
        float angle; ///< Первоначальный угол для транспорта
    };

    /// Тип для определения положения некоторых объектов и индикации разрешенных направлений на кусочке объекта
    enum AngleType {
        NullAngle = -1 ///< Неинициализированный угол
        , Up = 0 ///< Вверх
        , Right ///< Вправо
        , Down ///< Вниз
        , Left ///< Влево
        , UpRight ///< По диагонали вверх вправо
        , DownRight ///< По диагонали вниз влево
        , DownLeft ///< По диагонали вниз влево
        , UpLeft ///< По диагонали вверх вправо
    };

    /// Тип для задания направления движения транспорта
    enum DirectionType {
        NullDirection = -1 ///< Неинициализированное напрвление
        , Upward = 0 ///< Направление вверх
        , Rightward ///< Направление вправо
        , Downward ///< Направление вниз
        , Leftward ///< Направление влево
    };

    /// Возможные типы дорожных объединений
    enum CoatingUnionType {
        NoCoatingUnion = -1 ///< Неинициализированный тип
        , DrivewayType ///< Прямая дорога
        , CrossroadType ///< Обычный перекресток
        , TCrossroadType ///< Т-образный перекресток
        , TurnType ///< Поворот
    };

    /// Индексы массива для каждого типа сигнала
    enum DirectionSignalIndex {
        ForwardSignalIndex = 0 ///< Сигнал в прямом напрвлении
        , LeftwardSignalIndex ///< Сигнал в при повороте налево
        , RightwardSignalIndex ///< Сигнал в при повороте направо
    };

    /// Возможные сигналы светофора
    enum SignalType {
        NotWorking = 0 ///< Светофор не работает (равносильно его отсутствию)
        , Allowed ///< Зеленый сигнал
        , Warning ///< Желтый сигнал
        , Forbidden ///< Красный сигнал
        , Closed ///< В данном напрвлении движение запрещено
    };

    /// Возможные состояния для манёвров (движение, поворот, перестроение)
    enum StateType {
        NotStarted ///< Не начато (не выполняется)
        , MustStart ///< Необходимо начать
        , Started ///< Начато
        , MustStop ///< Необходимо закончить
    };

    /// Индексы, начиная с которых начинаются текстуры покрытий определенного типа
    enum CoatingType {
        AsphaltCoating = 0 ///< Асфальтовое объект
    };

    /// Индексы, начиная с которых начинаются текстуры сигналоа определенного типа
    enum SignalFileId {
        ForwardSignalId = 1 ///< Индекс сигнала для движения вперед
        , LeftwardSignalId = 6 ///< Индекс сигнал для поворота налево
        , RightwardSignalId = 11 ///< Индекс сигнал для поворота направо
    };

    /// Умный указатель для класса WorldController
    using WorldControllerUnique = std::unique_ptr<WorldController>;
    /// Массив точек генерации объектов
    using SpawnVector = std::vector<SpawnType>;

    /// Умный указатель для класса CoatingObject
    using CoatingUnique = std::unique_ptr<CoatingObject>;
    /// Массив объектов класса CoatingObject
    using CoatingVector = std::vector<CoatingUnique>;
    /// Матрица объектов класса CoatingObject
    using CoatingMatrix = std::vector<CoatingVector>;

    /// Умный указатель для класса CoatingUnion
    using CoatingUnionShared = std::shared_ptr<CoatingUnion>;
    /// Массив объектов класса CoatingUnion
    using CoatingUnionVector = std::vector<CoatingUnionShared>;
    /// Матрица объектов класса CoatingUnion
    using CoatingUnionMatrix = std::vector<CoatingUnionVector>;

    /// Умный указатель для класса ControlUnit
    using ControlUnitShared = std::shared_ptr<ControlUnit>;
    /// Массив объектов класса ControlUnit
    using ControlUnitVector = std::vector<ControlUnitShared>;

    /// Умный указатель для класса StaticObject
    using StaticShared = std::shared_ptr<StaticObject>;
    /// Массив объектов класса StaticObject
    using StaticVector = std::vector<StaticShared>;
    /// Матрица объектов класса StaticObject
    using StaticMatrix = std::vector<StaticVector>;

    /// Умный указатель для класса DynamicObject
    using DynamicShared = std::shared_ptr<DynamicObject>;
    /// Массив объектов класса DynamicObject
    using DynamicVector = std::vector<DynamicShared>;

    /// Массив возможных направлений движений по кусочку объекта
    /// @see AngleType
    using Directions = std::array<bool, 8>;
    /// Массив количества полос в каждом напрвлении для перекрестков
    /// @see DirectionType
    using LinesCounts = std::array<size_t, 4>;

    /// Массив сигналов из одного напрвлениия в каждое
    /// @see DirectionType
    using DirectionSignals = std::array<SignalType, 4>;
    /// Массив сигналов для всех напрвлений перекрестка
    /// @see DirectionType
    using CrossroadSignals = std::array<DirectionSignals, 4>;

    /// Массив всех текстур сигналов из одного направления в одно
    /// @see SignalType
    using SignalSprites = std::array<cocos2d::Sprite*, 5>;
    /// Массив всех текстур сигналов из одного направления в каждое (вперед, влево, вправо)
    /// @see DirectionSignalIndex
    using SignalsSprites = std::array<SignalSprites, 3>;
    /// Массив всех текстур сигналов для перекрестка
    /// @see DirectionType
    using DirectionsSignalSprites = std::array<SignalsSprites, 4>;

    /// @name Значения по умолчанию
    /// @{

    /// Значения по умолчанию для массива сигналов в одном направлении (светофора в этом направлении нет)
    DirectionSignals const DEFAULT_DIRECTIONS_SIGNALS = { NotWorking, NotWorking, NotWorking };
    /// Значения по умолчанию для массива сигналов всего перекрестка (светофора на перекрестке нет)
    CrossroadSignals const DEFAULT_CROSSROAD_SIGNALS = {
        DEFAULT_DIRECTIONS_SIGNALS
        , DEFAULT_DIRECTIONS_SIGNALS
        , DEFAULT_DIRECTIONS_SIGNALS
        , DEFAULT_DIRECTIONS_SIGNALS
    };

    /// Пустой массив текстур сигналов для одного типа сигнала одного направления
    SignalSprites const DEFAULT_SIGNAL_SPRITES = { nullptr, nullptr, nullptr, nullptr, nullptr };
    /// Пустой массив текстур сигналов для одного направления
    SignalsSprites const DEFAULT_SIGNALS_SPRITES = { DEFAULT_SIGNAL_SPRITES, DEFAULT_SIGNAL_SPRITES, DEFAULT_SIGNAL_SPRITES };
    /// Пустой массив текстур сигналов для всего перекрестка
    DirectionsSignalSprites const DEFAULT_DIRECTIONS_SIGNAL_SPRITES = {
        DEFAULT_SIGNALS_SPRITES
        , DEFAULT_SIGNALS_SPRITES
        , DEFAULT_SIGNALS_SPRITES
        , DEFAULT_SIGNALS_SPRITES
    };

    /// @}

    /// @name Маски названий файлов
    /// @{

    /// Маска файлов фонов
    std::string const BACKGROUND_FILENAME_MASK{ "res/background/BackgroundNo%No%.png" };
    /// Маска файлов карт
    std::string const MAP_FILENAME_MASK{ "res/map/MapNo%No%.rtmm" };
    /// Маска файлов текстур дорог
    std::string const ROAD_FILENAME_MASK{ "res/coating/RoadNo%No%.png" };
    /// Маска файлов текстур сигналов
    std::string const SIGNAL_FILENAME_MASK{ "res/signal/SignalNo%No%.png" };
    /// Маска файлов текстур зданий
    std::string const BUILDING_FILENAME_MASK{ "res/static/BuildingNo%No%.png" };
    /// Маска файлов текстур машин
    std::string const CAR_FILENAME_MASK{ "res/vehicle/CarNo%No%.png" };

    /// @}

    /// @name Параметры дорог
    /// @{

    /// Массив коэффициентов трения для каждого типа объекта
    /// @see CoatingType
    std::array<float, 2> const ROADS_RESISTANCES = {
        0.f     ///< Неинициализированный тип
        , 1.f   ///< Асфальт
    };

    /// Массив возможных направлений для каждой типа кучоска дороги
    /// @see RoadCoating
    std::array<Directions, 19> const ROADS_DIRECTIONS = {
          Directions{ false, false, false, false, false, false, false, false }  ///< Not init directions
        , Directions{ true, false, true, false, false, false, false, false }    ///< For road #1
        , Directions{ true, false, true, false, true, true, false, false }      ///< For road #2
        , Directions{ true, false, true, false, true, true, true, true }        ///< For road #3
        , Directions{ true, true, true, true, false, false, false, false }      ///< For road #4
        , Directions{ true, true, true, true, false, false, false, false }      ///< For road #5
        , Directions{ true, true, true, true, false, false, false, false }      ///< For road #6
        , Directions{ true, true, true, true, false, false, false, false }      ///< For road #7
        , Directions{ false, true, true, true, false, false, false, false }     ///< For road #8
        , Directions{ false, true, true, true, false, false, false, false }     ///< For road #9
        , Directions{ false, true, true, true, false, false, false, false }     ///< For road #10
        , Directions{ false, true, true, true, false, false, false, false }     ///< For road #11
        , Directions{ true, false, false, false, false, false, true, true }     ///< For road #12
        , Directions{ false, false, true, false, false, false, true, true }     ///< For road #13
        , Directions{ false, true, true, false, false, false, false, false }    ///< For road #14
        , Directions{ false, true, true, false, false, false, false, false }    ///< For road #15
        , Directions{ false, true, true, false, false, false, false, false }    ///< For road #16
        , Directions{ false, true, true, false, false, false, false, false }    ///< For road #17
        , Directions{ false, false, false, false, false, false, false, false }  ///< For road #18
    };

    /// @}

    /// @name Параметры машин
    /// @{

    /// Массив максимальных скоростей для машин
    /// @see CarObject
    std::array<float, 6> const CARS_MAX_SPEEDS = {
          0.f   ///< For car #0
        , 21.f  ///< For car #1
        , 24.f  ///< For car #2
        , 30.f  ///< For car #3
        , 33.f  ///< For car #4
        , 36.f  ///< For car #5
    };

    /// Массив ускорений для машин
    /// @see CarObject
    std::array<float, 6> const CARS_ACCELERATIONS = {
          0.f   ///< For car #0
        , 3.f   ///< For car #1
        , 4.f   ///< For car #2
        , 6.f   ///< For car #3
        , 8.25f ///< For car #4
        , 12.f  ///< For car #5
    };

    /// @}

    /// @name Функции для работы с параметрами положения объектов
    /// @{

    /// Функция для сравнения двух координат с определенной точностью
    /// @param a,b координаты, которые будут сравниваться
    /// @param delta максимальная разность между координатами
    /// @return результат сравнения
    bool SameCoordinates(float a, float b, float delta = COORD_DELTA);
    /// Функция пытается округлить координаты до центра клетки
    /// @param coordinate координата, которую будем пытаться округлить
    /// @param delta максимальное расстояние до центра клетки
    /// @return если координата достаточно близка к центру, то координаты центра, иначе саму координату
    float RoundCoordinate(float coordinate, float delta = COORD_DELTA);
    /// Функция для округления координаты до центра клетки
    /// @param coordinate округляемая координата
    /// @return координата ближайшего центра клетки
    float RoundToCenter(float coordinate);
    /// Функция для проверки координаты на центральность
    /// @param coordinate координата, которую проверяем
    /// @param delta максимальное расстояние до центра клетки
    /// @return true, если в центре клетки, иначе false
    bool InCenter(float coordinate, float delta = COORD_DELTA);
    /// Функция для нахождения расстояния до следующего центра клетка по ходу движения
    /// @param x,y координаты объекта
    /// @param angle направление движения (угол)
    /// @return расстояние до центра
    float DistanceToNextCenter(float x, float y, float angle);
    /// Функция проверяет, пересек ли объект центр клетки (центральную линию, перпендикулярную направлению движения)
    /// @param x,y координаты объекта
    /// @param angle направление движения (угол)
    /// @param lastDelta расстояние, которое объект прошёл за последнее перемещение
    /// @return true, если пересек какой-либо центр
    bool CenterIsCrossed(float x, float y, float angle, float lastDelta);

    /// Фукнция для сравнения двух углов
    /// @param a,b углы, которые будут сравниваться
    /// @param delta максимальная разность между углами
    /// @return результат сравнения
    bool SameAngles(float a, float b, float delta = ANGLE_DELTA);
    /// Фукнция пытается округлить угол до одно из главных направлений (период &pi;/4, т.е. 0, &pi;/4, &pi;/2, ...)
    /// @param angle угол, который будем пытаться округлить
    /// @param delta максимальная разность между исходным углом и округленным углом
    /// @return округленный угол, если исходный был достаточно близок, иначе исходный угол
    float RoundAngle(float angle, float delta = ANGLE_DELTA);
    /// Функция для нормализации угла до диапазона [-&pi;/2;&pi;/2)
    /// @param angle угол, который будем нормализовывать
    /// @return нормализованный угол
    float NormalizeAngle(float angle);

    /// @}

    /// @name Конверторы схожих типов
    /// @{

    /// Функция для конвертации координаты в номер ячейки
    /// @param coordinate координата, которая будет конвертирована
    /// @return номер ячейки
    int PixelToCell(float coordinate);
    /// Функция для конвертации номера ячейки в координату центра
    /// @param cellNumber номер ячейки, который будет конвертирован
    /// @return координата центра
    float CellToPixel(int cellNumber);

    /// Функция для конвертации угла в угловой тип
    /// @param angle угол, который будет конвертирован
    /// @return соответствующий угловой тип
    AngleType AngleToAngleType(float angle);
    /// Функция для конвертации угла в напраление
    /// @param angle угол, который будет конвертирован
    /// @return соответствующее напраление
    DirectionType AngleToDirection(float angle);

    /// Функция для конвертации углового типа в угол
    /// @param angle уголовой тип, который будет конвертирован
    /// @return соответствующий угол
    float AngleTypeToAngle(AngleType angle);
    /// Функция для конвертации углового типа в направление
    /// @param angle уголовой тип, который будет конвертирован
    /// @return соответствующее направление
    DirectionType AngleTypeToDirection(AngleType angle);

    /// Функция для конвертации направления в угол
    /// @param direction направление, которое будет конвертировано
    /// @return соответствующий угол
    float DirectionToAngle(DirectionType direction);
    /// Функция для конвертации направления в угловой тип
    /// @param direction направление, которое будет конвертировано
    /// @return соответствующий угловой тип
    AngleType DirectionToAngleType(DirectionType direction);

    /// @}

    /// Функция для суммирования двух угловых типов
    /// @param a,b угловые типы, которые будут складываться
    /// @return сумма угловых типов (a + b)
    AngleType SumAngleTypes(AngleType a, AngleType b);
    /// Функция для подсчёта рекомендуемого коэффициента замедления транспорта
    /// @param maxSpeed максимальная скорость транспорта
    /// @return рекомендуемый коэффициент замедления 
    float CountDeceleration(float maxSpeed);
}

#endif // __GLOBAL_INCLUDED__