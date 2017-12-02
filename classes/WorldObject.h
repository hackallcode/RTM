#ifndef __WORLD_OBJECT_INCLUDED__
#define __WORLD_OBJECT_INCLUDED__

#include "General.h"

namespace rtm {
    /// Класс объекта мира (родитель всех условно объемных объектов)
    class WorldObject 
    {
    public:
        /// Конструктор по умолчанию
        WorldObject();
        /// Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param x абсцисса
        /// @param y ордината
        /// @param angle угол поворота объекта
        WorldObject(cocos2d::Sprite* const sprite, float x, float y, float angle);
        /// Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param x абсцисса
        /// @param y ордината
        /// @param angle угол поворота объекта
        WorldObject(std::string const& filename, float x, float y, float angle);

        /// Деструктор по умолчанию
        virtual ~WorldObject() = default;

        /// Функция для получения спрайта
        /// @return указатель на спрайт
        cocos2d::Sprite* GetSprite() const;
        /// Функция для получения абсциссы
        /// @return абсцисса
        float GetX_() const;
        /// Функция для получения ординаты
        /// @return ордината
        float GetY_() const;
        /// Функция для получения угла поворота
        /// @return угол поворота
        float GetAngle() const;
        /// Функция для получения ширины
        /// @return ширина
        float GetWidth() const;
        /// Функция для получения высоты
        /// @return высота
        float GetHeight() const;

    protected:
        /// Функция для установки спрайта
        /// @param sprite указатель на спрайт
        void SetSprite_(cocos2d::Sprite* const sprite);
        /// Функция для установки абсциссы
        /// @param x абсцисса
        void SetX_(float x);
        /// Функция для установки ординаты
        /// @param y ордината
        void SetY_(float y);
        /// Функция для установки угла поворота
        /// @param angle угол поворота
        void SetAngle_(float angle);
        /// Функция для установки ширины
        /// @param width ширина
        void SetWidth_(float width);
        /// Функция для установки высоты
        /// @param height высота
        void SetHeight_(float height);
        
        /// Функция, выполняемая во время инициализации
        virtual void PositionInit_();
        /// Функция, выполняемая во время обновления положения
        virtual void PositionUpdate_();
        /// Функция, выполняемая во время обновления абсциссы
        virtual void OnXUpdate_();
        /// Функция, выполняемая во время обновления ординаты
        virtual void OnYUpdate_();
        /// Функция, выполняемая во время обновления угла поворота
        virtual void OnAngleUpdate_();
        /// Функция, выполняемая во время обновления ширины
        virtual void OnWidthUpdate_();
        /// Функция, выполняемая во время обновления высоты
        virtual void OnHeightUpdate_();
        
    private:
        /// Указатель на спрайт
        cocos2d::Sprite* sprite_;
        /// Абсцисса
        float x_;
        /// Абсцисса для отслеживания изменений
        float prevX_;
        /// Ордината
        float y_;
        /// Ордината для отслеживания изменений
        float prevY_;
        /// Угол поворота
        float angle_;
        /// Угол поворота для отслеживания изменений
        float prevAngle_;
        /// Ширина
        float width_;
        /// Ширина для отслеживания изменений
        float prevWidth_;
        /// Высота
        float height_;
        /// Высота для отслеживания изменений
        float prevHeight_;

        /// Функция для установки абсциссы спрайта
        /// @param x абсцисса спрайта
        void SetSpriteX_(float x);
        /// Функция для установки ординаты спрайта
        /// @param y ордината спрайта
        void SetSpriteY_(float y);
        /// Функция для установки угла поворота спрайта
        /// @param angle угол поворота спрайта
        void SetSpriteAngle_(float angle);
        /// Функция для установки ширины спрайта
        /// @param width ширина спрайта
        void SetSpriteWidth_(float width);
        /// Функция для установки высоты спрайта
        /// @param height высота спрайта
        void SetSpriteHeight_(float height);
    };
}

#endif // __WORLD_OBJECT_INCLUDED__