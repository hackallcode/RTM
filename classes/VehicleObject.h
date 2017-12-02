#ifndef __VEHICLE_OBJECT_INCLUDED__
#define __VEHICLE_OBJECT_INCLUDED__

#include "DynamicObject.h"

namespace rtm {
    /// Класс транспорта (динамического объекта карты)
    class VehicleObject 
        : public DynamicObject
    {
    public:
        /// Конструктор по умочанию
        VehicleObject();
        /// Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param column колонка, в которой необходимо отрисовать машину
        /// @param row строка, в которой необходимо отрисовать машину
        /// @param angle угол поворота машины
        /// @param maxSpeed максимальная скорость машины
        /// @param acceleration ускорение машины
        /// @param deceleration скорость замедления машины
        VehicleObject(cocos2d::Sprite* const sprite, int column, int row,
            float angle, float maxSpeed, float acceleration, float deceleration);
        /// Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param column колонка, в которой необходимо отрисовать машину
        /// @param row строка, в которой необходимо отрисовать машину
        /// @param angle угол поворота машины
        /// @param maxSpeed максимальная скорость машины
        /// @param acceleration ускорение машины
        /// @param deceleration скорость замедления машины
        VehicleObject(std::string const& filename, int column, int row,
            float angle, float maxSpeed, float acceleration, float deceleration);

        /// Деструктор по умолчанию
        virtual ~VehicleObject() = default;

        /// Функция обновления
        /// @param world контроллер мира, в котором находится объект
        virtual void Update(WorldController* const world) override;

    protected:
        /// Функция для сообщения о необходимости начать движение
        /// @return true, если возможно начать движение, иначе false
        bool MoveForward_();
        /// Функция для сообщения о необходимости остановиться
        /// @return true, если возможно остановиться, иначе false
        bool Stop_();
        /// Функция для сообщения о необходимости повернуть
        /// @return true, если возможно повернуть, иначе false
        bool Rotate_(float angle = ANGLE_RIGHT);
        /// Функция для сообщения о необходимости перестроиться
        /// @return true, если возможно перестроиться, иначе false
        bool ChangeLine_(bool isRight = LEFT);

        /// Функция, сообщающая о движении объекта
        /// @return true, если объект движется, иначе false
        bool IsMovement_() const;
        /// Функция, сообщающая о повороте объекта
        /// @return true, если объект поворачивает, иначе false
        bool IsRotation_() const;
        /// Функция, сообщающая о перестроении объекта
        /// @return true, если объект перестраивается, иначе false
        bool IsLineChanging_() const;
        /// Функция, сообщающая о торможении объекта перед светофором и т.д.
        /// @return true, если объект тормозит, иначе false
        bool IsBraking_() const;

        /// Функция для получения максимальной скорости
        /// @return максимальная скорость
        float GetMaxSpeed_() const;
        /// Функция для получения финальной скорости (к которой объект будет стремиться)
        /// @return конечная скорость
        float GetFinalSpeed_() const;

        /// Функция для установки финальной скорости (к которой объект будет стремиться)
        /// @param speed новая скорость
        void SetFinalSpeed_(float speed);
        /// Функция для установки коэффициента торможения
        /// @param factor новый коэффициент торможения (1 - торможение с обычным ускорением)
        void SetBrakingFactor_(float factor);
        /// Функция для установки тормозого пути (объект будет пытаться остановиться за данную дистанцию)
        /// @param distance дистанция, за которую необходимо остановиться
        void StopAtDistance_(float distance);

        /// @name Зрение у машин
        /// @{
        /// Функции для просмотра окружающего мира

        /// Функция для получения следующего по ходу движения покрытия
        /// @param world контроллер мира, в котором находится объект
        /// @param delta сдвиг в клетках относительно данного объекта (1 - следующая, 2 - через одну)
        /// @return указатель на покрытие
        CoatingObject* CheckForwardCoating_(WorldController* const world, int delta = 1);
        /// Функция для получения следующего по ходу движения объединения покрытий
        /// @param world контроллер мира, в котором находится объект
        /// @param delta сдвиг в клетках относительно данного объекта (1 - следующее, 2 - через одно)
        /// @return указатель на объединение покрытий
        CoatingUnion* CheckForwardCoatingUnion_(WorldController* const world, int delta = 1);
        /// Функция для проверки области видимости спереди
        /// @param world контроллер мира, в котором находится данный объект
        /// @param radius радиус видимости
        /// @param angle угол видимости (в каждую из сторон)
        /// @param angleShift сдвиг области видимости
        /// @return указатель на объект, находящийся в области видимости
        /// @return nullptr, если нет объектов в области видимости
        DynamicObject* CheckForwardArea_(WorldController* const world, float radius, float angle, float angleShift);
        /// Функция для проверки области видимости во время движения по прямой
        /// @param world контроллер мира, в котором находится данный объект
        /// @return указатель на объект, находящийся в области видимости
        /// @return nullptr, если нет объектов в области видимости
        DynamicObject* CheckMovingArea_(WorldController* const world);
        /// Функция для проверки области видимости перед поворотом
        /// @param world контроллер мира, в котором находится данный объект
        /// @param isRight сторона, в которую совершается поворот (тип поворота)
        /// @return указатель на объект, находящийся в области видимости
        /// @return nullptr, если нет объектов в области видимости
        DynamicObject* CheckTurnArea_(WorldController* const world, bool isRight);
        /// Функция для проверки области видимости во время поворота
        /// @param world контроллер мира, в котором находится данный объект
        /// @return указатель на объект, находящийся в области видимости
        /// @return nullptr, если нет объектов в области видимости
        DynamicObject* CheckRotationArea_(WorldController* const world);
        /// Функция для проверки области видимости перед нерегулируемым перекрестком
        /// @param world контроллер мира, в котором находится данный объект
        /// @return указатель на объект, находящийся в области видимости
        /// @return nullptr, если нет объектов в области видимости
        DynamicObject* CheckCrossroadArea_(WorldController* const world);
        /// Функция для проверки области видимости перед перестроением
        /// @param world контроллер мира, в котором находится данный объект
        /// @return указатель на объект, находящийся в области видимости
        /// @return nullptr, если нет объектов в области видимости
        DynamicObject* CheckLineChangingArea_(WorldController* const world);

        /// @}

        /// @name Маневры
        /// @{
        /// Функции выполняющиеся во время маневров

        /// Функция, выполняющаяся непосредственно перед перемещением объекта
        /// @param world контроллер мира, в котором находится данный объект
        virtual void BeforeMoving_(WorldController* const world);
        /// Функция, выполняющаяся непосредственно после перемещением объекта
        /// @param world контроллер мира, в котором находится данный объект
        virtual void AfterMoving_(WorldController* const world);
        
        /// Функция, выполняющаяся перед началом движения
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (в следующий раз выполнится MovementTick)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool MovementStart_(WorldController* const world);
        /// Функция, выполняющаяся во время движения
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (в следующий раз выполнится MovementEnd)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool MovementTick_(WorldController* const world);
        /// Функция, выполняющаяся после движения (перед началом остановки)
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (движение на этом закончится)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool MovementEnd_(WorldController* const world);
        
        /// Функция, выполняющаяся перед поворотом
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (в следующий раз выполнится RotationTick)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool RotationStart_(WorldController* const world);
        /// Функция, выполняющаяся во время поворота
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (в следующий раз выполнится RotationEnd)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool RotationTick_(WorldController* const world);
        /// Функция, выполняющаяся после поворота
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (поворот на этом закончится)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool RotationEnd_(WorldController* const world);

        /// Функция, выполняющаяся перед перестроением
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (в следующий раз выполнится LineChangingTick)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool LineChangingStart(WorldController* const world);
        /// Функция, выполняющаяся во время перестроения
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (в следующий раз выполнится LineChangingEnd)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool LineChangingTick_(WorldController* const world);
        /// Функция, выполняющаяся после перестроения
        /// @param world контроллер мира, в котором находится данный объект
        /// @return true, если шаг успешно завершён (перестроение на этом закончится)
        /// @return false, если необходимо повторить этот шаг (в следующий раз выполнится опять эта функция)
        virtual bool LineChangingEnd_(WorldController* const world);

        /// @}

    private:
        /// Этап выполнения движения (стоит, движется)
        StateType isMovement_;
        /// Этап выполнения поворота
        StateType isRotation_;
        /// Этап выполнения перестроения
        StateType isLineChanging_;
        /// Максимальная скорость
        float const maxSpeed_;
        /// Ускорение
        float const acceleration_;
        /// Скорость замедления
        float const deceleration_;
        /// Финальная скорость
        float finalSpeed_;
        /// Коэффициент торможения
        float brakingFactor_;
        /// Тормозной путь
        float brakingDistance_;
        /// Угол, на который надо повернуться
        float rotationAngle_;
        /// Радиус окружности, по которой двигается объект
        float rotationRadius_;
        /// Оставшийся перпендикулярный движению сдвиг при перестроении
        float remainingOffset_;
        /// Направление перестроения (перпендикулярно движению) 
        float remainingOffsetAngle_;

        /// Функция, выполняющая различные этапы при перестроении
        /// @param world контроллер мира, в котором находится данный объект
        void LineChanging_(WorldController* const world);
        /// Функция, выполняющая различные этапы при повороте
        /// @param world контроллер мира, в котором находится данный объект
        void Rotation_(WorldController* const world);
        /// Функция, выполняющая различные этапы при движении
        /// @param world контроллер мира, в котором находится данный объект
        void Movement_(WorldController* const world);
        /// Функция, выполняющая изменение скорости в зависимости от ускорения, тормозного пути и т.д.
        /// @param world контроллер мира, в котором находится данный объект
        void SpeedChanging_(WorldController* const world);
        /// Функция, выполняющая декрементирование тормозного пути (если задан)
        /// @param world контроллер мира, в котором находится данный объект
        void SmoothBrakingCounter(WorldController* const world);
    };
}

#endif // __VEHICLE_OBJECT_INCLUDED__