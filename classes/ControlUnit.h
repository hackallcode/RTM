#ifndef __CONTROL_UNIT_INCLUDED__
#define __CONTROL_UNIT_INCLUDED__

#include "General.h"

namespace rtm {
    /// Класс управляющего блока (светофор)
    class ControlUnit
    {
    public:
        /// Конструктор по умолчанию
        ControlUnit();
        /// Конструктор управляющего блока перекрестком 
        /// @param type номер типа управляющего блока
        /// @param column левая колонка перекрестка
        /// @param row нижняя строка перекрестка
        /// @param linesCounts количество полос в каждом направлении
        ControlUnit(size_t type, int column, int row, LinesCounts linesCounts);
        /// Конструктор управляющего блока перекрестком 
        /// @param type номер типа управляющего блока
        /// @param column левая колонка перекрестка
        /// @param row нижняя строка перекрестка
        /// @param linesCounts количество полос в каждом направлении
        /// @param nullDirection сторона, в направлении которой нельзя двигаться
        ControlUnit(size_t type, int column, int row, LinesCounts linesCounts, AngleType nullDirection);
        
        /// Деструктор по умолчанию
        virtual ~ControlUnit() = default;

        /// Функция обновления
        /// @param world контроллер мира, в котором находится объект
        void Update(WorldController* const world);

        /// Оператор преобразования в логический тип
        /// @return true, если управляющий блок работает (меняются сигналы), иначе false
        operator bool() const;
        /// Функция для получения сигнала (статуса направления)
        /// @param from направление, в котором транспорт движется
        /// @param to направление, в котором транспорт поедет дальше
        /// @return сигнал в нужном направлении
        SignalType GetSignal(DirectionType from, DirectionType to) const;

        /// Функция для добавления спрайтов на сцену
        /// @param layer слой, на который надо добавить спрайты управляющего блока
        void ShowSprites(cocos2d::Layer* const layer);
        /// Функция для удаления спрайтов со сцены
        /// @param layer слой, с которого надо удалить спрайты управляющего блока
        void ReleaseSprites(cocos2d::Layer* const layer);

    private:
        /// Номер типа управляющего блока (задает логику)
        size_t type_;
        /// Левая колонка перекрестка
        int column_;
        /// Нижняя строка перекрестка
        int row_;
        /// Количество полос в каждом направлении
        LinesCounts linesCounts_;
        /// Сторона, в направлении которой нельзя двигаться
        AngleType nullDirection_;
        /// Все сигналы управляющего блока
        CrossroadSignals signals_;
        /// Все спрайты управляющего блока
        DirectionsSignalSprites sprites_;
        /// Время, прошедшее с последней смены сигнала
        float time_;
        /// Номер состояния (для последовательного включения сигналов разных направлений)
        size_t state_;

        /// Функция для инициализации сигналов
        void InitSignals_();
        /// Функция для отображения спрайтов в зависимости от массива сигналов
        void ResetSprites_();

        /// Функция для безопасной установки сигнала (если направление не закрыто)
        /// @param i индекс массива для исходного направления
        /// @param j индекс массива для конечного направления
        /// @param signal новый сигнал
        void UpdateSignal_(size_t i, size_t j, SignalType signal);

        /// Функция для инкремента номера состояния
        void IncState_();
        /// Функция для установки номера состояния
        /// @param state новый номера состояния
        void SetState_(size_t state);
        /// Функция для сброса номера состояния
        void ResetState_();
    };
}

#endif // __CONTROL_UNIT_INCLUDED__