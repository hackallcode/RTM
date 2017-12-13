#ifndef __COATING_OBJECTS_INCLUDED__
#define __COATING_OBJECTS_INCLUDED__

#include "General.h"

namespace rtm {
    /// @brief Класс покрытия секции карты
    class CoatingObject
    {
    public:
        /// @brief Конструктор по умолчанию
        CoatingObject();
        /// @brief Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param column колонка, в которой необходимо отрисовать объект
        /// @param row строка, в которой необходимо отрисовать объект
        /// @param angle угол поворота объекта
        /// @param resistance коэффициент сопротивления на покрытии
        /// @param directions доступные направления для движения
        CoatingObject(cocos2d::Sprite* const sprite, int column, int row,
            AngleType angle, float resistance, Directions directions);
        /// @brief Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param column колонка, в которой необходимо отрисовать объект
        /// @param row строка, в которой необходимо отрисовать объект
        /// @param angle угол поворота объекта
        /// @param resistance коэффициент сопротивления на покрытии
        /// @param directions доступные направления для движения
        CoatingObject(std::string const& filename, int column, int row,
            AngleType angle, float resistance, Directions directions);

        /// @brief Деструктор по умолчанию
        virtual ~CoatingObject() = default;

        /// @brief Функция для получения спрайта
        /// @return указатель на спрайт
        cocos2d::Sprite* GetSprite() const;
        /// @brief Функция для получения коэффициента сопротивления на покрытии
        /// @return сопротивление
        float GetResistance() const;

        /// @brief Функция для проверки существования направления
        /// @param angle направление
        /// @return true, если доступно (существует), иначе false
        bool HasDirection(AngleType angle) const;
        /// @brief Функция для проверки разрешенности направления
        /// @param angle направление
        /// @return true, если разрешено ехать в данном направлении, иначе false
        bool IsDirectionAvailable(AngleType angle) const;
        /// @brief Функция установки разрешенности направления
        /// @param angle направление
        /// @param status разрешено ли ехать
        void SetDirectionAvailability(AngleType angle, bool status);

    protected:
        /// @brief Функция для установки спрайта
        /// @param sprite указатель на спрайт
        void SetSprite_(cocos2d::Sprite* const sprite);

    private:
        /// @brief Указатель на спрайт
        cocos2d::Sprite* sprite_;
        /// @brief Абсцисса
        float x_;
        /// @brief Ордината
        float y_;
        /// @brief Сопротивление на покрытии
        float resistance_;
        /// @brief Доступные направления
        Directions directions_;
        /// @brief Разрешенные направления
        Directions availableDirections_;

        /// @brief Функция для установки абсциссы
        /// @param x абсцисса
        void SetX_(float x);
        /// @brief Функция для установки ординаты
        /// @param y ордината
        void SetY_(float y);
    };
}

#endif // __COATING_OBJECTS_INCLUDED__