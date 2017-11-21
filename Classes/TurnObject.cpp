#include "TurnObject.h"
#include "CoatingObject.h"
#include "RoadCoating.h"
#include "DrivewayObject.h"

rtm::TurnObject::TurnObject()
    : CoatingUnion{}
    , angle_{ Up }
{}

rtm::TurnObject::TurnObject(int column, int row, size_t linesCount, AngleType angle, bool isRight)
    : CoatingUnion{ TurnType, column, row, std::move(
        isRight
        ? TurnObject::RightTurnMatrix(column, row, linesCount, angle)
        : TurnObject::LeftTurnMatrix(column, row, linesCount, angle)
    ) }
    , angle_{ angle }
{}

rtm::CoatingMatrix rtm::TurnObject::RightTurnMatrix(int column, int row, size_t linesCount, AngleType angle)
{
    size_t topBorder{ 0 };
    size_t rightBorder{ 0 };
    size_t bottomBorder{ 0 };
    size_t leftBorder{ 0 };

    if (angle == Up) {
        rightBorder = 1;
        bottomBorder = 1;
    }
    else if (angle == Right) {
        bottomBorder = 1;
        leftBorder = 1;
    }
    else if (angle == Down) {
        leftBorder = 1;
        topBorder = 1;
    }
    else if (angle == Left) {
        topBorder = 1;
        rightBorder = 1;
    }

    size_t size{ linesCount + 1 };

    CoatingMatrix inputDriveway{
        DrivewayObject::DrivewayMatrix(column + topBorder, row + leftBorder, linesCount, linesCount, angle)
    };
    CoatingMatrix outputDriveway{
        DrivewayObject::DrivewayMatrix(column + bottomBorder, row + rightBorder, linesCount, linesCount, SumAngleTypes(angle, Right))
    };

    CoatingMatrix result{ size };
    for (size_t i = 0; i < size; ++i) {
        result[i] = CoatingVector{ size };
    }

    // Specific coatings
    if (linesCount == 1) {
        size_t i{ leftBorder };
        size_t j{ bottomBorder };
        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo14, column + i, row + j, angle);

        i = rightBorder;
        j = topBorder;
        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo18, column + i, row + j, angle);
    }
    else if (angle == Up || angle == Down) {
        for (size_t i{ 0 }; i < size; ++i) {
            RoadType type{ static_cast<RoadType>(0) };

            if (angle == Up) {
                if (i == 0) {
                    type = RoadTypeNo15;
                }
                else if (i == size - 2) {
                    type = RoadTypeNo17;
                }
                else if (i == size - 1) {
                    type = RoadTypeNo18;
                }
                else {
                    type = RoadTypeNo16;
                }
            }
            else if (angle == Down) {
                if (i == 0) {
                    type = RoadTypeNo18;
                }
                else if (i == 1) {
                    type = RoadTypeNo17;
                }
                else if (i == size - 1) {
                    type = RoadTypeNo15;
                }
                else {
                    type = RoadTypeNo16;
                }
            }

            size_t j{ size - 1 - i };
            result[i][j] = std::make_unique<RoadCoating>(type, column + i, row + j, angle);
        }
    }
    else if (angle == Right || angle == Left) {
        for (size_t i{ 0 }; i < size; ++i) {
            RoadType type{ static_cast<RoadType>(0) };
            if (angle == Right) {
                if (i == 0) {
                    type = RoadTypeNo18;
                }
                else if (i == 1) {
                    type = RoadTypeNo17;
                }
                else if (i == size - 1) {
                    type = RoadTypeNo15;
                }
                else {
                    type = RoadTypeNo16;
                }
            }
            else if (angle == Left) {
                if (i == 0) {
                    type = RoadTypeNo15;
                }
                else if (i == size - 2) {
                    type = RoadTypeNo17;
                }
                else if (i == size - 1) {
                    type = RoadTypeNo18;
                }
                else {
                    type = RoadTypeNo16;
                }
            }

            result[i][i] = std::make_unique<RoadCoating>(type, column + i, row + i, angle);
        }
    }

    // Set driveways coatings
    if (angle == Up) {
        for (size_t i{ 0 }; i < size; ++i) {
            for (size_t j{ 0 }; j < size; ++j) {
                if (!result[i][j]) {
                    if (i + j < size - 1) {
                        result[i][j] = std::move(inputDriveway[i - topBorder][j - leftBorder]);
                    }
                    else {
                        result[i][j] = std::move(outputDriveway[i - bottomBorder][j - rightBorder]);
                    }
                }
            }
        }
    }
    else if (angle == Right) {
        for (size_t i{ 0 }; i < size; ++i) {
            for (size_t j{ 0 }; j < size; ++j) {
                if (!result[i][j]) {
                    if (j < i) {
                        result[i][j] = std::move(outputDriveway[i - bottomBorder][j - rightBorder]);
                    }
                    else {
                        result[i][j] = std::move(inputDriveway[i - topBorder][j - leftBorder]);
                    }
                }
            }
        }
    }
    else if (angle == Down) {
        for (size_t i{ 0 }; i < size; ++i) {
            for (size_t j{ 0 }; j < size; ++j) {
                if (!result[i][j]) {
                    if (i + j < size - 1) {
                        result[i][j] = std::move(outputDriveway[i - bottomBorder][j - rightBorder]);
                    }
                    else {
                        result[i][j] = std::move(inputDriveway[i - topBorder][j - leftBorder]);
                    }
                }
            }
        }
    }
    else if (angle == Left) {
        for (size_t i{ 0 }; i < size; ++i) {
            for (size_t j{ 0 }; j < size; ++j) {
                if (!result[i][j]) {
                    if (j < i) {
                        result[i][j] = std::move(inputDriveway[i - topBorder][j - leftBorder]);
                    }
                    else {
                        result[i][j] = std::move(outputDriveway[i - bottomBorder][j - rightBorder]);
                    }
                }
            }
        }
    }

    // Block backward directions
    AngleType backwardAngle{ SumAngleTypes(angle, Down) };
    for (size_t i{ 0 }; i < size; ++i) {
        if (angle == Up || angle == Down) {
            result[i][size - 1 - i]->SetDirectionAvailability(backwardAngle, false);
        }
        else if (angle == Right || angle == Left) {
            result[i][i]->SetDirectionAvailability(backwardAngle, false);
        }
    }

    return result;
}

rtm::CoatingMatrix rtm::TurnObject::LeftTurnMatrix(int column, int row, size_t linesCount, AngleType angle)
{
    size_t topBorder{ 0 };
    size_t rightBorder{ 0 };
    size_t bottomBorder{ 0 };
    size_t leftBorder{ 0 };

    if (angle == Up) {
        bottomBorder = 1;
        leftBorder = 1;
    }
    else if (angle == Right) {
        leftBorder = 1;
        topBorder = 1;
    }
    else if (angle == Down) {
        topBorder = 1;
        rightBorder = 1;
    }
    else if (angle == Left) {
        rightBorder = 1;
        bottomBorder = 1;
    }

    size_t size{ linesCount + 1 };

    CoatingMatrix inputDriveway{
        DrivewayObject::DrivewayMatrix(column + bottomBorder, row + rightBorder, linesCount, linesCount, angle)
    };
    CoatingMatrix outputDriveway{
        DrivewayObject::DrivewayMatrix(column + topBorder, row + leftBorder, linesCount, linesCount, SumAngleTypes(angle, Left))
    };

    CoatingMatrix result{ size };
    for (size_t i = 0; i < size; ++i) {
        result[i] = CoatingVector{ size };
    }

    AngleType coatingAngle{ SumAngleTypes(angle, Right) };

    // Specific coatings
    if (linesCount == 1) {
        size_t i{ leftBorder };
        size_t j{ bottomBorder };
        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo14, column + i, row + j, coatingAngle);

        i = rightBorder;
        j = topBorder;
        result[i][j] = std::make_unique<RoadCoating>(RoadTypeNo18, column + i, row + j, coatingAngle);
    }
    else if (angle == Up || angle == Down) {
        for (size_t i{ 0 }; i < size; ++i) {
            RoadType type{ static_cast<RoadType>(0) };

            if (angle == Up) {
                if (i == 0) {
                    type = RoadTypeNo18;
                }
                else if (i == 1) {
                    type = RoadTypeNo17;
                }
                else if (i == size - 1) {
                    type = RoadTypeNo15;
                }
                else {
                    type = RoadTypeNo16;
                }
            }
            else if (angle == Down) {
                if (i == 0) {
                    type = RoadTypeNo15;
                }
                else if (i == size - 2) {
                    type = RoadTypeNo17;
                }
                else if (i == size - 1) {
                    type = RoadTypeNo18;
                }
                else {
                    type = RoadTypeNo16;
                }
            }

            result[i][i] = std::make_unique<RoadCoating>(type, column + i, row + i, coatingAngle);
        }
    }
    else if (angle == Right || angle == Left) {
        for (size_t i{ 0 }; i < size; ++i) {
            RoadType type{ static_cast<RoadType>(0) };
            if (angle == Right) {
                if (i == 0) {
                    type = RoadTypeNo18;
                }
                else if (i == 1) {
                    type = RoadTypeNo17;
                }
                else if (i == size - 1) {
                    type = RoadTypeNo15;
                }
                else {
                    type = RoadTypeNo16;
                }
            }
            else if (angle == Left) {
                if (i == 0) {
                    type = RoadTypeNo15;
                }
                else if (i == size - 2) {
                    type = RoadTypeNo17;
                }
                else if (i == size - 1) {
                    type = RoadTypeNo18;
                }
                else {
                    type = RoadTypeNo16;
                }
            }

            size_t j{ size - 1 - i };
            result[i][j] = std::make_unique<RoadCoating>(type, column + i, row + j, coatingAngle);
        }
    }

    // Set driveways coatings
    if (angle == Up) {
        for (size_t i{ 0 }; i < size; ++i) {
            for (size_t j{ 0 }; j < size; ++j) {
                if (!result[i][j]) {
                    if (j < i) {
                        result[i][j] = std::move(inputDriveway[i - bottomBorder][j - rightBorder]);
                    }
                    else {
                        result[i][j] = std::move(outputDriveway[i - topBorder][j - leftBorder]);
                    }
                }
            }
        }
    }
    else if (angle == Right) {
        for (size_t i{ 0 }; i < size; ++i) {
            for (size_t j{ 0 }; j < size; ++j) {
                if (!result[i][j]) {
                    if (i + j < size - 1) {
                        result[i][j] = std::move(inputDriveway[i - bottomBorder][j - rightBorder]);
                    }
                    else {
                        result[i][j] = std::move(outputDriveway[i - topBorder][j - leftBorder]);
                    }
                }
            }
        }
    }
    else if (angle == Down) {
        for (size_t i{ 0 }; i < size; ++i) {
            for (size_t j{ 0 }; j < size; ++j) {
                if (!result[i][j]) {
                    if (j < i) {
                        result[i][j] = std::move(outputDriveway[i - topBorder][j - leftBorder]);
                    }
                    else {
                        result[i][j] = std::move(inputDriveway[i - bottomBorder][j - rightBorder]);
                    }
                }
            }
        }
    }
    else if (angle == Left) {
        for (size_t i{ 0 }; i < size; ++i) {
            for (size_t j{ 0 }; j < size; ++j) {
                if (!result[i][j]) {
                    if (i + j < size - 1) {
                        result[i][j] = std::move(outputDriveway[i - topBorder][j - leftBorder]);
                    }
                    else {
                        result[i][j] = std::move(inputDriveway[i - bottomBorder][j - rightBorder]);
                    }
                }
            }
        }
    }

    // Block backward directions
    AngleType backwardAngle{ SumAngleTypes(angle, Down) };
    for (size_t i = 0; i < size; ++i) {
        if (angle == Up || angle == Down) {
            result[i][i]->SetDirectionAvailability(backwardAngle, false);
        }
        else if (angle == Right || angle == Left) {
            result[i][size - 1 - i]->SetDirectionAvailability(backwardAngle, false);
        }
    }

    return result;
}

rtm::AngleType rtm::TurnObject::GetAngle() const
{
    return angle_;
}