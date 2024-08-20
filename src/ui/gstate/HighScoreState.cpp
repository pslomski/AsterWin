#include "HighScoreState.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include "MenuState.hpp"
#include "game/Consts.hpp"
#include "game/World.hpp"

namespace ui
{

HighScoreState::HighScoreState(StateManager* pManager)
    : State(pManager), mNewHighScore(0), mEnterName(false), mFont(nullptr), mNameIndex(0), mHighScores(10)
{
    mFont = new Font;
    mFont->createFont(20, FW_NORMAL);
    mCurrentName[0] = '\0';

    mFontSmall = new Font;
    mFontSmall->createFont(15, FW_NORMAL);

    int dy = 20;
    int left = int(1.0 / 4.0 * geWorld.scrWidth);
    int right = int(3.0 / 4.0 * geWorld.scrWidth);
    int top = 50;
    int bottom = top + dy;
    mHighScore = new TextControl(mFont, ui::Rectanglei(top, bottom, left, right));
    mHighScore->setAlignement(TextControl::TextAlignement::center);
    mHighScore->setText("High Scores");

    int Marg = 10;
    mHighScoreRect.top = 550;
    mHighScoreRect.bottom = 600;
    mHighScoreRect.left = left - Marg;
    mHighScoreRect.right = right + Marg;

    mEntriesRect.top = 135;
    mEntriesRect.bottom = mEntriesRect.top + 35;
    mEntriesRect.left = left / 2;
    mEntriesRect.right = right + left / 2;
}

HighScoreState::~HighScoreState()
{
    delete mHighScore;
    delete mFont;
    delete mFontSmall;
}

HighScoreState* HighScoreState::GetInstance(StateManager* pManager)
{
    static HighScoreState Instance(pManager);
    return &Instance;
}

void HighScoreState::onKeyDown(WPARAM wKey)
{
    if (mEnterName)
    {
        switch (wKey)
        {
                // In case of a return, the new score should be added.
            case VK_RETURN:
                if (!std::string(mCurrentName).empty())
                {
                    AddNewScore(mCurrentName, mNewHighScore);
                    mNewHighScore = 0;
                    mEnterName = false;
                    mNameIndex = 0;
                    mCurrentName[0] = '\0';
                }
                break;
            case VK_BACK:
                // Remove one character
                if (mNameIndex > 0)
                {
                    mNameIndex--;
                    mCurrentName[mNameIndex] = '\0';
                }
                break;
        }
    }
    else
    {
        switch (wKey)
        {
            case VK_ESCAPE:
            case VK_RETURN:
                changeState(CMenuState::GetInstance(stateManager));
                break;
        }
    }
}

void HighScoreState::onChar(WPARAM wChar)
{
    if (mEnterName && (mNameIndex < 25))
    {
        // Filter the characters for only alphabetical characters.
        if ((wChar >= 64 && wChar <= 91) || (wChar >= 97 && wChar <= 122))
        {
            mCurrentName[mNameIndex] = wChar;
            mNameIndex++;
            mCurrentName[mNameIndex] = '\0';
        }
    }
}

void HighScoreState::draw()
{
    glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
    glLoadIdentity(); // Reset The Projection Matrix
    glOrtho(0, geWorld.scrWidth, geWorld.scrHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
    glLoadIdentity(); // Reset The Modelview Matrix

    mHighScore->draw();
    ui::Rectanglei rcNum = mEntriesRect;
    rcNum.right = mEntriesRect.left + 40;
    ui::Rectanglei rcTxt = mEntriesRect;
    rcTxt.left = mEntriesRect.left + 60;
    int iCount = 1;
    char buf[256];
    for (auto iter = mHighScores.begin(); iter != mHighScores.end(); iter++)
    {
        _itoa_s(iCount, buf, 10);
        TextControl txtEntryN(mFont, rcNum);
        txtEntryN.setAlignement(TextControl::TextAlignement::right);
        txtEntryN.setText(buf);
        txtEntryN.draw();

        TextControl txtEntry(mFont, rcTxt);
        txtEntry.setAlignement(TextControl::TextAlignement::left);
        txtEntry.setText(iter->strPlayer);
        txtEntry.draw();

        std::stringstream ssScore;
        ssScore << iter->ulScore;
        txtEntry.setAlignement(TextControl::TextAlignement::right);
        txtEntry.setText(ssScore.str());
        txtEntry.draw();

        rcNum.offsetRect(0, 35);
        rcTxt.offsetRect(0, 35);
        ++iCount;
    }

    if (mEnterName)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_LOOP);
        glVertex2i(mHighScoreRect.left, mHighScoreRect.bottom);
        glVertex2i(mHighScoreRect.right, mHighScoreRect.bottom);
        glVertex2i(mHighScoreRect.right, mHighScoreRect.top);
        glVertex2i(mHighScoreRect.left, mHighScoreRect.top);
        glEnd();

        ui::Rectanglei rc(mHighScoreRect);
        rc.offsetRect(0, -50);
        TextControl txtEnterName(mFont, rc);
        txtEnterName.setAlignement(TextControl::TextAlignement::center);
        txtEnterName.setText("Enter Your name:");
        txtEnterName.draw();

        TextControl txtEntry(mFont, mHighScoreRect);
        txtEntry.setAlignement(TextControl::TextAlignement::center);
        txtEntry.setText(mCurrentName);
        txtEntry.draw();
    }
    else
    {
        ui::Rectanglei rc(geWorld.scrHeight - 100, geWorld.scrHeight - 50, 0, geWorld.scrWidth);
        TextControl txtEnterName(mFontSmall, rc);
        txtEnterName.setAlignement(TextControl::TextAlignement::center);
        txtEnterName.setText("Press Enter");
        txtEnterName.draw();
    }
}

void HighScoreState::enterState()
{
    geWorld.isGameRunning = false;
    mHighScores.clear();
    std::ifstream inputFile("HighScores.txt");
    if (inputFile.fail())
    {
        if (mNewHighScore) mEnterName = true;
        return;
    }

    // Read all entries from the file
    std::string line;
    HighScore newScore;
    std::basic_string<char>::size_type idx;
    while (!inputFile.eof())
    {
        getline(inputFile, line);
        if (line.empty()) continue;
        idx = line.find(";");
        if (idx == -1) continue;
        newScore.strPlayer = line.substr(0, idx); // strtok_s(buf, sep, &next_token1);
        newScore.ulScore = atoi(line.substr(idx + 1).c_str());
        mHighScores.push_back(newScore);
    }
    while (mHighScores.size() < 10)
        mHighScores.push_back(HighScore());

    // Sort the table
    sort(mHighScores.begin(), mHighScores.end());

    // Check if we have a new high-score that should be
    // added in the table. If yes, m_bEnterName is set
    // to true.
    ULONG lastScore = 0;
    if (mHighScores.size()) lastScore = mHighScores[mHighScores.size() - 1].ulScore;
    if (mNewHighScore && mNewHighScore > lastScore) mEnterName = true;
}

void HighScoreState::SaveScores()
{
    std::ofstream outputFile("HighScores.txt");
    if (outputFile.fail())
    {
        return;
    }
    for (auto iter = mHighScores.begin(); iter != mHighScores.end(); iter++)
    {
        outputFile << iter->strPlayer << ";" << iter->ulScore << '\n';
    }
}

void HighScoreState::AddNewScore(const std::string& strName, ULONG ulScore)
{
    // Create a new high-score and push it into the table
    HighScore newData;
    newData.strPlayer = strName;
    newData.ulScore = ulScore;
    mHighScores.push_back(newData);

    sort(mHighScores.begin(), mHighScores.end());

    // If too much elements, remove the last one.
    while (mHighScores.size() > 10)
        mHighScores.pop_back();

    SaveScores();
}
} // namespace ui
