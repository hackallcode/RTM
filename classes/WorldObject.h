#ifndef __WORLD_OBJECT_INCLUDED__
#define __WORLD_OBJECT_INCLUDED__

#include "General.h"

namespace rtm {
    /// @brief Класс объекта мира (родитель всех условно объемных объектов)
    class WorldObject 
    {
    public:
        /// @brief Конструктор по умолчанию
        WorldObject();
        /// @brief Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param x абсцисса
        /// @param y ордината
        /// @param angle угол поворота объекта
        WorldObject(cocos2d::Sprite* const sprite, float x, float y, float angle);
        /// @brief Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param x абсцисса
        /// @param y ордината
        /// @param angle угол поворота объекта
        WorldObject(std::string const& filename, float x, float y, float angle);

        /// @brief Деструктор по умолчанию
        virtual ~WorldObject() = default;

        /// @brief Функция для получения спрайта
        /// @return указатель на спрайт
        cocos2d::Sprite* GetSprite() const;
        /// @brief Функция для получения абсциссы
        /// @return абсцисса
        float GetX_() const;
        /// @brief Функция для получения ординаты
        /// @return ордината
        float GetY_() const;
        /// @brief Функция для получения угла поворота
        /// @return угол поворота
        float GetAngle() const;
        /// @brief Функция для получения ширины
        /// @return ширина
        float GetWidth() const;
        /// @brief Функция для получения высоты
        /// @return высота
        float GetHeight() const;

    protected:
        /// @brief Функция для установки спрайта
        /// @param sprite указатель на спрайт
        void SetSprite_(cocos2d::Sprite* const sprite);
        /// @brief Функция для установки абсциссы
        /// @param x абсцисса
        void SetX_(float x);
        /// @brief Функция для установки ординаты
        /// @param y ордината
        void SetY_(float y);
        /// @brief Функция для установки угла поворота
        /// @param angle угол поворота
        void SetAngle_(float angle);
        /// @brief Функция для установки ширины
        /// @param width ширина
        void SetWidth_(float width);
        /// @brief Функция для установки высоты
        /// @param height высота
        void SetHeight_(float height);
        
        /// @brief Функция, выполняемая во время инициализации
        virtual void PositionInit_();
        /// @brief Функция, выполняемая во время обновления положения
        virtual void PositionUpdate_();
        /// @brief Функция, выполняемая во время обновления абсциссы
        virtual void OnXUpdate_();
        /// @brief Функция, выполняемая во время обновления ординаты
        virtual void OnYUpdate_();
        /// @brief Функция, выполняемая во время обновления угла поворота
        virtual void OnAngleUpdate_();
        /// @brief Функция, выполняемая во время обновления ширины
        virtual void OnWidthUpdate_();
        /// @brief Функция, выполняемая во время обновления высоты
        virtual void OnHeightUpdate_();
        
    private:
        /// @brief Указатель на спрайт
        cocos2d::Sprite* sprite_;
        /// @brief Абсцисса
        float x_;
        /// @brief Абсцисса для отслеживания изменений
        float prevX_;
        /// @brief Ордината
        float y_;
        /// @brief Ордината для отслеживания изменений
        float prevY_;
        /// @brief Угол поворота
        float angle_;
        /// @brief Угол поворота для отслеживания изменений
        float prevAngle_;
        /// @brief Ширина
        float width_;
        /// @brief Ширина для отслеживания изменений
        float prevWidth_;
        /// @brief Высота
        float height_;
        /// @brief Высота для отслеживания изменений
        float prevHeight_;

        /// @brief Функция для установки абсциссы спрайта
        /// @param x абсцисса спрайта
        void SetSpriteX_(float x);
        /// @brief Функция для установки ординаты спрайта
        /// @param y ордината спрайта
        void SetSpriteY_(float y);
        /// @brief Функция для установки угла поворота спрайта
        /// @param angle угол поворота спрайта
        void SetSpriteAngle_(float angle);
        /// @brief Функция для установки ширины спрайта
        /// @param width ширина спрайта
        void SetSpriteWidth_(float width);
        /// @brief Функция для установки высоты спрайта
        /// @param height высота спрайта
        void SetSpriteHeight_(float height);
    };
}

#endif // __WORLD_OBJECT_INCLUDED__