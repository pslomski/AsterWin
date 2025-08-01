#pragma once

#include "PlayState.hpp"
#include "State.hpp"
#include "ui/Font.hpp"
#include "ui/TextControl.hpp"

namespace ui
{
class CMenuState : public State
{
public:
    ~CMenuState();

    void onKeyDown(WPARAM wKey) override;
    void update(const game::TimeDelta dt) override;
    void draw() override;
    void enterState() override;

    static CMenuState* getInstance(StateManager* pManager);

protected:
    CMenuState(StateManager* pManager);

private:
    void selectionUp();
    void selectionDown();
    void selectionChosen();
    void exitGame();

    Font font;
    Font* fontSmall{nullptr};
    Font* fontSmall2{nullptr};
    Font* fontLarge{nullptr};
    int currentSelection{0};
    PlayState* currentGame{nullptr};
    TextControl* titleText{nullptr};
    TextControl* newGameText{nullptr};
    TextControl* resumeGameText{nullptr};
    TextControl* scoresText{nullptr};
    TextControl* exitText{nullptr};
    TextControl* optionsText{nullptr};
    TextControl* getTextControl(int id);
    void setBlinkText(int id, bool isBlink);
};
} // namespace ui
