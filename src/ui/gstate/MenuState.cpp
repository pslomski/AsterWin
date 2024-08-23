#include "MenuState.hpp"
#include "HighScoreState.hpp"
#include "OptionsState.hpp"
#include "PlayState.hpp"
#include "StateManager.hpp"
#include "game/Consts.hpp"
#include "game/World.hpp"

namespace ui
{
CMenuState::CMenuState(StateManager* pManager)
    : State(pManager), m_pFont(nullptr), m_iCurrentSelection(0), m_pCurrentGame(nullptr)
{
    m_pFont = new Font;
    m_pFont->createFont(20, FW_NORMAL);
    m_pFontSmall = new Font;
    m_pFontSmall->createFont(15, FW_NORMAL);
    m_pFontLarge = new Font;
    m_pFontLarge->createFont(40, FW_NORMAL);
    m_pFontSmall2 = new Font;
    m_pFontSmall2->createFont(10, FW_NORMAL);

    int dy = int(1.0 / 12.0 * geWorld.scrHeight);
    int left = int(1.0 / 4.0 * geWorld.scrWidth);
    int right = int(3.0 / 4.0 * geWorld.scrWidth);
    int top = int(1.0 / 4.5 * geWorld.scrHeight);
    int bottom = top + dy;

    m_pTitleText = new TextControl(m_pFontLarge, ui::Rectanglei(0, top, 0, geWorld.scrWidth));
    m_pTitleText->setAlignement(TextControl::TextAlignement::center);
    m_pTitleText->setText("Asteroids 2010");

    m_pNewGameText = new TextControl(m_pFont, ui::Rectanglei(top, bottom, left, right));
    m_pNewGameText->setAlignement(TextControl::TextAlignement::center);
    m_pNewGameText->setText("New game");

    top += dy;
    bottom += dy;
    m_pResumeGameText = new TextControl(m_pFont, ui::Rectanglei(top, bottom, left, right));
    m_pResumeGameText->setAlignement(TextControl::TextAlignement::center);
    m_pResumeGameText->setText("Resume game");

    top += dy;
    bottom += dy;
    m_pOptionsText = new TextControl(m_pFont, ui::Rectanglei(top, bottom, left, right));
    m_pOptionsText->setAlignement(TextControl::TextAlignement::center);
    m_pOptionsText->setText("Settings");

    top += dy;
    bottom += dy;
    m_pScoresText = new TextControl(m_pFont, ui::Rectanglei(top, bottom, left, right));
    m_pScoresText->setAlignement(TextControl::TextAlignement::center);
    m_pScoresText->setText("High scores");

    top += dy;
    bottom += dy;
    m_pExitText = new TextControl(m_pFont, ui::Rectanglei(top, bottom, left, right));
    m_pExitText->setAlignement(TextControl::TextAlignement::center);
    m_pExitText->setText("Exit");
}

CMenuState::~CMenuState()
{
    delete m_pFont;
    delete m_pFontSmall;
    delete m_pFontSmall2;
    delete m_pFontLarge;
    delete m_pNewGameText;
    delete m_pResumeGameText;
    delete m_pScoresText;
    delete m_pExitText;
    delete m_pOptionsText;
}

CMenuState* CMenuState::GetInstance(StateManager* pManager)
{
    static CMenuState Instance(pManager);
    return &Instance;
}

void CMenuState::onKeyDown(WPARAM wKey)
{
    switch (wKey)
    {
        case VK_DOWN:
            SelectionDown();
            break;
        case VK_UP:
            SelectionUp();
            break;
        case VK_RETURN:
            SelectionChosen();
            break;
        case VK_ESCAPE:
            ExitGame();
            break;
    }
}

void CMenuState::update(double timeStep)
{
    TextControl* pTxtCtrl = GetTextControl(m_iCurrentSelection);
    if (pTxtCtrl) pTxtCtrl->update(timeStep);
}

void CMenuState::draw()
{
    glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
    glLoadIdentity(); // Reset The Projection Matrix
    glOrtho(0, geWorld.scrWidth, geWorld.scrHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
    glLoadIdentity(); // Reset The Modelview Matrix

    m_pTitleText->draw();
    m_pNewGameText->draw();
    m_pResumeGameText->draw();
    m_pOptionsText->draw();
    m_pScoresText->draw();
    m_pExitText->draw();

    TextControl txtControls(
        m_pFontSmall2,
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

    TextControl line(
        m_pFontSmall, ui::Rectanglei(geWorld.scrHeight - 100, geWorld.scrHeight - 50, 0, geWorld.scrWidth));
    line.setAlignement(TextControl::TextAlignement::center);
    line.setTextColor(0.7f, 0.7f, 0.7f);
    line.setText("Asteroids remake by Piotr Slomski");
    line.draw();
}

void CMenuState::enterState()
{
    geWorld.isGameRunning = false;
    // Checks whether there is a current game active
    SetBlinkText(m_iCurrentSelection, false);
    if (!m_pCurrentGame or m_pCurrentGame->isGameOver())
    {
        if (1 == m_iCurrentSelection) m_iCurrentSelection = 0;
        m_pResumeGameText->setTextColor(0.3f, 0.3f, 0.3f);
    }
    else
    {
        m_pResumeGameText->setTextColor(1.0f, 1.0f, 1.0f);
    }
    SetBlinkText(m_iCurrentSelection, true);
}

TextControl* CMenuState::GetTextControl(int id)
{
    switch (id)
    {
        case 0:
            return m_pNewGameText;
        case 1:
            return m_pResumeGameText;
        case 2:
            return m_pOptionsText;
        case 3:
            return m_pScoresText;
        case 4:
            return m_pExitText;
        default:
            return nullptr;
    }
}

void CMenuState::SetBlinkText(int id, bool in_bBlink)
{
    TextControl* pTxtCtrl = GetTextControl(m_iCurrentSelection);
    if (pTxtCtrl) pTxtCtrl->setBlink(in_bBlink);
}

void CMenuState::SelectionUp()
{
    SetBlinkText(m_iCurrentSelection, false);
    m_iCurrentSelection--;
    if (m_iCurrentSelection == -1) m_iCurrentSelection = 4;

    // If there is no current game, we should skip
    // the "Resume game" item.
    if (m_iCurrentSelection == 1)
    {
        if (!m_pCurrentGame || m_pCurrentGame->isGameOver()) m_iCurrentSelection--;
    }
    SetBlinkText(m_iCurrentSelection, true);
}

void CMenuState::SelectionDown()
{
    SetBlinkText(m_iCurrentSelection, false);
    m_iCurrentSelection++;
    if (m_iCurrentSelection == 5) m_iCurrentSelection = 0;

    // If there is no current game, we should skip
    // the "Resume game" item.
    if (m_iCurrentSelection == 1)
    {
        if (!m_pCurrentGame || m_pCurrentGame->isGameOver()) m_iCurrentSelection++;
    }
    SetBlinkText(m_iCurrentSelection, true);
}

void CMenuState::SelectionChosen()
{
    switch (m_iCurrentSelection)
    {
        case 0:
            if (!m_pCurrentGame) m_pCurrentGame = PlayState::getInstance(stateManager);
            m_pCurrentGame->reset();
            SetBlinkText(m_iCurrentSelection, false);
            m_iCurrentSelection = 1;
            SetBlinkText(m_iCurrentSelection, true);
            changeState(m_pCurrentGame);
            break;

        case 1:
            if (m_pCurrentGame && !m_pCurrentGame->isGameOver()) changeState(m_pCurrentGame);
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

void CMenuState::ExitGame()
{
    PostQuitMessage(0);
}

} // namespace ui
