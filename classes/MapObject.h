#ifndef __MAP_OBJECT_INCLUDED__
#define __MAP_OBJECT_INCLUDED__

#include "StaticObject.h"

namespace rtm {
    /// @brief Класс статического объекта карты
    class MapObject
        : public StaticObject
    {
    public:
        /// @brief Конструктор по умочанию
        MapObject();
        /// @brief Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param column колонка, в которой необходимо отрисовать объект
        /// @param row строка, в которой необходимо отрисовать объект
        /// @param angle угол поворота объекта
        MapObject(cocos2d::Sprite* sprite, int column, int row, float angle);
        /// @brief Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param column колонка, в которой необходимо отрисовать объект
        /// @param row строка, в которой необходимо отрисовать объект
        /// @param angle угол поворота объекта
        MapObject(std::string const& filename, int column, int row, float angle);

        /// @brief Деструктор по умолчанию
        virtual ~MapObject() = default;
    };
}

#endif // __MAP_OBJECT_INCLUDED__