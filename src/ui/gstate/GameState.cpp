#include "GameState.hpp"
#include "HighScoreState.hpp"
#include "MenuState.hpp"
#include "StateManager.hpp"
#include "game/Consts.hpp"
#include "game/World.hpp"

namespace ui
{
#define FNTSIZESMALL 15
#define FNTSIZELARGE 25

GameState::GameState(StateManager* pManager) : State(pManager), m_pFontSmall(nullptr), m_pFontLarge(nullptr)
{
    m_bDispFPS = false;
    m_pFontSmall = new Font;
    m_pFontSmall->createFont(FNTSIZESMALL, FW_NORMAL);

    m_pFontLarge = new Font;
    m_pFontLarge->createFont(FNTSIZELARGE, FW_NORMAL);

    m_pGameOverText = new TextControl(m_pFontLarge, ui::Rectanglei(0, geWorld.scrHeight, 0, geWorld.scrWidth));
    m_pGameOverText->setAlignement(TextControl::TextAlignement::center);
    m_pGameOverText->setText("Game Over");
}

GameState::~GameState()
{
    delete m_pFontSmall;
    delete m_pFontLarge;
    delete m_pGameOverText;
}

GameState* GameState::GetInstance(StateManager* pManager)
{
    static GameState Instance(pManager);
    return &Instance;
}

void GameState::enterState()
{
    AsterGame.enterState();
    geWorld.isGameRunning = true;
    AsterGame.isMusic = geMusic.GetVolume() > 0.001;
    geSound.Unmute();
    if (geMusic.IsStarted()) geMusic.Play();
}

void GameState::leaveState()
{
    AsterGame.leaveState();
    geSound.Mute();
    if (AsterGame.isGameOver())
        geMusic.Stop();
    else
        geMusic.Pause();
}

void GameState::Reset()
{
    AsterGame.reset();
}

void GameState::onKeyDown(WPARAM wKey)
{
    AsterGame.key[wKey] = true;
    switch (wKey)
    {
        case 'F':
            m_bDispFPS = !m_bDispFPS;
        case VK_ESCAPE:
        case VK_RETURN:
            if (AsterGame.isGameOver())
            {
                CHighScoreState* pHighScores = CHighScoreState::GetInstance(stateManager);
                pHighScores->SetNewHighScore(AsterGame.scoreCounter.get());
                AsterGame.clear();
                changeState(pHighScores);
            }
            else
            {
                if (wKey == VK_ESCAPE) changeState(CMenuState::GetInstance(stateManager));
            }
            break;
    }
}

void GameState::onKeyUp(WPARAM wKey)
{
    AsterGame.key[wKey] = false;
}

void GameState::onResize(int cx, int cy) {}

void GameState::update(double timeStep)
{
    AsterGame.update();
}

void GameState::draw()
{
    AsterGame.draw();

    glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
    glLoadIdentity(); // Reset The Projection Matrix
    glOrtho(0, geWorld.scrWidth, geWorld.scrHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
    glLoadIdentity(); // Reset The Modelview Matrix

    glColor3f(1.0, 1.0, 1.0);
    GLint w = GLint(geWorld.scrWidth);
    GLint h = GLint(geWorld.scrHeight);
    GLint y = FNTSIZESMALL + 5;
    // glRasterPos2i(10, y);
    m_pFontSmall->drawTextFmt(10, y, "Level: %d", AsterGame.gameLevel);

    // glRasterPos2i(w / 2 - 80, y);
    m_pFontSmall->drawTextFmt(w / 2 - 80, y, "Score: %d", AsterGame.scoreCounter.get());

    // glRasterPos2i(w - 150, y);
    m_pFontSmall->drawTextFmt(w - 150, y, "Lives: %d", AsterGame.lives);

    if (m_bDispFPS)
    {
        // glRasterPos2i(w / 2 - 80, y + 20);
        m_pFontSmall->drawTextFmt(w / 2 - 80, y + 20, "FPS: %.0f", AsterGame.fps);
    }

    if (IsGameOver())
    {
        m_pGameOverText->draw();
    }
}
} // namespace ui
