#ifndef __COATING_UNION_INCLUDED__
#define __COATING_UNION_INCLUDED__

#include "General.h"

namespace rtm {
    class CoatingUnion abstract
    {
    public:
        CoatingUnion();
        CoatingUnion(CoatingType type, int column, int row, std::vector<std::vector<CoatingObjectUnique>>&& objects);

        virtual ~CoatingUnion() = default;

        CoatingType GetType() const;
        CoatingObjectUnique const& GetCoatingObject(int column, int row) const;

        bool IsCorrectColumn(int column) const;
        bool IsCorrectRow(int row) const;

        void ShowCoatingObjects(WorldScene* const scene);
        void ReleaseCoatingObjects(WorldScene* const scene);

    protected:
        size_t GetWidth_() const;
        size_t GetHeight_() const;

    private:
        CoatingType type_;
        int column_;
        int row_;
        size_t width_;
        size_t height_;
        std::vector<std::vector<CoatingObjectUnique>> objects_;
    };
}

#endif // __COATING_UNION_INCLUDED__