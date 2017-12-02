﻿#ifndef __STATIC_OBJECT_INCLUDED__
#define __STATIC_OBJECT_INCLUDED__

#include "WorldObject.h"

namespace rtm {
    /// Класс статического объекта (который не обновляется)
    class StaticObject 
        : public WorldObject
    {
    public:
        /// Конструктор по умочанию
        StaticObject();
        /// Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param x абсцисса будущего объекта
        /// @param y ордината будущего объекта
        /// @param angle угол поворота строения
        StaticObject(cocos2d::Sprite* sprite, float x, float y, float angle);
        /// Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param x абсцисса будущего объекта
        /// @param y ордината будущего объекта
        /// @param angle угол поворота строения
        StaticObject(std::string const& filename, float x, float y, float angle);

        /// Деструктор по умолчанию
        virtual ~StaticObject() = default;
    };
}

#endif // __STATIC_OBJECT_INCLUDED__