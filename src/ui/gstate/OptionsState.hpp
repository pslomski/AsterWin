#pragma once

#include "State.hpp"
#include "ui/Font.hpp"
#include "ui/TextControl.hpp"

namespace ui
{
class OptionsState : public State
{
public:
    ~OptionsState();

    void onKeyDown(WPARAM wKey) override;
    void update(double timeStep) override;
    void draw() override;
    void enterState() override;
    void leaveState() override;
    static OptionsState* GetInstance(StateManager* pManager);

protected:
    OptionsState(StateManager* pManager);

private:
    int mCurrentSelection;
    Font* mFont;
    TextControl* mTitleText;
    TextControl* mMusicVolText;
    TextControl* mSoundVolText;

    void selectionUp();
    void selectionDown();
    void leftArrow();
    void rightArrow();
    TextControl* GetTextControl(const int id);
    void SetBlinkText(int id, bool isBlink);
};
} // namespace ui
