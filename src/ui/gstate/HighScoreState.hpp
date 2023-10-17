#pragma once

#include <string>
#include <vector>
#include "State.hpp"
#include "ui/Font.hpp"
#include "ui/Rectangle.hpp"
#include "ui/TextControl.hpp"

namespace ui
{
class CHighScoreState : public State
{
public:
    ~CHighScoreState();

    void SetNewHighScore(ULONG NewHighScore) { mNewHighScore = NewHighScore; }

    void onKeyDown(WPARAM wKey) override;
    void onChar(WPARAM wChar) override;
    void draw() override;
    void enterState() override;

    static CHighScoreState* GetInstance(StateManager* pManager);

protected:
    CHighScoreState(StateManager* pManager);

private:
    void SaveScores();
    void AddNewScore(const std::string& strName, ULONG ulScore);

    struct HighScoreData
    {
        std::string strPlayer;
        ULONG ulScore;
        HighScoreData() : strPlayer(), ulScore(0) {}
        bool operator<(const HighScoreData& other) { return ulScore > other.ulScore; }
    };

    ULONG mNewHighScore;
    bool mEnterName;
    char mCurrentName[26];
    int mNameIndex;
    Font* mFont;
    Font* mFontSmall;
    typedef std::vector<HighScoreData> THighScoreTable;
    THighScoreTable mHighScores;
    TextControl* mHighScore;
    ui::Rectanglei mHighScoreRect;
    ui::Rectanglei mEntriesRect;
};
} // namespace ui
