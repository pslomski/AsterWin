#include "MenuState.hpp"
#include <wingdi.h>
#include "HighScoreState.hpp"
#include "OptionsState.hpp"
#include "PlayState.hpp"
#include "StateManager.hpp"
#include "game/Consts.hpp"
#include "game/World.hpp"
#include "log/Log.hpp"

namespace ui
{
CMenuState::CMenuState(StateManager* pManager) : State(pManager)
{
    font.createFont(20, FW_NORMAL);
    fontSmall = new Font;
    fontSmall->createFont(15, FW_NORMAL);
    fontLarge = new Font;
    fontLarge->createFont(40, FW_NORMAL);
    fontSmall2 = new Font;
    fontSmall2->createFont(10, FW_NORMAL);

    int dy = int(1.0 / 12.0 * geWorld.scrHeight);
    int left = int(1.0 / 4.0 * geWorld.scrWidth);
    int right = int(3.0 / 4.0 * geWorld.scrWidth);
    int top = int(1.0 / 4.5 * geWorld.scrHeight);
    int bottom = top + dy;

    titleText = new TextControl(fontLarge, ui::Rectanglei(0, top, 0, geWorld.scrWidth));
    titleText->setAlignement(TextControl::TextAlignement::center);
    titleText->setText("Asteroids 2010");

    newGameText = new TextControl(&font, ui::Rectanglei(top, bottom, left, right));
    newGameText->setAlignement(TextControl::TextAlignement::center);
    newGameText->setText("New game");

    top += dy;
    bottom += dy;
    resumeGameText = new TextControl(&font, ui::Rectanglei(top, bottom, left, right));
    resumeGameText->setAlignement(TextControl::TextAlignement::center);
    resumeGameText->setText("Resume game");

    top += dy;
    bottom += dy;
    optionsText = new TextControl(&font, ui::Rectanglei(top, bottom, left, right));
    optionsText->setAlignement(TextControl::TextAlignement::center);
    optionsText->setText("Settings");

    top += dy;
    bottom += dy;
    scoresText = new TextControl(&font, ui::Rectanglei(top, bottom, left, right));
    scoresText->setAlignement(TextControl::TextAlignement::center);
    scoresText->setText("High scores");

    top += dy;
    bottom += dy;
    exitText = new TextControl(&font, ui::Rectanglei(top, bottom, left, right));
    exitText->setAlignement(TextControl::TextAlignement::center);
    exitText->setText("Exit");
}

CMenuState::~CMenuState()
{
    delete fontSmall;
    delete fontSmall2;
    delete fontLarge;
    delete titleText;
    delete newGameText;
    delete resumeGameText;
    delete scoresText;
    delete exitText;
    delete optionsText;
}

CMenuState* CMenuState::getInstance(StateManager* pManager)
{
    static CMenuState Instance(pManager);
    return &Instance;
}

void CMenuState::onKeyDown(WPARAM wKey)
{
    switch (wKey)
    {
        case VK_DOWN:
            selectionDown();
            break;
        case VK_UP:
            selectionUp();
            break;
        case VK_RETURN:
            selectionChosen();
            break;
        case VK_ESCAPE:
            exitGame();
            break;
    }
}

void CMenuState::update(double timeStep)
{
    TextControl* pTxtCtrl = getTextControl(currentSelection);
    if (pTxtCtrl) pTxtCtrl->update(timeStep);
}

void CMenuState::draw()
{
    glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
    glLoadIdentity(); // Reset The Projection Matrix
    glOrtho(0, geWorld.scrWidth, geWorld.scrHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
    glLoadIdentity(); // Reset The Modelview Matrix

    titleText->draw();
    newGameText->draw();
    resumeGameText->draw();
    optionsText->draw();
    scoresText->draw();
    exitText->draw();

    TextControl txtControls(
        fontSmall2,
        ui::Rectanglei(int(0.7 * geWorld.scrHeight), int(0.7 * geWorld.scrHeight + 20), 0, geWorld.scrWidth));
    txtControls.setAlignement(TextControl::TextAlignement::center);
    txtControls.setTextColor(0.8f, 0.8f, 0.8f);
    txtControls.setText("Controls:");
    txtControls.draw();
    txtControls.position.offsetRect(0, 20);
    txtControls.setText("left arrow - turn left, right arrow - turn right");
    txtControls.draw();
    txtControls.position.offsetRect(0, 20);
    txtControls.setText("up arrow - forward, space - fire");
    txtControls.draw();

    TextControl line(fontSmall, ui::Rectanglei(geWorld.scrHeight - 100, geWorld.scrHeight - 50, 0, geWorld.scrWidth));
    line.setAlignement(TextControl::TextAlignement::center);
    line.setTextColor(0.7f, 0.7f, 0.7f);
    line.setText("Asteroids remake by Piotr Slomski");
    line.draw();
}

void CMenuState::enterState()
{
    LOG_INF("Entering MenuState");
    geWorld.isGameRunning = false;
    // Checks whether there is a current game active
    setBlinkText(currentSelection, false);
    if (!currentGame or currentGame->isGameOver())
    {
        if (1 == currentSelection) currentSelection = 0;
        resumeGameText->setTextColor(0.3f, 0.3f, 0.3f);
    }
    else
    {
        resumeGameText->setTextColor(1.0f, 1.0f, 1.0f);
    }
    setBlinkText(currentSelection, true);
}

TextControl* CMenuState::getTextControl(int id)
{
    switch (id)
    {
        case 0:
            return newGameText;
        case 1:
            return resumeGameText;
        case 2:
            return optionsText;
        case 3:
            return scoresText;
        case 4:
            return exitText;
        default:
            return nullptr;
    }
}

void CMenuState::setBlinkText(int id, bool isBlink)
{
    TextControl* textCtrl = getTextControl(currentSelection);
    if (textCtrl) textCtrl->setBlink(isBlink);
}

void CMenuState::selectionUp()
{
    setBlinkText(currentSelection, false);
    currentSelection--;
    if (currentSelection == -1) currentSelection = 4;

    // If there is no current game, we should skip
    // the "Resume game" item.
    if (currentSelection == 1)
    {
        if (!currentGame || currentGame->isGameOver()) currentSelection--;
    }
    setBlinkText(currentSelection, true);
}

void CMenuState::selectionDown()
{
    setBlinkText(currentSelection, false);
    currentSelection++;
    if (currentSelection == 5) currentSelection = 0;

    // If there is no current game, we should skip
    // the "Resume game" item.
    if (currentSelection == 1)
    {
        if (!currentGame || currentGame->isGameOver()) currentSelection++;
    }
    setBlinkText(currentSelection, true);
}

void CMenuState::selectionChosen()
{
    switch (currentSelection)
    {
        case 0:
            if (!currentGame) currentGame = PlayState::getInstance(stateManager);
            currentGame->reset();
            setBlinkText(currentSelection, false);
            currentSelection = 1;
            setBlinkText(currentSelection, true);
            changeState(currentGame);
            break;

        case 1:
            if (currentGame && !currentGame->isGameOver()) changeState(currentGame);
            break;

        case 2:
            changeState(OptionsState::GetInstance(stateManager));
            break;

        case 3:
            changeState(HighScoreState::getInstance(stateManager));
            break;

        case 4:
            PostQuitMessage(0);
            break;
    }
}

void CMenuState::exitGame()
{
    PostQuitMessage(0);
}

} // namespace ui
