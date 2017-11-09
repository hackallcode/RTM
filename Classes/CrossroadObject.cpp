#include "CrossroadObject.h"
#include "CoatingObject.h"
#include "RoadCoating.h"

rtm::CrossroadObject::CrossroadObject()
    : CoatingUnion{}
    , upLines_(0)
    , toRightLines_(0)
    , downLines_(0)
    , toLeftLines_(0)
{}

rtm::CrossroadObject::CrossroadObject(int column, int row, 
    size_t upLines, size_t toRightLines, size_t downLines, size_t toLeftLines)
    : CoatingUnion{ CrossroadType, column, row,
        std::move(CrossroadObject::CreateMatrix(column, row, upLines, toRightLines, downLines, toLeftLines)) }
    , upLines_(upLines)
    , toRightLines_(toRightLines)
    , downLines_(downLines)
    , toLeftLines_(toLeftLines)
{}

std::vector<std::vector<rtm::CoatingObjectUnique>> rtm::CrossroadObject::CreateMatrix(int column, int row,
    size_t upLines, size_t toRightLines, size_t downLines, size_t toLeftLines)
{
    size_t lastCol{ upLines + downLines - 1 };
    size_t lastRow{ toRightLines + toLeftLines - 1 };

    std::vector<std::vector<rtm::CoatingObjectUnique>> result{ upLines + downLines };
    for (size_t i = 0; i <= lastCol; ++i) {
        result[i] = std::vector<rtm::CoatingObjectUnique>{ toRightLines + toLeftLines };
    }

    if (lastCol == 0 && lastRow == 0) {
        result[0][0] = std::make_unique<RoadCoating>(RoadTypeNo4, column, row, TopDirection);
    }
    else if (lastCol == 0) {
        result[0][0] = std::make_unique<RoadCoating>(RoadTypeNo5, column, row, TopDirection);
        result[0][lastRow] = std::make_unique<RoadCoating>(RoadTypeNo5, column, row + lastRow, BottomDirection);
    }
    else if (lastRow == 0) {
        result[0][0] = std::make_unique<RoadCoating>(RoadTypeNo5, column, row, RightDirection);
        result[lastCol][0] = std::make_unique<RoadCoating>(RoadTypeNo5, column + lastCol, row, LeftDirection);
    }
    else {
        result[0][0] = std::make_unique<RoadCoating>(RoadTypeNo6, column, row, TopDirection);
        result[lastCol][0] = std::make_unique<RoadCoating>(RoadTypeNo6, column + lastCol, row, LeftDirection);
        result[0][lastRow] = std::make_unique<RoadCoating>(RoadTypeNo6, column, row + lastRow, RightDirection);
        result[lastCol][lastRow] = std::make_unique<RoadCoating>(RoadTypeNo6, column + lastCol, row + lastRow, BottomDirection);
    }

    // Block backward directions    
    for (size_t i = 0; i <= lastCol; ++i) {
        for (size_t j = 0; j <= lastRow; ++j) {
            if (!result[i][j]) {
                result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo7, column + i, row + j, TopDirection);
            }
            // Left side
            if (0 <= i && i <= downLines) {
                // Bottom side
                if (0 <= j && j <= toRightLines) {
                    result[i][j]->SetDirectionAvailability(TopDirection, false);
                    result[i][j]->SetDirectionAvailability(LeftDirection, false);
                }
                // Top side
                else {
                    result[i][j]->SetDirectionAvailability(TopDirection, false);
                    result[i][j]->SetDirectionAvailability(RightDirection, false);
                }
            }
            // Right side
            else {
                // Bottom side
                if (0 <= j && j <= toRightLines) {
                    result[i][j]->SetDirectionAvailability(BottomDirection, false);
                    result[i][j]->SetDirectionAvailability(LeftDirection, false);
                }
                // Top side
                else {
                    result[i][j]->SetDirectionAvailability(BottomDirection, false);
                    result[i][j]->SetDirectionAvailability(RightDirection, false);
                }
            }
        }
    }

    return result;
}
