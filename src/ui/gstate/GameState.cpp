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

GameState::GameState(StateManager* pManager) : State(pManager), fontSmall(nullptr), fontLarge(nullptr)
{
    isDisplayFps = false;
    fontSmall = new Font;
    fontSmall->createFont(FNTSIZESMALL, FW_NORMAL);

    fontLarge = new Font;
    fontLarge->createFont(FNTSIZELARGE, FW_NORMAL);

    textGameOver = new TextControl(fontLarge, ui::Rectanglei(0, geWorld.scrHeight, 0, geWorld.scrWidth));
    textGameOver->setAlignement(TextControl::TextAlignement::center);
    textGameOver->setText("Game Over");
}

GameState::~GameState()
{
    delete fontSmall;
    delete fontLarge;
    delete textGameOver;
}

GameState* GameState::getInstance(StateManager* pManager)
{
    static GameState Instance(pManager);
    return &Instance;
}

void GameState::enterState()
{
    asterGame.enterState();
    geWorld.isGameRunning = true;
    asterGame.isMusic = geMusic.GetVolume() > 0.001;
    geSound.Unmute();
    if (geMusic.IsStarted()) geMusic.Play();
}

void GameState::leaveState()
{
    asterGame.leaveState();
    geSound.Mute();
    if (asterGame.isGameOver())
        geMusic.Stop();
    else
        geMusic.Pause();
}

void GameState::reset()
{
    asterGame.reset();
}

void GameState::onKeyDown(WPARAM wKey)
{
    asterGame.key[wKey] = true;
    switch (wKey)
    {
        case 'F':
            isDisplayFps = !isDisplayFps;
        case VK_ESCAPE:
        case VK_RETURN:
            if (asterGame.isGameOver())
            {
                HighScoreState* pHighScores = HighScoreState::GetInstance(stateManager);
                pHighScores->SetNewHighScore(asterGame.scoreCounter.get());
                asterGame.clear();
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
    asterGame.key[wKey] = false;
}

void GameState::onResize(int cx, int cy) {}

void GameState::update(double timeStep)
{
    asterGame.update();
}

void GameState::draw()
{
    asterGame.draw();

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
    fontSmall->drawTextFmt(10, y, "Level: %d", asterGame.gameLevel);

    // glRasterPos2i(w / 2 - 80, y);
    fontSmall->drawTextFmt(w / 2 - 80, y, "Score: %d", asterGame.scoreCounter.get());

    // glRasterPos2i(w - 150, y);
    fontSmall->drawTextFmt(w - 150, y, "Lives: %d", asterGame.lives);

    if (isDisplayFps)
    {
        // glRasterPos2i(w / 2 - 80, y + 20);
        fontSmall->drawTextFmt(w / 2 - 80, y + 20, "FPS: %.0f", asterGame.fps);
    }

    if (isGameOver())
    {
        textGameOver->draw();
    }
}
} // namespace ui
