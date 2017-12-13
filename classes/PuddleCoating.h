#ifndef __PUDDLE_COATING_INCLUDED__
#define __PUDDLE_COATING_INCLUDED__

#include "CoatingObject.h"

namespace rtm {
    /// @brief Класс, описывающий лужи
    class PuddleCoating
        : public CoatingObject
    {
    public:
        /// @brief Конструктор по умочанию
        PuddleCoating();
        /// @brief Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param column колонка, в которой необходимо отрисовать лужу
        /// @param row строка, в которой необходимо отрисовать лужу
        /// @param angle угол поворота лужи
        PuddleCoating(cocos2d::Sprite* const sprite, int column, int row, AngleType angle);
        /// @brief Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param column колонка, в которой необходимо отрисовать лужу
        /// @param row строка, в которой необходимо отрисовать лужу
        /// @param angle угол поворота лужи
        PuddleCoating(std::string const& filename, int column, int row, AngleType angle);
        /// @brief Конструктор стандартного лужи
        /// @param id номер стандартной лужи
        /// @param column колонка, в которой необходимо отрисовать лужу
        /// @param row строка, в которой необходимо отрисовать лужу
        /// @param angle угол поворота лужи
        PuddleCoating(size_t id, int column, int row, AngleType angle);

        /// @brief Деструктор по умолчанию
        virtual ~PuddleCoating() = default;
    };  
}

#endif // __PUDDLE_COATING_INCLUDED__