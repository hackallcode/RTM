#ifndef __BUSH_OBJECT_INCLUDED__
#define __BUSH_OBJECT_INCLUDED__

#include "MapObject.h"

namespace rtm {
    /// @brief Класс, описывающий кусты
    class BushObject
        : public MapObject
    {
    public:
        /// @brief Конструктор по умочанию
        BushObject();
        /// @brief Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param column колонка, в которой необходимо отрисовать куст
        /// @param row строка, в которой необходимо отрисовать куст
        /// @param angle угол поворота куста
        BushObject(cocos2d::Sprite* const sprite, int column, int row, float angle);
        /// @brief Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param column колонка, в которой необходимо отрисовать куст
        /// @param row строка, в которой необходимо отрисовать куст
        /// @param angle угол поворота куста
        BushObject(std::string const& filename, int column, int row, float angle);
        /// @brief Конструктор стандартного куста
        /// @param type стандартный тип куста
        /// @param column колонка, в которой необходимо отрисовать куст
        /// @param row строка, в которой необходимо отрисовать куст
        /// @param angle угол поворота куста
        BushObject(size_t type, int column, int row, float angle);

        /// @brief Деструктор по умолчанию
        virtual ~BushObject() = default;
    };
}

#endif // __BUSH_OBJECT_INCLUDED__