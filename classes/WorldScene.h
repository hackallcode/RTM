#ifndef __WORLD_SCENE_INCLUDED__
#define __WORLD_SCENE_INCLUDED__

#include "General.h"

namespace rtm {
    
    /// @brief Класс главной сцены, на которой всё и происходит (для отрисовки)
    class WorldScene
        : public cocos2d::Scene
    {
    public:
        /// @brief Конструктор класса, поддерживающий RAII
        /// @return указатель на созданный объект
        static WorldScene* Create();
        
        /// @brief Деструктор по умолчанию
        ~WorldScene() = default;

        /// @brief Функция для инициализации полей
        /// @return true в случае успешной инициализации, иначе false
        virtual bool init() override;
        /// @brief Функция для обновления сцены
        /// @param time время, прошедшее с момента прошлого обновления
        virtual void update(float time) override;

        /// @brief Функция для получения основного слоя, на котором находятся объекты
        /// @return основной слой
        cocos2d::Layer* GetMainLayer() const;

        /// @name Функции для установки фона
        
        /// @brief Функции для установки фона из файла
        /// @param filename полный путь к файлу с фоном (картинка)
        void SetBackground(std::string const& filename);
        /// @brief Функции для установки фона по номеру
        /// @param number номер стандартного фона
        void SetBackground(size_t number);

        /// @}
    private:
        /// @brief Основная сцена, к которой адресуются нажатия клавиш и т.д.
        static WorldScene* globalScene_;
        /// @brief Основной слой, на котором располагаются объекты
        cocos2d::Layer* mainLayer_;
        /// @brief Слой для фона, находится позади основного
        cocos2d::Layer* backgroundLayer_;
        /// @brief Картинка фона
        cocos2d::Sprite* background_;
        /// @brief Контроллер мира, привязанный к данной сцене
        WorldControllerUnique map_;

        /// @brief Время, прошедшее с последнего нажатия клавиш перемещения по карте (стрелочек)
        float clickTime_;
        /// @brief Сдвиг по горизонтали при просмотре
        int viewColumn_;
        /// @brief Сдвиг по вертикали при просмотре
        int viewRow_;
        /// @brief Состояние клавиши CTRL
        bool isCtrlPressed_;
        /// @brief Состояние клавиши ALT
        bool isAltPressed_;
        /// @brief Состояние клавиши "вверх"
        bool isUpArrowPressed_;
        /// @brief Состояние клавиши "вправо"
        bool isRightArrowPressed_;
        /// @brief Состояние клавиши "вниз"
        bool isDownArrowPressed_;
        /// @brief Состояние клавиши "влево"
        bool isLeftArrowPressed_;

        /// @brief Функция открытия карты
        void OpenMap_();
        /// @brief Функция перезагрузки карты
        void Restart_();
        
        /// @brief Функция для установки первоначальной позиции просмотра
        void SetDefaultPosition_();
        /// @brief Функция сдвига области просмотра вверх
        void ShiftUp_();
        /// @brief Функция сдвига области просмотра вправо
        void ShiftRight_();
        /// @brief Функция сдвига области просмотра вниз
        void ShiftDown_();
        /// @brief Функция сдвига области просмотра влево
        void ShiftLeft_();
        /// @brief Функция для обновления положения главного слоя в зависимости от области просмотра
        void UpdatePosition_();

        /// @brief Функция для установки масштаба просмотра по умолчанию
        void SetDefaultScale_();
        /// @brief Функция для увеличения масштаба просмотра
        void IncreaseScale_();
        /// @brief Функция для уменьшения масштаба просмотра
        void DecreaseScale_();

        /// @brief Функция для установки скорости обработки (скорости объектов) по умолчанию
        void SetDefaultSpeed_();
        /// @brief Функция для увеличения скорости обработки (скорости объектов)
        void IncreaseSpeed_();
        /// @brief Функция для уменьшения скорости обработки (скорости объектов)
        void DecreaseSpeed_();

        /// @brief Функция для получения контроллера данной сцены
        /// @return контроллер сцены
        WorldControllerUnique& GetMap_();

        /// @brief Функция-обработчик нажатий клавиш клавиатуры
        static void KeyPressed_(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
        /// @brief Функция-обработчик отпусканий клавиш клавиатуры
        static void KeyReleased_(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
    };
};

#endif // __WORLD_SCENE_INCLUDED__
