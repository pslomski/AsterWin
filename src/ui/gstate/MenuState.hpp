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
    void update(double timeStep) override;
    void draw() override;
    void enterState() override;

    static CMenuState* GetInstance(StateManager* pManager);

protected:
    CMenuState(StateManager* pManager);

private:
    void SelectionUp();
    void SelectionDown();
    void SelectionChosen();
    void ExitGame();

    Font* m_pFont;
    Font* m_pFontSmall;
    Font* m_pFontSmall2;
    Font* m_pFontLarge;
    int m_iCurrentSelection;
    PlayState* m_pCurrentGame;
    TextControl* m_pTitleText;
    TextControl* m_pNewGameText;
    TextControl* m_pResumeGameText;
    TextControl* m_pScoresText;
    TextControl* m_pExitText;
    TextControl* m_pOptionsText;
    TextControl* GetTextControl(int id);
    void SetBlinkText(int id, bool in_bBlink);
};
} // namespace ui
