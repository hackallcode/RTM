#ifndef __ROAD_COATING_INCLUDED__
#define __ROAD_COATING_INCLUDED__

#include "CoatingObject.h"

namespace rtm {
    /// Класс, описывающий дороги
    class RoadCoating
        : public CoatingObject
    {
    public:
        /// Конструктор по умочанию
        RoadCoating();
        /// Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param column колонка, в которой необходимо отрисовать дорогу
        /// @param row строка, в которой необходимо отрисовать дорогу
        /// @param angle угол поворота дороги
        /// @param resistance коэффициент сопротивления на дороге
        /// @param directions доступные направления для движения
        RoadCoating(cocos2d::Sprite* const sprite, int column, int row,
            AngleType angle, float resistance, Directions directions);
        /// Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param column колонка, в которой необходимо отрисовать дорогу
        /// @param row строка, в которой необходимо отрисовать дорогу
        /// @param angle угол поворота дороги
        /// @param resistance коэффициент сопротивления на дороге
        /// @param directions доступные направления для движения
        RoadCoating(std::string const& filename, int column, int row,
            AngleType angle, float resistance, Directions directions);
        /// Конструктор стандартного дороги
        /// @param type стандартный тип покрытия
        /// @param id номер стандартной дороги
        /// @param column колонка, в которой необходимо отрисовать дорогу
        /// @param row строка, в которой необходимо отрисовать дорогу
        /// @param angle угол поворота дороги
        RoadCoating(CoatingType type, size_t id, int column, int row, AngleType angle);

        /// Деструктор по умолчанию
        virtual ~RoadCoating() = default;

    private:
        /// Функция для получения коэффициента сопротивления на стандартной дороге по номеру
        /// @param type тип покрытия
        /// @return сопротивление
        static float GetClassResistance_(CoatingType type);
        /// Функция для получения доступных направлений стандартной дороги по номеру
        /// @param id номер стандартной дороги
        /// @return доступные направления
        static Directions const& GetClassDirections_(size_t id);
    };  
}

#endif // __ROAD_COATING_INCLUDED__