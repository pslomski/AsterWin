#include "OptionsState.hpp"
#include <algorithm>
#include "MenuState.hpp"
#include "audio/Sound.hpp"
#include "gl/Utils.hpp"
#include "log/Log.hpp"
#include "ui/Viewport.hpp"

namespace ui
{
OptionsState::OptionsState(StateManager* pManager) : State(pManager)
{
    mCurrentSelection = 0;

    mFont = new Font;
    mFont->createFont(20);

    int dy = 20;
    int left = int(1.0 / 4.0 * ui::viewport.width);
    int right = int(3.0 / 4.0 * ui::viewport.width);
    int top = 50;
    int bottom = top + dy;
    mTitleText = new TextControl(mFont, ui::Rectanglei(top, bottom, left, right));
    mTitleText->setAlignement(TextControl::TextAlignement::center);
    mTitleText->setText("Settings");

    dy = 45;
    top = 135;
    bottom += dy;
    mSoundVolText = new TextControl(mFont, ui::Rectanglei(top, bottom, left, right));
    mSoundVolText->setAlignement(TextControl::TextAlignement::center);
    mSoundVolText->setText("Sound volume: ");

    sndTest.init(SND_SHIP_FIRE, SND_VOL_SHIP_FIRE);
}

OptionsState::~OptionsState()
{
    delete mFont;
    delete mTitleText;
    delete mSoundVolText;
}

OptionsState* OptionsState::GetInstance(StateManager* pManager)
{
    static OptionsState Instance(pManager);
    return &Instance;
}

void OptionsState::enterState()
{
    LOG_INF("Entering OptionsState");
    SetBlinkText(mCurrentSelection, true);
    settings.load();
}

void OptionsState::leaveState()
{
    settings.save();
    LOG_INF("Leaving OptionsState");
}

void OptionsState::onKeyDown(SDL_Scancode key)
{
    switch (key)
    {
        case SDL_SCANCODE_DOWN:
            selectionDown();
            break;
        case SDL_SCANCODE_UP:
            selectionUp();
            break;
        case SDL_SCANCODE_LEFT:
            leftArrow();
            break;
        case SDL_SCANCODE_RIGHT:
            rightArrow();
            break;
        case SDL_SCANCODE_ESCAPE:
            changeState(CMenuState::getInstance(stateManager));
            break;
        default:
            break;
    }
}

void OptionsState::update(const game::TimeDelta dt)
{
    TextControl* txtCtrl = GetTextControl(mCurrentSelection);
    if (txtCtrl) txtCtrl->update(dt);
}

void OptionsState::draw()
{
    constexpr auto bufSize{1024u};
    static char buf[bufSize];

    setGlView(ui::viewport.width, ui::viewport.height);

    mTitleText->draw();

    snprintf(buf, bufSize, "Sound volume: %d", settings.soundVol);
    mSoundVolText->setText(std::string(buf).c_str());
    mSoundVolText->draw();
}

TextControl* OptionsState::GetTextControl(const int id)
{
    return (id == 0) ? mSoundVolText : nullptr;
}

void OptionsState::SetBlinkText(int id, bool isBlink)
{
    TextControl* txtCtrl = GetTextControl(id);
    if (txtCtrl) txtCtrl->setBlink(isBlink);
}

// Only the sound volume remains, so there is nothing to move the selection to.
void OptionsState::selectionUp() {}

void OptionsState::selectionDown() {}

void OptionsState::leftArrow()
{
    settings.soundVol = std::max(0, settings.soundVol - 1);
    geSound.setVolume(0.1f * settings.soundVol);
    sndTest.play();
}

void OptionsState::rightArrow()
{
    settings.soundVol = std::min(10, settings.soundVol + 1);
    geSound.setVolume(0.1f * settings.soundVol);
    sndTest.play();
}
} // namespace ui
