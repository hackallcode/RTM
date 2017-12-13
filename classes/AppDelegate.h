#ifndef  __APP_DELEGATE_INCLUDED__
#define  __APP_DELEGATE_INCLUDED__

#include "cocos2d.h"

/// @brief Приложение, основанное на Cocos2d
class  AppDelegate : private cocos2d::Application
{
public:
    /// @brief Конструктор по умолчанию
    AppDelegate();
    /// @brief Деструктор
    virtual ~AppDelegate();

    /// @brief Функция для установки атрибутов OpenGL (красный, зеленый, синий, альфа-канал...)
    virtual void initGLContextAttrs();

    /// @brief Функция для инициализации Director'а и Scene'ы
    /// @return true    Инициализация успешна, приложение продолжает выполняться
    /// @return false   Инициализация провалилась, приложение закроется
    virtual bool applicationDidFinishLaunching();

    /// @brief Функция вызывается, когда приложение скрывается
    virtual void applicationDidEnterBackground();

    /// @brief Функция вызывается при первом запуске приложения
    virtual void applicationWillEnterForeground();
};

#endif // __APP_DELEGATE_INCLUDED__

