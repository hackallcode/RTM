#ifndef __PUDDLE_COATING_INCLUDED__
#define __PUDDLE_COATING_INCLUDED__

#include "CoatingObject.h"

namespace rtm {
    /// Класс, описывающий лужи
    class PuddleCoating
        : public CoatingObject
    {
    public:
        /// Конструктор по умочанию
        PuddleCoating();
        /// Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param column колонка, в которой необходимо отрисовать лужу
        /// @param row строка, в которой необходимо отрисовать лужу
        /// @param angle угол поворота лужи
        /// @param resistance коэффициент сопротивления на луже
        /// @param directions доступные направления для движения
        PuddleCoating(cocos2d::Sprite* const sprite, int column, int row, AngleType angle);
        /// Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param column колонка, в которой необходимо отрисовать лужу
        /// @param row строка, в которой необходимо отрисовать лужу
        /// @param angle угол поворота лужи
        /// @param resistance коэффициент сопротивления на луже
        /// @param directions доступные направления для движения
        PuddleCoating(std::string const& filename, int column, int row, AngleType angle);
        /// Конструктор стандартного лужи
        /// @param type стандартный тип покрытия
        /// @param id номер стандартной лужи
        /// @param column колонка, в которой необходимо отрисовать лужу
        /// @param row строка, в которой необходимо отрисовать лужу
        /// @param angle угол поворота лужи
        PuddleCoating(size_t id, int column, int row, AngleType angle);

        /// Деструктор по умолчанию
        virtual ~PuddleCoating() = default;
    };  
}

#endif // __PUDDLE_COATING_INCLUDED__