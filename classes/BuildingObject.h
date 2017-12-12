#ifndef __BUILDING_OBJECT_INCLUDED__
#define __BUILDING_OBJECT_INCLUDED__

#include "MapObject.h"

namespace rtm {
    /// Класс, описывающий строения (здания)
    class BuildingObject
        : public MapObject
    {
    public:
        /// Конструктор по умочанию
        BuildingObject();
        /// Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param column колонка, в которой необходимо отрисовать строение
        /// @param row строка, в которой необходимо отрисовать строение
        /// @param angle угол поворота строения
        BuildingObject(cocos2d::Sprite* const sprite, int column, int row, float angle);
        /// Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param column колонка, в которой необходимо отрисовать строение
        /// @param row строка, в которой необходимо отрисовать строение
        /// @param angle угол поворота строения
        BuildingObject(std::string const& filename, int column, int row, float angle);
        /// Конструктор стандартного строения
        /// @param type стандартный тип строения
        /// @param column колонка, в которой необходимо отрисовать строение
        /// @param row строка, в которой необходимо отрисовать строение
        /// @param angle угол поворота строения
        BuildingObject(size_t type, int column, int row, float angle);

        /// Деструктор по умолчанию
        virtual ~BuildingObject() = default;
    };
}

#endif // __BUILDING_OBJECT_INCLUDED__