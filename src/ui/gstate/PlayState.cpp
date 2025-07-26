#include "PlayState.hpp"
#include "HighScoreState.hpp"
#include "MenuState.hpp"
#include "StateManager.hpp"
#include "audio/Sound.hpp"
#include "game/World.hpp"
#include "log/Log.hpp"

namespace ui
{
#define FNTSIZESMALL 15
#define FNTSIZELARGE 25

PlayState::PlayState(StateManager* pManager) : State(pManager), fontSmall(nullptr), fontLarge(nullptr)
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

PlayState::~PlayState()
{
    delete fontSmall;
    delete fontLarge;
    delete textGameOver;
}

PlayState* PlayState::getInstance(StateManager* pManager)
{
    static PlayState instance(pManager);
    return &instance;
}

void PlayState::enterState()
{
    LOG_INF("Entering PlayState");
    asterGame.enterState();
    asterGame.isMusic = geMusic.getVolume() > 0.001;
    geSound.unmute();
    if (geMusic.isStarted())
    {
        geMusic.play();
    }
}

void PlayState::leaveState()
{
    asterGame.leaveState();
    geSound.mute();
    if (asterGame.isGameOver())
    {
        geMusic.stop();
    }
    else
    {
        geMusic.pause();
    }
    LOG_INF("Leaving PlayState");
}

void PlayState::reset()
{
    asterGame.reset();
}

void PlayState::onKeyDown(WPARAM wKey)
{
    asterGame.key[wKey] = true;
    switch (wKey)
    {
        case 'F':
            isDisplayFps = !isDisplayFps;
            break;
        case VK_ESCAPE:
        case VK_RETURN:
            if (asterGame.isGameOver())
            {
                HighScoreState* pHighScores = HighScoreState::getInstance(stateManager);
                pHighScores->setNewHighScore(asterGame.scoreCounter.get());
                asterGame.clear();
                changeState(pHighScores);
            }
            else
            {
                if (wKey == VK_ESCAPE) changeState(CMenuState::getInstance(stateManager));
            }
            break;
    }
}

void PlayState::onKeyUp(WPARAM wKey)
{
    asterGame.key[wKey] = false;
}

void PlayState::update([[maybe_unused]] const double timeStep)
{
    asterGame.update();
}

void PlayState::draw()
{
    asterGame.draw();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, geWorld.scrWidth, geWorld.scrHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor4fv(gl::colorWhite);
    GLint w = GLint(geWorld.scrWidth);
    // GLint h = GLint(geWorld.scrHeight);
    GLint y = FNTSIZESMALL + 5;
    gl::Color color(1.0, 1.0, 1.0);
    fontSmall->drawTextFmt(10, y, color, "Level: %d", asterGame.gameLevel);
    fontSmall->drawTextFmt(w / 2 - 80, y, color, "Score: %d", asterGame.scoreCounter.get());
    fontSmall->drawTextFmt(w - 150, y, color, "Lives: %d", asterGame.lives);
    if (isDisplayFps)
    {
        fontSmall->drawTextFmt(w / 2 - 80, y + 20, color, "FPS: %.0f", asterGame.fps);
    }
    if (isGameOver())
    {
        textGameOver->draw();
    }
}
} // namespace ui
