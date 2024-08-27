#include "HighScoreState.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include "MenuState.hpp"
#include "game/Consts.hpp"
#include "game/World.hpp"

namespace ui
{

HighScoreState::HighScoreState(StateManager* pManager) : State(pManager)
{
    font = new Font;
    font->createFont(20, FW_NORMAL);
    currentName[0] = '\0';

    fontSmall = new Font;
    fontSmall->createFont(15, FW_NORMAL);

    int dy = 20;
    int left = int(1.0 / 4.0 * geWorld.scrWidth);
    int right = int(3.0 / 4.0 * geWorld.scrWidth);
    int top = 50;
    int bottom = top + dy;
    highScore = new TextControl(font, ui::Rectanglei(top, bottom, left, right));
    highScore->setAlignement(TextControl::TextAlignement::center);
    highScore->setText("High Scores");

    int Marg = 10;
    highScoreRect.top = 550;
    highScoreRect.bottom = 600;
    highScoreRect.left = left - Marg;
    highScoreRect.right = right + Marg;

    entriesRect.top = 135;
    entriesRect.bottom = entriesRect.top + 35;
    entriesRect.left = left / 2;
    entriesRect.right = right + left / 2;
}

HighScoreState::~HighScoreState()
{
    delete highScore;
    delete font;
    delete fontSmall;
}

HighScoreState* HighScoreState::getInstance(StateManager* pManager)
{
    static HighScoreState Instance(pManager);
    return &Instance;
}

void HighScoreState::onKeyDown(WPARAM wKey)
{
    if (isEnterName)
    {
        switch (wKey)
        {
            case VK_RETURN:
                if (!std::string(currentName).empty())
                {
                    addNewScore(currentName, newHighScore);
                    newHighScore = 0;
                    isEnterName = false;
                    nameIndex = 0;
                    currentName[0] = '\0';
                }
                break;
            case VK_BACK:
                if (nameIndex > 0)
                {
                    nameIndex--;
                    currentName[nameIndex] = '\0';
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
                changeState(CMenuState::getInstance(stateManager));
                break;
        }
    }
}

void HighScoreState::onChar(WPARAM wChar)
{
    if (isEnterName && (nameIndex < 25))
    {
        // Filter the characters for only alphabetical characters.
        if ((wChar >= 64 && wChar <= 91) || (wChar >= 97 && wChar <= 122))
        {
            currentName[nameIndex] = wChar;
            nameIndex++;
            currentName[nameIndex] = '\0';
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

    highScore->draw();
    ui::Rectanglei rcNum = entriesRect;
    rcNum.right = entriesRect.left + 40;
    ui::Rectanglei rcTxt = entriesRect;
    rcTxt.left = entriesRect.left + 60;
    int iCount = 1;
    char buf[256];
    for (auto iter = highScores.begin(); iter != highScores.end(); iter++)
    {
        _itoa_s(iCount, buf, 10);
        TextControl txtEntryN(font, rcNum);
        txtEntryN.setAlignement(TextControl::TextAlignement::right);
        txtEntryN.setText(buf);
        txtEntryN.draw();

        TextControl txtEntry(font, rcTxt);
        txtEntry.setAlignement(TextControl::TextAlignement::left);
        txtEntry.setText(iter->playerName);
        txtEntry.draw();

        std::stringstream ssScore;
        ssScore << iter->score;
        txtEntry.setAlignement(TextControl::TextAlignement::right);
        txtEntry.setText(ssScore.str());
        txtEntry.draw();

        rcNum.offsetRect(0, 35);
        rcTxt.offsetRect(0, 35);
        ++iCount;
    }

    if (isEnterName)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_LOOP);
        glVertex2i(highScoreRect.left, highScoreRect.bottom);
        glVertex2i(highScoreRect.right, highScoreRect.bottom);
        glVertex2i(highScoreRect.right, highScoreRect.top);
        glVertex2i(highScoreRect.left, highScoreRect.top);
        glEnd();

        ui::Rectanglei rc(highScoreRect);
        rc.offsetRect(0, -50);
        TextControl txtEnterName(font, rc);
        txtEnterName.setAlignement(TextControl::TextAlignement::center);
        txtEnterName.setText("Enter Your name:");
        txtEnterName.draw();

        TextControl txtEntry(font, highScoreRect);
        txtEntry.setAlignement(TextControl::TextAlignement::center);
        txtEntry.setText(currentName);
        txtEntry.draw();
    }
    else
    {
        ui::Rectanglei rc(geWorld.scrHeight - 100, geWorld.scrHeight - 50, 0, geWorld.scrWidth);
        TextControl txtEnterName(fontSmall, rc);
        txtEnterName.setAlignement(TextControl::TextAlignement::center);
        txtEnterName.setText("Press Enter");
        txtEnterName.draw();
    }
}

void HighScoreState::enterState()
{
    geWorld.isGameRunning = false;
    highScores.clear();
    std::ifstream inputFile("HighScores.txt");
    if (inputFile.fail())
    {
        if (newHighScore) isEnterName = true;
        return;
    }

    // Read all entries from the file
    std::string line;
    common::HighScore newScore;
    std::basic_string<char>::size_type idx;
    while (!inputFile.eof())
    {
        getline(inputFile, line);
        if (line.empty()) continue;
        idx = line.find(";");
        if (idx == -1) continue;
        newScore.playerName = line.substr(0, idx); // strtok_s(buf, sep, &next_token1);
        newScore.score = atoi(line.substr(idx + 1).c_str());
        highScores.push_back(newScore);
    }
    while (highScores.size() < 10)
        highScores.push_back(common::HighScore{});

    sort(highScores.begin(), highScores.end());

    uint32_t lastScore = 0;
    if (highScores.size()) lastScore = highScores[highScores.size() - 1].score;
    if (newHighScore && newHighScore > lastScore) isEnterName = true;
}

void HighScoreState::saveScores()
{
    std::ofstream outputFile("HighScores.txt");
    if (outputFile.fail())
    {
        return;
    }
    for (const auto& highScore : highScores)
    {
        outputFile << highScore.playerName << ";" << highScore.score << '\n';
    }
}

void HighScoreState::addNewScore(const std::string& playerName, uint32_t score)
{
    common::HighScore newHighScore;
    newHighScore.playerName = playerName;
    newHighScore.score = score;
    highScores.push_back(newHighScore);

    sort(highScores.begin(), highScores.end());

    while (highScores.size() > 10)
        highScores.pop_back();

    saveScores();
}
} // namespace ui
