#ifndef __WORLD_SCENE_INCLUDED__
#define __WORLD_SCENE_INCLUDED__

#include "General.h"

namespace rtm {
    
    /// Класс главной сцены, на которой всё и происходит (для отрисовки)
    class WorldScene
        : public cocos2d::Scene
    {
    public:
        /// Конструктор класса, поддерживающий RAII
        /// @return указатель на созданный объект
        static WorldScene* Create();
        
        /// Деструктор по умолчанию
        ~WorldScene() = default;

        /// Функция для инициализации полей
        /// @return true в случае успешной инициализации, иначе false
        virtual bool init() override;
        /// Функция для обновления сцены
        /// @param time время, прошедшее с момента прошлого обновления
        virtual void update(float time) override;

        /// Функция для получения основного слоя, на котором находятся объекты
        /// @return основной слой
        cocos2d::Layer* GetMainLayer() const;

        /// @name Функции для установки фона
        
        /// Функции для установки фона из файла
        /// @param filename полный путь к файлу с фоном (картинка)
        void SetBackground(std::string const& filename);
        /// Функции для установки фона по номеру
        /// @param number номер стандартного фона
        void SetBackground(size_t number);

        /// @}
    private:
        /// Основная сцена, к которой адресуются нажатия клавиш и т.д.
        static WorldScene* globalScene_;
        /// Основной слой, на котором располагаются объекты
        cocos2d::Layer* mainLayer_;
        /// Слой для фона, находится позади основного
        cocos2d::Layer* backgroundLayer_;
        /// Картинка фона
        cocos2d::Sprite* background_;
        /// Контроллер мира, привязанный к данной сцене
        WorldControllerUnique map_;

        /// Время, прошедшее с последнего нажатия клавиш перемещения по карте (стрелочек)
        float clickTime_;
        /// Сдвиг по горизонтали при просмотре
        int viewColumn_;
        /// Сдвиг по вертикали при просмотре
        int viewRow_;
        /// Состояние клавиши CTRL
        bool isCtrlPressed_;
        /// Состояние клавиши ALT
        bool isAltPressed_;
        /// Состояние клавиши "вверх"
        bool isUpArrowPressed_;
        /// Состояние клавиши "вправо"
        bool isRightArrowPressed_;
        /// Состояние клавиши "вниз"
        bool isDownArrowPressed_;
        /// Состояние клавиши "влево"
        bool isLeftArrowPressed_;

        /// Функция открытия карты
        void OpenMap_();
        /// Функция перезагрузки карты
        void Restart_();
        
        /// Функция для установки первоначальной позиции просмотра
        void SetDefaultPosition_();
        /// Функция сдвига области просмотра вверх
        void ShiftUp_();
        /// Функция сдвига области просмотра вправо
        void ShiftRight_();
        /// Функция сдвига области просмотра вниз
        void ShiftDown_();
        /// Функция сдвига области просмотра влево
        void ShiftLeft_();
        /// Функция для обновления положения главного слоя в зависимости от области просмотра
        void UpdatePosition_();

        /// Функция для установки масштаба просмотра по умолчанию
        void SetDefaultScale_();
        /// Функция для увеличения масштаба просмотра
        void IncreaseScale_();
        /// Функция для уменьшения масштаба просмотра
        void DecreaseScale_();

        /// Функция для установки скорости обработки (скорости объектов) по умолчанию
        void SetDefaultSpeed_();
        /// Функция для увеличения скорости обработки (скорости объектов)
        void IncreaseSpeed_();
        /// Функция для уменьшения скорости обработки (скорости объектов)
        void DecreaseSpeed_();

        /// Функция для получения контроллера данной сцены
        /// @return контроллер сцены
        WorldControllerUnique& GetMap_();

        /// Функция-обработчик нажатий клавиш клавиатуры
        static void KeyPressed_(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
        /// Функция-обработчик отпусканий клавиш клавиатуры
        static void KeyReleased_(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
    };
};

#endif // __WORLD_SCENE_INCLUDED__
