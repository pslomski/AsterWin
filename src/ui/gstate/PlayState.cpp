#include "PlayState.hpp"
#include "HighScoreState.hpp"
#include "MenuState.hpp"
#include "StateManager.hpp"
#include "audio/Sound.hpp"
#include "gl/Utils.hpp"
#include "log/Log.hpp"
#include "ui/Viewport.hpp"

namespace ui
{
#define FNTSIZESMALL 15
#define FNTSIZELARGE 25

PlayState::PlayState(StateManager* pManager) : State(pManager), fontSmall(nullptr), fontLarge(nullptr)
{
    isDisplayFps = false;
    fontSmall = new Font;
    fontSmall->createFont(FNTSIZESMALL);

    fontLarge = new Font;
    fontLarge->createFont(FNTSIZELARGE);

    textGameOver = new TextControl(fontLarge, ui::Rectanglei(0, ui::viewport.height, 0, ui::viewport.width));
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

void PlayState::onKeyDown(SDL_Scancode key)
{
    asterGame.key[key] = true;
    switch (key)
    {
        case SDL_SCANCODE_F:
            isDisplayFps = !isDisplayFps;
            break;
        case SDL_SCANCODE_ESCAPE:
        case SDL_SCANCODE_RETURN:
            if (asterGame.isGameOver())
            {
                HighScoreState* pHighScores = HighScoreState::getInstance(stateManager);
                pHighScores->setNewHighScore(asterGame.scoreCounter.get());
                asterGame.clear();
                changeState(pHighScores);
            }
            else
            {
                if (key == SDL_SCANCODE_ESCAPE) changeState(CMenuState::getInstance(stateManager));
            }
            break;
        default:
            break;
    }
}

void PlayState::onKeyUp(SDL_Scancode key)
{
    asterGame.key[key] = false;
}

void PlayState::update([[maybe_unused]] const game::TimeDelta dt)
{
    asterGame.update();
}

void PlayState::draw()
{
    asterGame.draw();

    setGlView(ui::viewport.width, ui::viewport.height);
    setGlColor(colorWhite);
    GLint w = GLint(ui::viewport.width);
    GLint y = FNTSIZESMALL + 5;
    fontSmall->drawTextFmt(10, y, colorWhite, "Level: %d", asterGame.gameLevel);
    fontSmall->drawTextFmt(w / 2 - 80, y, colorWhite, "Score: %d", asterGame.scoreCounter.get());
    fontSmall->drawTextFmt(w - 150, y, colorWhite, "Lives: %d", asterGame.lives);
    if (isDisplayFps)
    {
        fontSmall->drawTextFmt(w / 2 - 80, y + 20, colorWhite, "FPS: %.0f", asterGame.fps);
    }
    if (isGameOver())
    {
        textGameOver->draw();
    }
}
} // namespace ui
