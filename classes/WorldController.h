#ifndef __MAP_CONTROLLER_INCLUDED__
#define __MAP_CONTROLLER_INCLUDED__

#include "General.h"

namespace rtm {

    /// @brief Класс контроллера карты, связующее звено всех объектов
    class WorldController
    {
    public:
        /// @brief Конструктор по умолчанию
        WorldController();
        /// @brief Конструктор без загрузки какой-либо карты
        /// @param scene сцена, к которой привязан контроллер
        WorldController(WorldScene* const scene);        
        /// @brief Конструктор с загрузкой карты
        /// @param scene сцена, к которой привязан контроллер
        /// @param filename путь к карте
        WorldController(WorldScene* const scene, std::string const& filename);        
        /// @brief Конструктор с загрузкой карты
        /// @param scene сцена, к которой привязан контроллер
        /// @param mapNumber номер стандартной карты
        WorldController(WorldScene* const scene, size_t mapNumber);

        /// @brief Функция обновления
        /// @param time время, прошедшее с момента прошлого обновления (в секундах)
        void Update(float time);

        /// @brief Функция для получения основного слоя сцены (на нём вся движуха)
        /// @return основной слой
        cocos2d::Layer* GetLayer() const;
        /// @brief Функция для получения количества столбцов карты
        /// @return количество столбцов
        size_t GetColumnsCount() const;
        /// @brief Функция для получения количества строк карты
        /// @return количество строк
        size_t GetRowsCount() const;
        /// @brief Функция для получения последней разницей между обновлениями
        /// @return разница во времени между обновлениями (в секундах)
        float GetDeltaTime() const;
        /// @brief Функция для получения коэффициента ускорения времени
        /// @return коэффициент ускорения времени (1 - реальная скорость)
        float GetTimeFactor() const;
        /// @brief Функция для получения объекта в определенной клетке
        /// @return указатель на объект объекта
        CoatingObject* GetCoatingObject(int column, int row);        
        /// @brief Функция для получения объединения покрытий в определенной клетке
        /// @return указатель на объект объединения покрытий
        CoatingUnion* GetCoatingUnion(int column, int row);        
        /// @brief Функция для получения статического объекта в определенной клетке
        /// @return указатель на статический объект
        StaticObject* GetStaticObject(int column, int row);        
        /// @brief Функция для получения массива движущихся объектов
        /// @return массив движущихся объектов
        std::vector<DynamicShared>& GetDynamicObjects();

        /// @brief Функция сообщает, происходят ли обновления
        /// @return true, если происходят не происходят (стоит пауза), иначе false
        bool IsPause();
        /// @brief Функция проверяет корректность столбца
        /// @param column номер проверяемой колонки
        /// @return true, если столбец корректный, иначе false
        bool IsCorrectColumn(int column);        
        /// @brief Функция проверяет корректность строки
        /// @param row номер проверяемой строки
        /// @return true, если строка корректная, иначе false
        bool IsCorrectRow(int row);
        /// @brief Функция проверяет, можно ли двигаться в столбце
        /// @param column номер проверяемой колонки
        /// @return true, если в столбце можно двигаться, иначе false
        bool IsAllowableColumn(int column);
        /// @brief Функция проверяет, можно ли двигаться в строке
        /// @param row номер проверяемой строки
        /// @return true, если в строке можно двигаться, иначе false
        bool IsAllowableRow(int row);
        /// @brief Функция проверяет видимость столбца
        /// @param column номер проверяемой колонки
        /// @return true, если столбец виден, иначе false
        bool IsVisibleColumn(int column);
        /// @brief Функция проверяет видимость строки
        /// @param row номер проверяемой строки
        /// @return true, если строка видна, иначе false
        bool IsVisibleRow(int row);

        /// @brief Функция для установки коэффициента ускорения времени
        /// @param factor коэффициент ускорения времени (1 - реальная скорость)
        void SetTimeFactor(float factor);

        /// @brief Функция для загрузки карты из файла
        /// @param filename полный путь к файлу с картой
        bool LoadMap(std::string const& filename);
        /// @brief Функция для загрузки карты по номеру
        /// @param number номер стандартной карты
        bool LoadMap(size_t number);
        /// @brief Функция для добавления машины на карту
        void SpawnCar();
        /// @brief Функция для удаления аварий
        void RemoveAccidents();
        /// @brief Функция для удаления всего транспорта
        void RemoveVehicles();
        /// @brief Функция для продолжения выполнения обновлений
        void Play();
        /// @brief Функция для временной остановки обновлений
        void Pause();
        /// @brief Функция для перезагрузки карты
        void Reset();

    private:
        /// @brief Сцена, к которой привязан контроллер
        WorldScene* scene_;
        /// @brief Основной слой сцены (на нём вся движуха)
        cocos2d::Layer* mainLayer_;
        /// @brief Происходят ли обновления (точнее стоит ли пауза)
        bool isPause_;
        /// @brief Размер скрытой зоны
        uint8_t hiddenArea_;
        /// @brief Количество колонок (включая скрытую зону)
        uint16_t columnsCount_;
        /// @brief Количество строк (включая скрытую зону)
        uint16_t rowsCount_;
        /// @brief Массив точек генерации транспорта
        SpawnVector spawns_;
        /// @brief Последняя разница между обновлениями
        float deltaTime_;
        /// @brief Время, прошеднее с последней автоматического генерации транспорта
        float spawnTime_;
        /// @brief Время, прошеднее с последнего автоматического удаления аварий
        float cleanTime_;
        /// @brief Коэффициент ускорения времени
        float timeFactor_;
        /// @brief Последняя загруженная карта (путь к файлу)
        std::string lastMapFile_;

        /// @brief Матрица покрытий
        CoatingUnionMatrix coatingUnions_;
        /// @brief Матрица объединений покрытий
        ControlUnitVector controlUnits_;
        /// @brief Матрица статических объектов
        StaticMatrix staticObjects_;
        /// @brief Массив движущихся объектов
        DynamicVector dynamicObjects_;

        /// @brief Функция проверяет доступность зоны для генерации статических объектов и объектов объекта
        /// @param column левая колонка проверяемой зоны
        /// @param row нижняя строка проверяемой зоны
        /// @param width ширина проверяемой зоны
        /// @param height высота проверяемой зоны
        /// @return true, если можно сгенерировать объект в данной зоне, иначе false
        bool IsEmpty_(int column, int row, size_t width = 1, size_t height = 1);

        /// @brief Функция для парсинга параметров и генерации объектов
        /// @param params массив параметров генерации
        /// @param count количество параметров генерации в массиве
        /// @return true, если объект получилось сгенерировать, иначе false
        bool GenerateObject_(uint8_t* params, uint8_t count);
        
        /// @brief Функция для генерации объединения покрытий
        /// @param column левая колонка объединения покрытий
        /// @param row нижняя строка объединения покрытий
        /// @param coatingUnion умный указатель на объединение дорог
        /// @return true, если объект получилось сгенерировать, иначе false
        bool AddCoatingUnion_(int column, int row, CoatingUnionShared coatingUnion);
        /// @brief Функция для генерации прямой односторонней дороги
        /// @param type тип объекта (асфальт, грязь...)
        /// @param column левая колонка объекта дороги
        /// @param row нижняя строка объекта дороги
        /// @param width ширина объекта дороги
        /// @param height высота объекта дороги
        /// @param angle направление, в котором разрешенно движение
        /// @return true, если объект получилось сгенерировать, иначе false
        bool AddDriveway_(CoatingType type, int column, int row, size_t width, size_t height, AngleType angle);
        /// @brief Функция для генерации перекрестка
        /// @param type тип объекта (асфальт, грязь...)
        /// @param column левая колонка перекрестка
        /// @param row нижняя строка перекрестка
        /// @param linesCounts количество полос в каждом направлении
        /// @param controlUnitType тип управляющего модуля перекрестком (тип светофора)
        /// @return true, если объект получилось сгенерировать, иначе false
        bool AddCrossroad_(CoatingType type, int column, int row, LinesCounts linesCounts,
            size_t controlUnitType = 0);
        /// @brief Функция для генерации т-образного перекрестка
        /// @param type тип объекта (асфальт, грязь...)
        /// @param column левая колонка перекрестка
        /// @param row нижняя строка перекрестка
        /// @param linesCounts количество полос в каждом направлении
        /// @param nullDirection сторона, в направлении которой нельзя двигаться
        /// @param controlUnitType тип управляющего модуля перекрестком (тип светофора)
        /// @return true, если объект получилось сгенерировать, иначе false
        bool AddTCrossroad_(CoatingType type, int column, int row, LinesCounts linesCounts, AngleType nullDirection,
            size_t controlUnitType = 0);
        /// @brief Функция для генерации левого поворота
        /// @param type тип объекта (асфальт, грязь...)
        /// @param column левая колонка поворота
        /// @param row нижняя строка поворота
        /// @param linesCount количество полос
        /// @param angle угол поворота (самого поворота)
        /// @return true, если объект получилось сгенерировать, иначе false
        bool AddLeftTurt_(CoatingType type, int column, int row, size_t linesCount, AngleType angle);
        /// @brief Функция для генерации правого поворота
        /// @param type тип объекта (асфальт, грязь...)
        /// @param column левая колонка поворота
        /// @param row нижняя строка поворота
        /// @param linesCount количество полос
        /// @param angle угол поворота (самого поворота)
        /// @return true, если объект получилось сгенерировать, иначе false
        bool AddRightTurt_(CoatingType type, int column, int row, size_t linesCount, AngleType angle);
        
        /// @brief Функция для добавления управляющего блока в общий массив (для обновлений)
        /// @param controlUnit умный указатель на управляющий блок
        /// @return true, если объект получилось добавить, иначе false
        bool AddControlUnit_(ControlUnitShared controlUnit);

        /// @brief Функция для генерации статического объекта
        /// @param column колонка статического объекта
        /// @param row строка статического объекта
        /// @param staticObject умный указатель на статический объект
        /// @return true, если объект получилось сгенерировать, иначе false
        bool AddStaticObject_(int column, int row, StaticShared staticObject);
        /// @brief Функция для генерации строения
        /// @param type тип строения
        /// @param column колонка строения
        /// @param row строка строения
        /// @param angle угол поворота строения
        /// @return true, если объект получилось сгенерировать, иначе false
        bool AddBuilding_(size_t type, int column, int row, float angle);
        /// @brief Функция для генерации куста
        /// @param type тип куста
        /// @param column колонка куста
        /// @param row строка куста
        /// @param angle угол поворота куста
        /// @return true, если объект получилось сгенерировать, иначе false
        bool AddBush_(size_t type, int column, int row, float angle);

        /// @brief Функция для генерации динамического объекта
        /// @param column колонка динамического объекта
        /// @param row строка динамического объекта
        /// @param dynamicObject умный указатель на динамический объект
        /// @return true, если объект получилось сгенерировать, иначе false
        bool AddDynamicObject_(int column, int row, DynamicShared dynamicObject);
        /// @brief Функция для генерации машины
        /// @param type тип машины
        /// @param column колонка машины
        /// @param row строка машины
        /// @param angle угол поворота машины
        /// @return true, если объект получилось сгенерировать, иначе false
        bool AddCar_(size_t type, int column, int row, float angle);

        /// @brief Функция для получения столбца в массиве
        /// @param column столбец объекта
        /// @return столбец в массиве
        inline size_t GetVectorColumn_(int column);
        /// @brief Функция для получения строки в массиве
        /// @param row строка объекта
        /// @return строка в массиве
        inline size_t GetVectorRow_(int row);
        /// @brief Функция для получения столбца объекта
        /// @param column столбец в массиве
        /// @return столбец объекта
        inline int GetRealColumn_(size_t column);
        /// @brief Функция для получения строки объекта
        /// @param row строка в массиве
        /// @return строка объекта
        inline int GetRealRow_(size_t row);

        /// @brief Функция закрытии карты
        void CloseMap_();
        /// @brief Функция для очистки массива точек генерации
        void ClearSpawns_();
        /// @brief Функция для очистки матрицы покрытий
        void ClearCoatingObjects_();
        /// @brief Функция для очистки движущихся объектов
        void ClearControlUnits_();
        /// @brief Функция для очистки статических объектов
        void ClearStaticObjects_();
        /// @brief Функция для очистки движущихся объектов
        void ClearDynamicObjects_();
    };
}

#endif // __MAP_CONTROLLER_INCLUDED__