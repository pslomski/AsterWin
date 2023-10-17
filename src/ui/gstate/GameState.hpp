#pragma once

#include "State.hpp"
#include "game/Game.hpp"
#include "ui/Font.hpp"
#include "ui/TextControl.hpp"

namespace ui
{
class GameState : public State
{
public:
    ~GameState();

    void onKeyDown(WPARAM wKey) override;
    void onKeyUp(WPARAM) override;
    void onResize(int cx, int cy) override;
    void update(double timeStep) override;
    void draw() override;
    void enterState() override;
    void leaveState() override;
    bool IsGameOver() { return AsterGame.isGameOver(); }
    void Reset();
    static GameState* GetInstance(StateManager* pManager);

protected:
    GameState(StateManager* pManager);

private:
    Font* m_pFontSmall;
    Font* m_pFontLarge;
    TextControl* m_pGameOverText;
    ge::Game AsterGame;
    bool m_bDispFPS;
};
} // namespace ui
