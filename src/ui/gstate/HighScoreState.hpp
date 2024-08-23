#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "State.hpp"
#include "ui/Font.hpp"
#include "ui/Rectangle.hpp"
#include "ui/TextControl.hpp"

namespace ui
{
class HighScoreState : public State
{
public:
    ~HighScoreState();

    void setNewHighScore(uint32_t NewHighScore) { newHighScore = NewHighScore; }

    void onKeyDown(WPARAM wKey) override;
    void onChar(WPARAM wChar) override;
    void draw() override;
    void enterState() override;

    static HighScoreState* getInstance(StateManager* pManager);

protected:
    HighScoreState(StateManager* pManager);

private:
    void saveScores();
    void addNewScore(const std::string& strName, uint32_t ulScore);

    struct HighScore
    {
        bool operator<(const HighScore& other) { return score > other.score; }

        std::string playerName{};
        uint32_t score{};
    };
    using HighScores = std::vector<HighScore>;

    uint32_t newHighScore{};
    bool isEnterName{false};
    char currentName[26];
    int nameIndex{};
    Font* font{};
    Font* fontSmall{};
    HighScores highScores{10};
    TextControl* highScore;
    ui::Rectanglei highScoreRect;
    ui::Rectanglei entriesRect;
};
} // namespace ui
