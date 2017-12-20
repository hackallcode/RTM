#ifndef __VEHICLE_OBJECT_INCLUDED__
#define __VEHICLE_OBJECT_INCLUDED__

#include "DynamicObject.h"

namespace rtm {
    /// @brief Класс транспорта (динамического объекта карты)
    class VehicleObject 
        : public DynamicObject
    {
    public:
        /// @brief Конструктор по умочанию
        VehicleObject();
        /// @brief Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param column колонка, в которой необходимо отрисовать машину
        /// @param row строка, в которой необходимо отрисовать машину
        /// @param angle угол поворота машины
        /// @param maxSpeed максимальная скорость машины
        /// @param acceleration ускорение машины
        /// @param deceleration скорость замедления машины
        VehicleObject(cocos2d::Sprite* const sprite, int column, int row,
            float angle, float maxSpeed, float acceleration, float deceleration);
        /// @brief Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param column колонка, в которой необходимо отрисовать машину
        /// @param row строка, в которой необходимо отрисовать машину
        /// @param angle угол поворота машины
        /// @param maxSpeed максимальная скорость машины
        /// @param acceleration ускорение машины
        /// @param deceleration скорость замедления машины
        VehicleObject(std::string const& filename, int column, int row,
            float angle, float maxSpeed, float acceleration, float deceleration);

        /// @brief Деструктор по умолчанию
        virtual ~VehicleObject() = default;

        /// @brief Функция обновления
        /// @param world контроллер мира, в котором находится объект
        virtual void Update(WorldController* const world) override;

    protected:
        /// @brief Функция для сообщения о необходимости начать движение
        /// @return true, если возможно начать движение, иначе false
        bool MoveForward_();
        /// @brief Функция для сообщения о необходимости остановиться
        /// @return true, если возможно остановиться, иначе false
        bool Stop_();
        /// @brief Функция для сообщения о необходимости повернуть
        /// @param angle угол, на который необходимо повернуть
        /// @return true, если возможно повернуть, иначе false (если поворот уже начат)
        bool Rotate_(float angle = ANGLE_RIGHT);
        /// @brief Функция для сообщения о необходимости перестроиться
        /// @param isRight сторона, в которую надо перестроиться (можно использовать константы rtm::LEFT и rtm::RIGHT)
        /// @return true, если возможно перестроиться, иначе false
        bool ChangeLine_(bool isRight = LEFT);

        /// @brief Функция, сообщающая о движении объекта
        /// @return true, если объект движется, иначе false
        bool IsMovement_() const;
        /// @brief Функция, сообщающая о повороте объекта
        /// @return true, если объект поворачивает, иначе false
        bool IsRotation_() const;
        /// @brief Функция, сообщающая о перестроении объекта
        /// @return true, если объект перестраивается, иначе false
        bool IsLineChanging_() const;
        /// @brief Функция, сообщающая о торможении объекта перед светофором и т.д.
        /// @return true, если объект тормозит, иначе false
        bool IsBraking_() const;

        /// @brief Функция для получения максимальной скорости
        /// @return максимальная скорость
        float GetMaxSpeed_() const;
        /// @brief Функция для получения финальной скорости (к которой объект будет стремиться)
        /// @return конечная скорость
        float GetFinalSpeed_() const;

        /// @brief Функция для установки финальной скорости (к которой объект будет стремиться)
        /// @param speed новая скорость
        void SetFinalSpeed_(float speed);
        /// @brief Функция для установки коэффициента торможения
        /// @param factor новый коэффициент торможения (1 - торможение с обычным ускорением)
        void SetBrakingFactor_(float factor);
        /// @brief Функция для установки тормозого пути (объект будет пытаться остановиться за данную дистанцию)
        /// @param distance дистанция, за которую необходимо остановиться
        void StopAtDistance_(float distance);

        /// @name Зрение у машин
        /// @{
        /// @brief Функции для просмотра окружающего мира

        /// @brief Функция для получения следующего по ходу движения покрытия
        /// @param world контроллер мира, в котором находится объект
        /// @param delta сдвиг в клетках относительно данного объекта (1 - следующая, 2 - через одну)
        /// @return указатель на покрытие
        CoatingObject* CheckForwardCoating_(WorldController* const world, int delta = 1);
        /// @brief Функция для получения следующего по ходу движения объединения покрытий
        /// @param world контроллер мира, в котором находится объект
        /// @param delta сдвиг в клетках относительно данного объекта (1 - следующее, 2 - через одно)
        /// @return указатель на объединение покрытий
        CoatingUnion* CheckForwardCoatingUnion_(WorldController* const world, int delta = 1);
        /// @brief Функция для проверки области видимости спереди
        /// @param world контроллер мира, в котором находится данный объект
        /// @param radius радиус видимости
        /// @param angle угол видимости (в каждую из сторон)
        /// @param angleShift сдвиг области видимости
        /// @return указатель на объект, находящийся в области видимости
        /// @return nullptr, если нет объектов в области видимости
        DynamicObject* CheckForwardArea_(WorldController* const world, float radius, float angle, float angleShift);
        /// @brief Функция для проверки области видимости во время движения по прямой
        /// @param world контроллер мира, в котором находится данный объект
        /// @return указатель на объект, находящийся в области видимости
        /// @return nullptr, если нет объектов в области видимости
        DynamicObject* CheckMovingArea_(WorldController* const world);
        /// @brief Функция для проверки области видимости перед поворотом
        /// @param world контроллер мира, в котором находится данный объект
        /// @param isRight сторона, в которую совершается поворот (тип поворота)
        /// @return указатель на объект, находящийся в области видимости
        /// @return nullptr, если нет объектов в области видимости
        DynamicObject* CheckTurnArea_(WorldController* const world, bool isRight);
        /// @brief Функция для проверки области видимости во время поворота
        /// @param world контроллер мира, в котором находится данный объект
        /// @return указатель на объект, находящийся в области видимости
        /// @return nullptr, если нет объектов в области видимости
        DynamicObject* CheckRotationArea_(WorldController* const world);
        /// @brief Функция для проверки области видимости перед нерегулируемым перекрестком
        /// @param world контроллер мира, в котором находится данный объект
        /// @return указатель на объект, находящийся в области видимости
        /// @return nullptr, если нет объектов в области видимости
        DynamicObject* CheckCrossroadArea_(WorldController* const world);
        /// @brief Функция для проверки области видимости перед перестроением
        /// @param world контроллер мира, в котором находится данный объект
        /// @return указатель на объект, находящийся в области видимости
        /// @return nullptr, если нет объектов в области видимости
        DynamicObject* CheckLineChangingArea_(WorldController* const world);

        /// @}

        /// @name Маневры
        /// @{
        /// @brief Функции выполняющиеся во время маневров

        /// @brief Функция, выполняющаяся непосредственно перед перемещением объекта
        /// @param world контроллер мира, в котором находится данный объект
        virtual void BeforeMoving_(WorldController* const world);
        /// @brief Функция, выполняющаяся непосредственно после перемещением объекта
        /// @param world контроллер мира, в котором находится данный объект
        virtual void AfterMoving_(WorldController* const world);
        
        /// @brief Функция, выполняющаяся перед началом движения
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (в следующий раз выполнится MovementTick)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool MovementStart_(WorldController* const world);
        /// @brief Функция, выполняющаяся во время движения
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (в следующий раз выполнится MovementEnd)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool MovementTick_(WorldController* const world);
        /// @brief Функция, выполняющаяся после движения (перед началом остановки)
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (движение на этом закончится)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool MovementEnd_(WorldController* const world);
        
        /// @brief Функция, выполняющаяся перед поворотом
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (в следующий раз выполнится RotationTick)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool RotationStart_(WorldController* const world);
        /// @brief Функция, выполняющаяся во время поворота
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (в следующий раз выполнится RotationEnd)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool RotationTick_(WorldController* const world);
        /// @brief Функция, выполняющаяся после поворота
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (поворот на этом закончится)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool RotationEnd_(WorldController* const world);

        /// @brief Функция, выполняющаяся перед перестроением
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (в следующий раз выполнится LineChangingTick)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool LineChangingStart(WorldController* const world);
        /// @brief Функция, выполняющаяся во время перестроения
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (в следующий раз выполнится LineChangingEnd)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool LineChangingTick_(WorldController* const world);
        /// @brief Функция, выполняющаяся после перестроения
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (перестроение на этом закончится)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool LineChangingEnd_(WorldController* const world);

        /// @}

    private:
        /// @brief Этап выполнения движения (стоит, движется)
        StateType isMovement_;
        /// @brief Этап выполнения поворота
        StateType isRotation_;
        /// @brief Этап выполнения перестроения
        StateType isLineChanging_;
        /// @brief Максимальная скорость
        float const maxSpeed_;
        /// @brief Ускорение
        float const acceleration_;
        /// @brief Скорость замедления
        float const deceleration_;
        /// @brief Финальная скорость
        float finalSpeed_;
        /// @brief Коэффициент торможения
        float brakingFactor_;
        /// @brief Тормозной путь
        float brakingDistance_;
        /// @brief Угол, на который надо повернуться
        float rotationAngle_;
        /// @brief Радиус окружности, по которой двигается объект
        float rotationRadius_;
        /// @brief Оставшийся перпендикулярный движению сдвиг при перестроении
        float remainingOffset_;
        /// @brief Направление перестроения (перпендикулярно движению) 
        float remainingOffsetAngle_;

        /// @brief Функция, выполняющая различные этапы при перестроении
        /// @param world контроллер мира, в котором находится данный объект
        void LineChanging_(WorldController* const world);
        /// @brief Функция, выполняющая различные этапы при повороте
        /// @param world контроллер мира, в котором находится данный объект
        void Rotation_(WorldController* const world);
        /// @brief Функция, выполняющая различные этапы при движении
        /// @param world контроллер мира, в котором находится данный объект
        void Movement_(WorldController* const world);
        /// @brief Функция, выполняющая изменение скорости в зависимости от ускорения, тормозного пути и т.д.
        /// @param world контроллер мира, в котором находится данный объект
        void SpeedChanging_(WorldController* const world);
        /// @brief Функция, выполняющая декрементирование тормозного пути (если задан)
        /// @param world контроллер мира, в котором находится данный объект
        void SmoothBrakingCounter(WorldController* const world);
    };
}

#endif // __VEHICLE_OBJECT_INCLUDED__