#include "ControlUnit.h"
#include "WorldController.h"
#include "WorldScene.h"

///////////////////////
//  LOCAL FUNCTIONS  //
///////////////////////

inline size_t LeftwardIndex(size_t forwardIndex) {
    return (forwardIndex - 1) % 4;
}

inline size_t RightwardIndex(size_t forwardIndex) {
    return (forwardIndex + 1) % 4;
}

inline size_t BackwardIndex(size_t forwardIndex) {
    return (forwardIndex + 2) % 4;
}

///////////////////////
//  CLASS FUNCTIONS  //
///////////////////////

rtm::ControlUnit::ControlUnit()
    : type_{ NoControlUnit }
    , column_{ 0 }
    , row_{ 0 }
    , linesCounts_{ 0, 0, 0, 0 }
    , nullDirection_{ NullAngle }
    , signals_{ DEFAULT_CROSSROAD_SIGNALS }
    , sprites_{ DEFAULT_DIRECTIONS_SIGNAL_SPRITES }
    , time_{ 0.f }
    , state_{ 0 }
{}

rtm::ControlUnit::ControlUnit(ControlUnitType type, int column, int row, LinesCounts linesCounts)
    : type_{ type }
    , column_{ column }
    , row_{ row }
    , linesCounts_{ linesCounts }
    , nullDirection_{ NullAngle }
    , signals_{ DEFAULT_CROSSROAD_SIGNALS }
    , sprites_{ DEFAULT_DIRECTIONS_SIGNAL_SPRITES }
    , time_{ 0.f }
    , state_{ 0 }
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
    , sprites_{ DEFAULT_DIRECTIONS_SIGNAL_SPRITES }
    , time_{ 0.f }
    , state_{ 0 }
{
    InitSignals_();
}

void rtm::ControlUnit::Update(WorldController* const world)
{
    time_ += world->GetDeltaTime();
    switch (type_)
    {
    case rtm::ControlUnitNo1:
        if (state_ == 0 && time_ > 10.f) {
            UpdateSignal_(Upward, Upward, Warning);
            UpdateSignal_(Upward, LeftwardIndex(Upward), Warning);
            UpdateSignal_(Upward, RightwardIndex(Upward), Warning);
            IncState_();
        }
        else if (state_ == 1 && time_ > 1.f) {
            UpdateSignal_(Upward, Upward, Forbidden);
            UpdateSignal_(Upward, LeftwardIndex(Upward), Forbidden);
            UpdateSignal_(Upward, RightwardIndex(Upward), Forbidden);
            if (nullDirection_ != Left && linesCounts_[Right] > 0) {
                IncState_();
            }
            else {
                if (nullDirection_ != Up && linesCounts_[Down] > 0) {
                    SetState_(6);
                }
                else {
                    SetState_(10);
                }
            }
        }
        else if (state_ == 2 && time_ > 3.f) {
            UpdateSignal_(Rightward, Rightward, Warning);
            UpdateSignal_(Rightward, LeftwardIndex(Rightward), Warning);
            UpdateSignal_(Rightward, RightwardIndex(Rightward), Warning);
            IncState_();
        }
        else if (state_ == 3 && time_ > 1.f) {
            UpdateSignal_(Rightward, Rightward, Allowed);
            UpdateSignal_(Rightward, LeftwardIndex(Rightward), Allowed);
            UpdateSignal_(Rightward, RightwardIndex(Rightward), Allowed);
            IncState_();
        }
        else if (state_ == 4 && time_ > 10.f) {
            UpdateSignal_(Rightward, Rightward, Warning);
            UpdateSignal_(Rightward, LeftwardIndex(Rightward), Warning);
            UpdateSignal_(Rightward, RightwardIndex(Rightward), Warning);
            IncState_();
        }
        else if (state_ == 5 && time_ > 1.f) {
            UpdateSignal_(Rightward, Rightward, Forbidden);
            UpdateSignal_(Rightward, LeftwardIndex(Rightward), Forbidden);
            UpdateSignal_(Rightward, RightwardIndex(Rightward), Forbidden);
            if (nullDirection_ != Up && linesCounts_[Down] > 0) {
                IncState_();
            }
            else {
                if (nullDirection_ != Right && linesCounts_[Left] > 0) {
                    SetState_(10);
                }
                else {
                    SetState_(14);
                }
            }
        }
        else if (state_ == 6 && time_ > 3.f) {
            UpdateSignal_(Downward, Downward, Warning);
            UpdateSignal_(Downward, LeftwardIndex(Downward), Warning);
            UpdateSignal_(Downward, RightwardIndex(Downward), Warning);
            IncState_();
        }
        else if (state_ == 7 && time_ > 1.f) {
            UpdateSignal_(Downward, Downward, Allowed);
            UpdateSignal_(Downward, LeftwardIndex(Downward), Allowed);
            UpdateSignal_(Downward, RightwardIndex(Downward), Allowed);
            IncState_();
        }
        else if (state_ == 8 && time_ > 10.f) {
            UpdateSignal_(Downward, Downward, Warning);
            UpdateSignal_(Downward, LeftwardIndex(Downward), Warning);
            UpdateSignal_(Downward, RightwardIndex(Downward), Warning);
            IncState_();
        }
        else if (state_ == 9 && time_ > 1.f) {
            UpdateSignal_(Downward, Downward, Forbidden);
            UpdateSignal_(Downward, LeftwardIndex(Downward), Forbidden);
            UpdateSignal_(Downward, RightwardIndex(Downward), Forbidden);
            if (nullDirection_ != Right && linesCounts_[Left] > 0) {
                IncState_();
            }
            else {
                if (nullDirection_ != Down && linesCounts_[Up] > 0) {
                    SetState_(14);
                }
                else {
                    SetState_(2);
                }
            }
        }
        else if (state_ == 10 && time_ > 3.f) {
            UpdateSignal_(Leftward, Leftward, Warning);
            UpdateSignal_(Leftward, LeftwardIndex(Leftward), Warning);
            UpdateSignal_(Leftward, RightwardIndex(Leftward), Warning);
            IncState_();
        }
        else if (state_ == 11 && time_ > 1.f) {
            UpdateSignal_(Leftward, Leftward, Allowed);
            UpdateSignal_(Leftward, LeftwardIndex(Leftward), Allowed);
            UpdateSignal_(Leftward, RightwardIndex(Leftward), Allowed);
            IncState_();
        }
        else if (state_ == 12 && time_ > 10.f) {
            UpdateSignal_(Leftward, Leftward, Warning);
            UpdateSignal_(Leftward, LeftwardIndex(Leftward), Warning);
            UpdateSignal_(Leftward, RightwardIndex(Leftward), Warning);
            IncState_();
        }
        else if (state_ == 13 && time_ > 1.f) {
            UpdateSignal_(Leftward, Leftward, Forbidden);
            UpdateSignal_(Leftward, LeftwardIndex(Leftward), Forbidden);
            UpdateSignal_(Leftward, RightwardIndex(Leftward), Forbidden);
            if (nullDirection_ != Down && linesCounts_[Up] > 0) {
                IncState_();
            }
            else {
                if (nullDirection_ != Left && linesCounts_[Right] > 0) {
                    SetState_(2);
                }
                else {
                    SetState_(6);
                }
            }
        }
        else if (state_ == 14 && time_ > 3.f) {
            UpdateSignal_(Upward, Upward, Warning);
            UpdateSignal_(Upward, LeftwardIndex(Upward), Warning);
            UpdateSignal_(Upward, RightwardIndex(Upward), Warning);
            IncState_();
        }
        else if (state_ == 15 && time_ > 1.f) {
            UpdateSignal_(Upward, Upward, Allowed);
            UpdateSignal_(Upward, LeftwardIndex(Upward), Allowed);
            UpdateSignal_(Upward, RightwardIndex(Upward), Allowed);
            ResetState_();
        }
        else
            ResetSprites_();
        break;
    }
}

rtm::ControlUnit::operator bool() const
{
    return type_ != NoControlUnit;
}

rtm::SignalType rtm::ControlUnit::GetSignal(DirectionType from, DirectionType to) const
{
    return signals_[from][to];
}

void rtm::ControlUnit::InitSignals_()
{
    for (size_t i{ 0 }; i < 4; ++i) {
        // Close directions to null direction
        if (nullDirection_ != NullDirection) {
            signals_[i][nullDirection_] = Closed;
        }
        // Close empty directions (without lines)
        if (linesCounts_[i] == 0) {
            for (size_t j{ 0 }; j < 4; ++j) {
                signals_[j][i] = Closed;
            }
        }
    }

    switch (type_)
    {
    case NoControlUnit:
        // Close back turn
        for (size_t i{ 0 }; i < 4; ++i) {
            signals_[i][BackwardIndex(i)] = Closed;
        }
        break;
    case ControlUnitNo1:
        // Close back turn
        for (size_t i{ 0 }; i < 4; ++i) {
            signals_[i][BackwardIndex(i)] = Closed;
        }
        DirectionType firstDirection{ NullDirection };
        if (nullDirection_ != Down && linesCounts_[Up] > 0) {
            firstDirection = Upward;
            SetState_(0);
        }
        else {
            if (nullDirection_ != Left && linesCounts_[Right] > 0) {
                firstDirection = Rightward;
                SetState_(4);
            }
            else {
                firstDirection = Leftward;
                SetState_(8);
            }
        }
        // Set "green lines"
        UpdateSignal_(firstDirection, firstDirection, Allowed);
        UpdateSignal_(firstDirection, LeftwardIndex(firstDirection), Allowed);
        UpdateSignal_(firstDirection, RightwardIndex(firstDirection), Allowed);
        // Set "red lines"
        for (size_t i{ RightwardIndex(firstDirection) }; i != firstDirection; i = RightwardIndex(i)) {
            UpdateSignal_(i, i, Forbidden);
            UpdateSignal_(i, LeftwardIndex(i), Forbidden);
            UpdateSignal_(i, RightwardIndex(i), Forbidden);
        }
        break;
    }
}

void rtm::ControlUnit::ShowSprites(cocos2d::Layer* const scene)
{
    for (size_t i = 0; i < 4; ++i) {
        // If from TCrossroad's nullDirection
        if (BackwardIndex(i) == nullDirection_) {
            sprites_[i][ForwardSignalIndex] = { nullptr, nullptr, nullptr, nullptr, nullptr };
            sprites_[i][LeftwardSignalIndex] = { nullptr, nullptr, nullptr, nullptr, nullptr };
            sprites_[i][RightwardSignalIndex] = { nullptr, nullptr, nullptr, nullptr, nullptr };
            continue;
        }

        bool hasSprites{ false };

        // Forward sprites
        if (signals_[i][i] == Closed || signals_[i][i] == NotWorking) {
            sprites_[i][ForwardSignalIndex] = { nullptr, nullptr, nullptr, nullptr, nullptr };
        }
        else for (size_t k = 0; k < 5; ++k) {
            sprites_[i][ForwardSignalIndex][k] = cocos2d::Sprite::create(GetSignalFile_(ForwardSignalId + k));
            hasSprites = true;
        }

        // Leftward sprites
        if (signals_[i][LeftwardIndex(i)] == Closed || signals_[i][LeftwardIndex(i)] == NotWorking) {
            sprites_[i][LeftwardSignalIndex] = { nullptr, nullptr, nullptr, nullptr, nullptr };
        }
        else for (size_t k = 0; k < 5; ++k) {
            sprites_[i][LeftwardSignalIndex][k] = cocos2d::Sprite::create(GetSignalFile_(LeftwardSignalId + k));
            hasSprites = true;
        }

        // Rightward sprites
        if (signals_[i][RightwardIndex(i)] == Closed || signals_[i][RightwardIndex(i)] == NotWorking) {
            sprites_[i][RightwardSignalIndex] = { nullptr, nullptr, nullptr, nullptr, nullptr };
        }
        else for (size_t k = 0; k < 5; ++k) {
            sprites_[i][RightwardSignalIndex][k] = cocos2d::Sprite::create(GetSignalFile_(RightwardSignalId + k));
            hasSprites = true;
        }

        if (!hasSprites) {
            continue;
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

        // Set position
        for (size_t l = 0; l < 3; ++l) {
            int zOrder{ FORWARD_SIGNAL_Z_ORDER };
            if (zOrder == LeftwardSignalIndex) {
                zOrder = LEFTWARD_SIGNAL_Z_ORDER;
            }
            else if (zOrder == RightwardSignalIndex) {
                zOrder = RIGHTWARD_SIGNAL_Z_ORDER;
            }

            for (size_t k = 0; k < 5; ++k) {
                if (sprites_[i][l][k] != nullptr) {
                    scene->addChild(sprites_[i][l][k], zOrder);
                    sprites_[i][l][k]->setAnchorPoint(cocos2d::Vec2{ 0.5, 0.5 });
                    sprites_[i][l][k]->setPosition(cocos2d::Vec2{ x, y });
                    sprites_[i][l][k]->setRotation(angle);
                }
            }
        }
    }

    ResetSprites_();
}

void rtm::ControlUnit::ReleaseSprites(cocos2d::Layer* const scene)
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
            if (sprites_[i][ForwardSignalIndex][k] != nullptr) {
                sprites_[i][ForwardSignalIndex][k]->setVisible(signals_[i][i] == k);
            }
            // Left
            if (sprites_[i][LeftwardSignalIndex][k] != nullptr) {
                sprites_[i][LeftwardSignalIndex][k]->setVisible(signals_[i][LeftwardIndex(i)] == k);
            }
            // Right            
            if (sprites_[i][RightwardSignalIndex][k] != nullptr) {
                sprites_[i][RightwardSignalIndex][k]->setVisible(signals_[i][RightwardIndex(i)] == k);
            }
        }
    }
}

void rtm::ControlUnit::UpdateSignal_(size_t i, size_t j, SignalType signal)
{
    if (signals_[i][j] != Closed) {
        signals_[i][j] = signal;
    }
}

void rtm::ControlUnit::IncState_()
{
    ++state_;
    time_ = 0.f;
}

void rtm::ControlUnit::SetState_(size_t state)
{
    state_ = state;
    time_ = 0.f;
}

void rtm::ControlUnit::ResetState_()
{
    SetState_(0);
}

std::string rtm::ControlUnit::GetSignalFile_(size_t id)
{
    std::string filename{ SIGNAL_FILENAME_MASK };
    auto it{ filename.find("%No%") };
    filename.replace(it, 4, std::to_string(static_cast<int>(id)));

    return filename;
}
