#include "ControlUnit.h"
#include "WorldController.h"
#include "WorldScene.h"

rtm::ControlUnit::ControlUnit()
    : type_{ NoControlUnit }
    , column_{ 0 }
    , row_{ 0 }
    , linesCounts_{ 0, 0, 0, 0 }
    , nullDirection_{ NullAngle }
    , signals_{ DEFAULT_CROSSROAD_SIGNALS }
    , time_{ 0.f }
{}

rtm::ControlUnit::ControlUnit(ControlUnitType type, int column, int row, LinesCounts linesCounts)
    : type_{ type }
    , column_{ column }
    , row_{ row }
    , linesCounts_{ linesCounts }
    , nullDirection_{ NullAngle }
    , signals_{ DEFAULT_CROSSROAD_SIGNALS }
    , time_{ 0.f }
{
    InitSignals_();
}

rtm::ControlUnit::ControlUnit(ControlUnitType type, int column, int row, LinesCounts linesCounts, AngleType nullDirection)
    : type_{ type }
    , column_{ column }
    , row_{ row }
    , linesCounts_{ linesCounts }
    , nullDirection_{ nullDirection }
    , signals_{ DEFAULT_CROSSROAD_SIGNALS }
    , time_{ 0.f }
{
    InitSignals_();
}

void rtm::ControlUnit::Update(WorldController* const world)
{
    time_ += world->GetDeltaTime();
    switch (type_)
    {
    case rtm::ControlUnitNo1:
        if (time_ > 10.f) {
            if (signals_[Upward][Upward] == Allowed) {
                signals_[Upward][Upward] = Forbidden;
                signals_[Downward][Downward] = Forbidden;
                signals_[Rightward][Rightward] = Allowed;
                signals_[Leftward][Leftward] = Allowed;
            }
            else {
                signals_[Upward][Upward] = Allowed;
                signals_[Downward][Downward] = Allowed;
                signals_[Rightward][Rightward] = Forbidden;
                signals_[Leftward][Leftward] = Forbidden;
            }
            ResetSprites_();
            time_ = 0.f;
        }
        break;
    }
}

rtm::ControlUnit::operator bool() const
{
    return type_ != NoControlUnit;
}

rtm::TrafficLightSignal rtm::ControlUnit::GetSignal(DirectionType from, DirectionType to) const
{
    return signals_[from][to];
}

void rtm::ControlUnit::InitSignals_()
{
    switch (type_)
    {
    case rtm::ControlUnitNo1:
        // Close turns
        signals_[Upward][Leftward] = Closed;
        signals_[Upward][Rightward] = Closed;
        signals_[Upward][Downward] = Closed;
        signals_[Rightward][Upward] = Closed;
        signals_[Rightward][Downward] = Closed;
        signals_[Rightward][Leftward] = Closed;
        signals_[Downward][Rightward] = Closed;
        signals_[Downward][Leftward] = Closed;
        signals_[Downward][Upward] = Closed;
        signals_[Leftward][Downward] = Closed;
        signals_[Leftward][Upward] = Closed;
        signals_[Leftward][Rightward] = Closed;
        // Set "green lines"
        signals_[Upward][Upward] = Allowed;
        signals_[Downward][Downward] = Allowed;
        // Set "red lines"
        signals_[Rightward][Rightward] = Forbidden;
        signals_[Leftward][Leftward] = Forbidden;
        break;
    }
}

void rtm::ControlUnit::ShowSprites(WorldScene* const scene)
{
    for (size_t i = 0; i < 4; ++i) {
        // If from TCrossroad's nullDirection
        if ((i + 2) % 4 == nullDirection_) {
            sprites_[i][0] = { nullptr, nullptr, nullptr };
            sprites_[i][1] = { nullptr, nullptr, nullptr };
            sprites_[i][2] = { nullptr, nullptr, nullptr };
            continue;
        }

        // Create sprites
        for (size_t k = 0; k < 5; ++k) {
            sprites_[i][0][k] = cocos2d::Sprite::create(GetSignalFile_(ForwardSignal + k));
            sprites_[i][1][k] = cocos2d::Sprite::create(GetSignalFile_(RightwardSignal + k));
            sprites_[i][2][k] = cocos2d::Sprite::create(GetSignalFile_(LeftwardSignal + k));
        }

        // Calc coords
        int col{ column_ };
        int row{ row_ };
        if (nullDirection_ == NullAngle) {
            if (i == Upward) {
                col += 1 + linesCounts_[Downward];
            }
            else if (i == Rightward) {
                row += 1;
            }
            else if (i == Downward) {
                col += 1;
                row += 1 + linesCounts_[Rightward] + linesCounts_[Leftward];
            }
            else if (i == Leftward) {
                col += 1 + linesCounts_[Downward] + linesCounts_[Upward];
                row += 1 + linesCounts_[Rightward];
            }
        }
        else {
            size_t topBorder{ 1 };
            size_t rightBorder{ 1 };
            size_t bottomBorder{ 1 };
            size_t leftBorder{ 1 };

            if (nullDirection_ == Up) {
                topBorder = 0;
            }
            else if (nullDirection_ == Right) {
                rightBorder = 0;
            }
            else if (nullDirection_ == Down) {
                bottomBorder = 0;
            }
            else if (nullDirection_ == Left) {
                leftBorder = 0;
            }

            if (i == Upward) {
                col += leftBorder + linesCounts_[Downward];
            }
            else if (i == Rightward) {
                row += bottomBorder;
            }
            else if (i == Downward) {
                col += leftBorder;
                row += linesCounts_[Rightward] + linesCounts_[Leftward] + topBorder + bottomBorder - 1;
            }
            else if (i == Leftward) {
                col += linesCounts_[Downward] + linesCounts_[Upward] + leftBorder + rightBorder - 1;
                row += bottomBorder + linesCounts_[Rightward];
            }
        }

        float x{ CellToPixel(col) };
        float y{ CellToPixel(row) };
        if (i == Upward || i == Downward) {
            x += CELL_SIZE * (linesCounts_[i] - 1) / 2;
        }
        else if (i == Rightward || i == Leftward) {
            y += CELL_SIZE * (linesCounts_[i] - 1) / 2;
        }

        // Set position
        for (size_t k = 0; k < 5; ++k) {
            float angle{ 0 };
            if (i == Rightward) {
                angle = 90.f;
            }
            else if (i == Downward) {
                angle = 180.f;
            }
            else if (i == Leftward) {
                angle = -90.f;
            }

            scene->addChild(sprites_[i][0][k], FORWARD_SIGNAL_Z_ORDER);
            scene->addChild(sprites_[i][1][k], LEFTWARD_SIGNAL_Z_ORDER);
            scene->addChild(sprites_[i][2][k], RIGHTWARD_SIGNAL_Z_ORDER);
            for (size_t l = 0; l < 3; ++l) {
                sprites_[i][l][k]->setAnchorPoint(cocos2d::Vec2{ 0.5, 0.5 });
                sprites_[i][l][k]->setPosition(cocos2d::Vec2{ x, y });
                sprites_[i][l][k]->setRotation(angle);
            }
        }
    }

    ResetSprites_();
}

void rtm::ControlUnit::ReleaseSprites(WorldScene * const scene)
{
    for (size_t i = 0; i < 4; ++i) {
        for (size_t l = 0; l < 3; ++l) {
            for (size_t k = 0; k < 5; ++k) {
                if (sprites_[i][l][k] != nullptr) {
                    scene->removeChild(sprites_[i][l][k]);
                    sprites_[i][l][k] = nullptr;
                }
            }
        }
    }
}

void rtm::ControlUnit::ResetSprites_()
{
    for (size_t i = 0; i < 4; ++i) {
        for (size_t k = 0; k < 5; ++k) {
            // Forward
            if (sprites_[i][0][k] != nullptr) {
                sprites_[i][0][k]->setVisible(signals_[i][i] == k);
            }
            // Left
            if (sprites_[i][1][k] != nullptr) {
                sprites_[i][1][k]->setVisible(signals_[i][(i - 1) % 4] == k);
            }
            // Right
            if (sprites_[i][2][k] != nullptr) {
                sprites_[i][2][k]->setVisible(signals_[i][(i + 1) % 4] == k);
            }
        }
    }
}

std::string rtm::ControlUnit::GetSignalFile_(size_t id)
{
    std::string filename{ SIGNAL_FILENAME_MASK };
    auto it{ filename.find("%No%") };
    filename.replace(it, 4, std::to_string(static_cast<int>(id)));

    return filename;
}