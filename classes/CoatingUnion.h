#ifndef __COATING_UNION_INCLUDED__
#define __COATING_UNION_INCLUDED__

#include "General.h"

namespace rtm {
    /// Класс объединения покрытий
    /// @see CoatingObject
    class CoatingUnion 
    {
    public:
        /// Конструктор по умолчанию
        CoatingUnion();
        /// Конструктор по матрице покрытий
        /// @param type тип объединения покрытий (получившегося элемента)
        /// @param column левая колонка объединения
        /// @param row нижняя строка объединения
        /// @param objects матрица объектов покрытий
        CoatingUnion(CoatingUnionType type, int column, int row, CoatingMatrix&& objects);

        /// Деструктор по умолчанию
        virtual ~CoatingUnion() = default;

        /// Функция для получения типа объединения
        /// @return тип объединения
        CoatingUnionType GetType() const;
        /// Функция для получения ширины объединения
        /// @return ширина объединения
        size_t GetWidth() const;
        /// Функция для получения высоты объединения
        /// @return высота объединения
        size_t GetHeight() const;
        /// Функция для получения объекта
        /// @param column колонка (относительно всей карты), в которой находится объект
        /// @param row строка (относительно всей карты), в которой находится объект
        /// @return умный указатель на объект
        CoatingUnique const& GetCoatingObject(int column, int row) const;
        /// Функция для получения длины (количества покрытий)
        /// @return длина
        virtual float GetLength() const;
        
        /// Функция для проверки корректности колонки в данном объединении
        /// @return true, если объединение содержит данную колонку, иначе false
        bool IsCorrectColumn(int column) const;
        /// Функция для проверки корректности строки в данном объединении
        /// @return true, если объединение содержит данную строку, иначе false
        bool IsCorrectRow(int row) const;

        /// Функция для добавления спрайтов на сцену
        /// @param layer слой, на который надо добавить спрайты управляющего блока
        virtual void ShowSprites(cocos2d::Layer* const layer);
        /// Функция для удаления спрайтов со сцены
        /// @param layer слой, с которого надо удалить спрайты управляющего блока
        virtual void ReleaseSprites(cocos2d::Layer* const layer);

    protected:
        /// Функция для получения левой колонки объединения
        /// @return левая колонка объединения
        int GetColumn_() const;
        /// Функция для получения нижней строки объединения
        /// @return нижняя строка объединения
        int GetRow_() const;

    private:
        /// Тип объединения покрытий (получившегося элемента)
        CoatingUnionType type_;
        /// Левая колонка объединения
        int column_;
        /// Нижняя строка объединения
        int row_;
        /// Ширина объединения
        size_t width_;
        /// Высота объединения
        size_t height_;
        /// @Матрица объектов покрытий
        CoatingMatrix objects_;
    };
}

#endif // __COATING_UNION_INCLUDED__