#include "TCrossroadObject.h"
#include "CoatingObject.h"
#include "RoadCoating.h"

rtm::TCrossroadObject::TCrossroadObject()
    : CoatingUnion{}
    , upLines_(0)
    , toRightLines_(0)
    , downLines_(0)
    , toLeftLines_(0)
    , nullDirection_(TopDirection)
{}

rtm::TCrossroadObject::TCrossroadObject(int column, int row, size_t upLines, size_t toRightLines, 
    size_t downLines, size_t toLeftLines, DirectionType nullDirection)
    : CoatingUnion{ TCrossroadType, column, row,
        std::move(TCrossroadObject::CreateMatrix(column, row, upLines, toRightLines, downLines, toLeftLines, nullDirection)) }
    , upLines_(upLines)
    , toRightLines_(toRightLines)
    , downLines_(downLines)
    , toLeftLines_(toLeftLines)
    , nullDirection_(nullDirection)
{}

std::vector<std::vector<rtm::CoatingObjectUnique>> rtm::TCrossroadObject::CreateMatrix(int column, int row,
    size_t upLines, size_t toRightLines, size_t downLines, size_t toLeftLines, DirectionType nullDirection)
{
    size_t lastCol{ upLines + downLines - 1 };
    size_t lastRow{ toRightLines + toLeftLines - 1 };

    std::vector<std::vector<rtm::CoatingObjectUnique>> result{ upLines + downLines };
    for (size_t i = 0; i <= lastCol; ++i) {
        result[i] = std::vector<rtm::CoatingObjectUnique>{ toRightLines + toLeftLines };
    }

    if (lastCol == 0 && lastRow == 0) {
        result[0][0] = std::make_unique<RoadCoating>(RoadTypeNo8, column, row, nullDirection);
    }
    else if (lastCol == 0) {
        if (nullDirection == TopDirection) {
            result[0][0] = std::make_unique<RoadCoating>(RoadTypeNo5, column, row, nullDirection);
        }
        else if (nullDirection == BottomDirection) {
            result[0][lastRow] = std::make_unique<RoadCoating>(RoadTypeNo5, column, row + lastRow, nullDirection);
        }
        else if (nullDirection == RightDirection) {
            result[0][0] = std::make_unique<RoadCoating>(RoadTypeNo11, column, row, nullDirection);
            result[0][lastRow] = std::make_unique<RoadCoating>(RoadTypeNo9, column, row + lastRow, nullDirection);
        }
        else if (nullDirection == LeftDirection) {
            result[0][0] = std::make_unique<RoadCoating>(RoadTypeNo9, column, row, nullDirection);
            result[0][lastRow] = std::make_unique<RoadCoating>(RoadTypeNo11, column, row + lastRow, nullDirection);
        }
    }
    else if (lastRow == 0) {
        if (nullDirection == TopDirection) {
            result[0][0] = std::make_unique<RoadCoating>(RoadTypeNo9, column, row, nullDirection);
            result[lastCol][0] = std::make_unique<RoadCoating>(RoadTypeNo11, column + lastCol, row, nullDirection);
        }
        else if (nullDirection == BottomDirection) {
            result[0][0] = std::make_unique<RoadCoating>(RoadTypeNo11, column, row, nullDirection);
            result[lastCol][0] = std::make_unique<RoadCoating>(RoadTypeNo9, column + lastCol, row, nullDirection);
        }
        else if (nullDirection == RightDirection) {
            result[0][0] = std::make_unique<RoadCoating>(RoadTypeNo5, column, row, nullDirection);
        }
        else if (nullDirection == LeftDirection) {
            result[lastCol][0] = std::make_unique<RoadCoating>(RoadTypeNo5, column + lastCol, row, nullDirection);
        }
    }
    else {
        if (nullDirection == TopDirection) {
            result[0][0] = std::make_unique<RoadCoating>(RoadTypeNo6, column, row, TopDirection);
            result[lastCol][0] = std::make_unique<RoadCoating>(RoadTypeNo6, column + lastCol, row, LeftDirection);
        }
        else if (nullDirection == BottomDirection) {
            result[0][lastRow] = std::make_unique<RoadCoating>(RoadTypeNo6, column, row + lastRow, RightDirection);
            result[lastCol][lastRow] = std::make_unique<RoadCoating>(RoadTypeNo6, column + lastCol, row + lastRow, BottomDirection);
        }
        else if (nullDirection == RightDirection) {
            result[0][0] = std::make_unique<RoadCoating>(RoadTypeNo6, column, row, TopDirection);
            result[0][lastRow] = std::make_unique<RoadCoating>(RoadTypeNo6, column, row + lastRow, RightDirection);
        }
        else if (nullDirection == LeftDirection) {
            result[lastCol][0] = std::make_unique<RoadCoating>(RoadTypeNo6, column + lastCol, row, LeftDirection);
            result[lastCol][lastRow] = std::make_unique<RoadCoating>(RoadTypeNo6, column + lastCol, row + lastRow, BottomDirection);
        }
    }

    // Block backward directions    
    for (size_t i = 0; i <= lastCol; ++i) {
        for (size_t j = 0; j <= lastRow; ++j) {
            if (!result[i][j]) {
                if (
                    (j == lastRow && nullDirection == TopDirection) ||
                    (i == lastCol && nullDirection == RightDirection) ||
                    (j == 0 && nullDirection == BottomDirection) ||
                    (i == 0 && nullDirection == LeftDirection)
                ) {
                    result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo10, column + i, row + j, nullDirection);
                }
                else {
                    result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo7, column + i, row + j, TopDirection);
                }
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
