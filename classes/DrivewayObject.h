#ifndef __DRIVEWAY_OBJECTS_INCLUDED__
#define __DRIVEWAY_OBJECTS_INCLUDED__

#include "CoatingUnion.h"

namespace rtm {
    /// @brief Класс прямой дороги
    class DrivewayObject
        : public CoatingUnion
    {
    public:
        /// @brief Конструктор по умолчанию
        DrivewayObject();
        /// @brief Конструктор по размерам
        /// @param type тип покрытия
        /// @param column левая колонка объекта
        /// @param row нижняя строка объекта
        /// @param width ширина объекта
        /// @param height высота объекта
        /// @param angle направление движения
        DrivewayObject(CoatingType type, int column, int row, size_t width, size_t height, AngleType angle);
        /// @brief Функция для получения матрицы покрытий дороги
        /// @param type тип покрытия
        /// @param column левая колонка объекта
        /// @param row нижняя строка объекта
        /// @param width ширина объекта
        /// @param height высота объекта
        /// @param angle направление движения
        /// @return матрица покрытий
        static CoatingMatrix DrivewayMatrix(CoatingType type, int column, int row, size_t width, size_t height, AngleType angle);

        /// @brief Деструктор по умолчанию
        virtual ~DrivewayObject() = default;

        /// @brief Функция для получения длины объекта (для вычисления кратчайшего пути)
        /// @return длина
        virtual float GetLength() const override;
        /// @brief Функция для получения количества полос
        /// @return количество полос
        size_t GetLinesCount() const;

        /// @brief Функция для проверки: находится ли объект в правой полосе
        /// @param column колонка, в которой находится объект
        /// @param row строка, в которой находится объект
        /// @return true, если объект находится в правой полосе, иначе false
        bool isRightLine(int column, int row) const;
        /// @brief Функция для проверки: находится ли объект в правой полосе
        /// @param x абсцисса объекта
        /// @param y ордината объекта
        /// @return true, если объект находится в правой полосе, иначе false
        bool isRightLine(float x, float y) const;
        /// @brief Функция для проверки: находится ли объект в левой полосе
        /// @param column колонка, в которой находится объект
        /// @param row строка, в которой находится объект
        /// @return true, если объект находится в левой полосе, иначе false
        bool isLeftLine(int column, int row) const;
        /// @brief Функция для проверки: находится ли объект в левой полосе
        /// @param x абсцисса объекта
        /// @param y ордината объекта
        /// @return true, если объект находится в левой полосе, иначе false
        bool isLeftLine(float x, float y) const;

    private:
        /// @brief Направление движения
        AngleType angle_;
        /// @brief Длина объекта (для вычисления кратчайшего пути)
        float length_;
        /// @brief Количество полос
        size_t linesCount_;

        /// @brief Функция для вычисления длины объекта
        /// @param width ширина объекта
        /// @param height высота объекта
        /// @param angle направление движения
        /// @return длина объекта
        static float CountLength_(size_t width, size_t height, AngleType angle);
        /// @brief Функция для получения количества полос
        /// @param width ширина объекта
        /// @param height высота объекта
        /// @param angle направление движения
        /// @return количество полос
        static size_t CountLines_(size_t width, size_t height, AngleType angle);
    };
}

#endif // __DRIVEWAY_OBJECTS_INCLUDED__