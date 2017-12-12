#ifndef __CAR_OBJECT_INCLUDED__
#define __CAR_OBJECT_INCLUDED__

#include "VehicleObject.h"

namespace rtm {
    /// Класс, описывающий машины
    class CarObject
        : public VehicleObject
    {
    public:
        /// Конструктор по умочанию
        CarObject();
        /// Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param column колонка, в которой необходимо отрисовать машину
        /// @param row строка, в которой необходимо отрисовать машину
        /// @param angle угол поворота машины
        /// @param maxSpeed максимальная скорость машины
        /// @param acceleration ускорение машины
        CarObject(cocos2d::Sprite* const sprite, int column, int row, float angle, float maxSpeed, float acceleration);
        /// Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param column колонка, в которой необходимо отрисовать машину
        /// @param row строка, в которой необходимо отрисовать машину
        /// @param angle угол поворота машины
        /// @param maxSpeed максимальная скорость машины
        /// @param acceleration ускорение машины
        CarObject(std::string const& filename, int column, int row, float angle, float maxSpeed, float acceleration);
        /// Конструктор стандартной машины
        /// @param type стандартный тип машины
        /// @param column колонка, в которой необходимо отрисовать машину
        /// @param row строка, в которой необходимо отрисовать машину
        /// @param angle угол поворота машины
        CarObject(size_t type, int column, int row, float angle);

        /// Деструктор по умолчанию
        virtual ~CarObject() = default;

    protected:
        /// Функция, которая просто пропускает выполнение родителя
        /// @param world контроллер мира, в котором находится объект
        virtual bool MovementStart_(WorldController* const world) override;
        /// Функция для вычисления скорости
        /// @param world контроллер мира, в котором находится объект
        virtual bool MovementTick_(WorldController* const world) override;
        /// Функция обнуляет финальную скорость
        /// @param world контроллер мира, в котором находится объект
        virtual bool MovementEnd_(WorldController* const world) override;
        /// Функция, описывающая движение перед перестроением
        /// @param world контроллер мира, в котором находится объект
        virtual bool LineChangingStart(WorldController* const world) override;

    private:
        /// Рекомендованная скорость
        float recommendedSpeed_;
        /// Желаемая скорость (приоритетнее рекомендуемой)
        float desiredSpeed_;
        /// Задана ли желаемая скорость
        bool hasDesiredSpeed_;
        /// Далеко ли следующий поворот
        bool isTurnNear_;
        /// Напрвление следующего поворота
        bool isRightTurn_;
        /// Происходит ли сейчас ожидание сигнала светофора
        bool waitForSignal_;
        /// Происходит ли сейчас ожидание освобождения нерегулируемого перекрестка
        bool waitForTurn_;
        /// Желаемое направление движения (при первой возможности машина повернет)
        AngleType desiredDirection_;
                
        /// Функция для установки желаемой скорости
        void SetDesiredSpeed_(float speed);
        /// Функция для сброса желаемой скорости
        void ResetDesiredSpeed_();

        /// Функция для проверки объекта (повороты и т.д.)
        /// @param world контроллер мира, в котором находится объект
        void CheckCoatingAhead_(WorldController* const world);
        /// Функция для проверки объединения покрытий (заранее тормозим перед светофорами и т.д.)
        /// @param world контроллер мира, в котором находится объект
        void CheckCoatingUnionAhead_(WorldController* const world);
        /// Функция для проверки дороги спереди (принятие решений)
        /// @param world контроллер мира, в котором находится объект
        void CheckRoadAhead_(WorldController* const world);

        /// Функция для получения максимальной скорости стандартной машины по номеру
        /// @param id номер стандартной машины
        /// @return максимальная скорость машины
        static float GetClassMaxSpeed_(size_t id);
        /// Функция для получения ускорения стандартной машины по номеру
        /// @param id номер стандартной машины
        /// @return ускорение машины
        static float GetClassAcceleration_(size_t id);
    };
}

#endif // __CAR_OBJECT_INCLUDED__