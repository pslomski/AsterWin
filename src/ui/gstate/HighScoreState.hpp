#pragma once

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

    void SetNewHighScore(ULONG NewHighScore) { mNewHighScore = NewHighScore; }

    void onKeyDown(WPARAM wKey) override;
    void onChar(WPARAM wChar) override;
    void draw() override;
    void enterState() override;

    static HighScoreState* GetInstance(StateManager* pManager);

protected:
    HighScoreState(StateManager* pManager);

private:
    void SaveScores();
    void AddNewScore(const std::string& strName, ULONG ulScore);

    struct HighScore
    {
        std::string strPlayer;
        ULONG ulScore;
        HighScore() : strPlayer(), ulScore(0) {}
        bool operator<(const HighScore& other) { return ulScore > other.ulScore; }
    };
    using HighScores = std::vector<HighScore>;

    ULONG mNewHighScore;
    bool mEnterName;
    char mCurrentName[26];
    int mNameIndex;
    Font* mFont;
    Font* mFontSmall;
    HighScores mHighScores;
    TextControl* mHighScore;
    ui::Rectanglei mHighScoreRect;
    ui::Rectanglei mEntriesRect;
};
} // namespace ui
