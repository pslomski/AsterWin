#pragma once

#include "State.hpp"
#include "game/Game.hpp"
#include "ui/Font.hpp"
#include "ui/TextControl.hpp"

namespace ui
{
class PlayState : public State
{
public:
    ~PlayState();

    void onKeyDown(WPARAM wKey) override;
    void onKeyUp(WPARAM) override;
    void update(double timeStep) override;
    void draw() override;
    void enterState() override;
    void leaveState() override;
    bool isGameOver() { return asterGame.isGameOver(); }
    void reset();
    static PlayState* getInstance(StateManager* pManager);

protected:
    PlayState(StateManager* pManager);

private:
    Font* fontSmall;
    Font* fontLarge;
    TextControl* textGameOver;
    game::Game asterGame;
    bool isDisplayFps;
};
} // namespace ui
