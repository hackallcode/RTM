#include "TurnObject.h"
#include "CoatingObject.h"
#include "RoadCoating.h"
#include "DrivewayObject.h"

rtm::TurnObject::TurnObject()
    : CoatingUnion{}
    , angle_{ Up }
{}

rtm::TurnObject::TurnObject(int column, int row, size_t linesCount, bool isRight, AngleType angle)
    : CoatingUnion{ TurnType, column, row, std::move(
        isRight
        ? TurnObject::RightTurnMatrix(column, row, linesCount, angle)
        : TurnObject::LeftTurnMatrix(column, row, linesCount, angle)
    ) }
    , angle_{ angle }
{}

rtm::CoatingMatrix rtm::TurnObject::RightTurnMatrix(int column, int row, size_t linesCount, AngleType angle)
{
    CoatingMatrix inputDriveway{ DrivewayObject::DrivewayMatrix(column, row, linesCount, linesCount, angle) };
    CoatingMatrix outputDriveway{
        DrivewayObject::DrivewayMatrix(column, row, linesCount, linesCount, AngleTypeSum(angle, Right))
    };

    CoatingMatrix result{ linesCount };
    for (size_t i = 0; i < linesCount; ++i) {
        result[i] = CoatingVector{ linesCount };
        for (size_t j = 0; j < linesCount; ++j) {
            if (linesCount == 1) {
                result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo14, column + i, row + j, angle);
            }
            else if (angle == Up) {
                if (i + j == linesCount - 1) {
                    if (i == 0) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo15, column + i, row + j, angle);
                    }
                    else if (i == linesCount - 1) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo17, column + i, row + j, angle);
                    }
                    else {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo16, column + i, row + j, angle);
                    }
                }
                else if (i + j < linesCount - 1) {
                    result[i][j] = std::move(inputDriveway[i][j]);
                }
                else {
                    result[i][j] = std::move(outputDriveway[i][j]);
                }
            }
            else if (angle == Right) {
                if (i == j) {
                    if (i == 0) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo17, column + i, row + j, angle);
                    }
                    else if (i == linesCount - 1) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo15, column + i, row + j, angle);
                    }
                    else {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo16, column + i, row + j, angle);
                    }
                }
                else if (j < i) {
                    result[i][j] = std::move(outputDriveway[i][j]);
                }
                else {
                    result[i][j] = std::move(inputDriveway[i][j]);
                }
            }
            else if (angle == Down) {
                if (i + j == linesCount - 1) {
                    if (i == 0) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo17, column + i, row + j, angle);
                    }
                    else if (i == linesCount - 1) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo15, column + i, row + j, angle);
                    }
                    else {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo16, column + i, row + j, angle);
                    }
                }
                else if (i + j < linesCount - 1) {
                    result[i][j] = std::move(outputDriveway[i][j]);
                }
                else {
                    result[i][j] = std::move(inputDriveway[i][j]);
                }
            }
            else if (angle == Left) {
                if (i == j) {
                    if (i == 0) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo15, column + i, row + j, angle);
                    }
                    else if (i == linesCount - 1) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo17, column + i, row + j, angle);
                    }
                    else {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo16, column + i, row + j, angle);
                    }
                }
                else if (j < i) {
                    result[i][j] = std::move(inputDriveway[i][j]);
                }
                else {
                    result[i][j] = std::move(outputDriveway[i][j]);
                }
            }
        }
    }

    // Block backward directions
    AngleType backwardAngle{ AngleTypeSum(angle, Down) };
    for (size_t i = 0; i < linesCount; ++i) {
        if (angle == Up || angle == Down) {
            result[i][linesCount - 1 - i]->SetDirectionAvailability(backwardAngle, false);
        }
        else if (angle == Right || angle == Left) {
            result[i][i]->SetDirectionAvailability(backwardAngle, false);
        }
    }

    return result;
}

rtm::CoatingMatrix rtm::TurnObject::LeftTurnMatrix(int column, int row, size_t linesCount, AngleType angle)
{
    CoatingMatrix inputDriveway{ DrivewayObject::DrivewayMatrix(column, row, linesCount, linesCount, angle) };
    CoatingMatrix outputDriveway{
        DrivewayObject::DrivewayMatrix(column, row, linesCount, linesCount, AngleTypeSum(angle, Left))
    };

    AngleType coatingAngle{ AngleTypeSum(angle, Right) };
    CoatingMatrix result{ linesCount };
    for (size_t i = 0; i < linesCount; ++i) {
        result[i] = CoatingVector{ linesCount };
        for (size_t j = 0; j < linesCount; ++j) {
            if (linesCount == 1) {
                result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo14, column + i, row + j, coatingAngle);
            }
            else if (angle == Up) {
                if (i == j) {
                    if (i == 0) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo17, column + i, row + j, coatingAngle);
                    }
                    else if (i == linesCount - 1) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo15, column + i, row + j, coatingAngle);
                    }
                    else {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo16, column + i, row + j, coatingAngle);
                    }
                }
                else if (j < i) {
                    result[i][j] = std::move(inputDriveway[i][j]);
                }
                else {
                    result[i][j] = std::move(outputDriveway[i][j]);
                }
            }
            else if (angle == Right) {
                if (i + j == linesCount - 1) {
                    if (i == 0) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo17, column + i, row + j, coatingAngle);
                    }
                    else if (i == linesCount - 1) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo15, column + i, row + j, coatingAngle);
                    }
                    else {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo16, column + i, row + j, coatingAngle);
                    }
                }
                else if (i + j < linesCount - 1) {
                    result[i][j] = std::move(inputDriveway[i][j]);
                }
                else {
                    result[i][j] = std::move(outputDriveway[i][j]);
                }
            }
            else if (angle == Down) {
                if (i == j) {
                    if (i == 0) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo15, column + i, row + j, coatingAngle);
                    }
                    else if (i == linesCount - 1) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo17, column + i, row + j, coatingAngle);
                    }
                    else {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo16, column + i, row + j, coatingAngle);
                    }
                }
                else if (j < i) {
                    result[i][j] = std::move(outputDriveway[i][j]);
                }
                else {
                    result[i][j] = std::move(inputDriveway[i][j]);
                }
            }
            else if (angle == Left) {
                if (i + j == linesCount - 1) {
                    if (i == 0) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo15, column + i, row + j, coatingAngle);
                    }
                    else if (i == linesCount - 1) {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo17, column + i, row + j, coatingAngle);
                    }
                    else {
                        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo16, column + i, row + j, coatingAngle);
                    }
                }
                else if (i + j < linesCount - 1) {
                    result[i][j] = std::move(outputDriveway[i][j]);
                }
                else {
                    result[i][j] = std::move(inputDriveway[i][j]);
                }
            }
        }
    }

    // Block backward directions
    AngleType backwardAngle{ AngleTypeSum(angle, Down) };
    for (size_t i = 0; i < linesCount; ++i) {
        if (angle == Up || angle == Down) {
            result[i][i]->SetDirectionAvailability(backwardAngle, false);
        }
        else if (angle == Right || angle == Left) {
            result[i][linesCount - 1 - i]->SetDirectionAvailability(backwardAngle, false);
        }
    }
    
    return result;
}

rtm::AngleType rtm::TurnObject::GetAngle() const
{
    return angle_;
}