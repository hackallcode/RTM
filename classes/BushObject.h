#ifndef __BUSH_OBJECT_INCLUDED__
#define __BUSH_OBJECT_INCLUDED__

#include "MapObject.h"

namespace rtm {
    /// Класс, описывающий кусты
    class BushObject
        : public MapObject
    {
    public:
        /// Конструктор по умочанию
        BushObject();
        /// Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param column колонка, в которой необходимо отрисовать куст
        /// @param row строка, в которой необходимо отрисовать куст
        /// @param angle угол поворота куста
        BushObject(cocos2d::Sprite* const sprite, int column, int row, float angle);
        /// Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param column колонка, в которой необходимо отрисовать куст
        /// @param row строка, в которой необходимо отрисовать куст
        /// @param angle угол поворота куста
        BushObject(std::string const& filename, int column, int row, float angle);
        /// Конструктор стандартного куста
        /// @param type стандартный тип куста
        /// @param column колонка, в которой необходимо отрисовать куст
        /// @param row строка, в которой необходимо отрисовать куст
        /// @param angle угол поворота куста
        BushObject(size_t type, int column, int row, float angle);

        /// Деструктор по умолчанию
        virtual ~BushObject() = default;
    };
}

#endif // __BUSH_OBJECT_INCLUDED__