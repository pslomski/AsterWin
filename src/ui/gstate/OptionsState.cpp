#include "OptionsState.hpp"
#include <algorithm>
#include <basetsd.h>
#include "MenuState.hpp"
#include "game/World.hpp"
#include "log/Log.hpp"
#include "ui/Settings.hpp"

namespace ui
{
OptionsState::OptionsState(StateManager* pManager) : State(pManager)
{
    mCurrentSelection = 0;

    mFont = new Font;
    mFont->createFont(20, FW_NORMAL);

    int dy = 20;
    int left = int(1.0 / 4.0 * geWorld.scrWidth);
    int right = int(3.0 / 4.0 * geWorld.scrWidth);
    int top = 50;
    int bottom = top + dy;
    mTitleText = new TextControl(mFont, ui::Rectanglei(top, bottom, left, right));
    mTitleText->setAlignement(TextControl::TextAlignement::center);
    mTitleText->setText("Settings");

    dy = 45;
    top = 135;
    bottom += dy;
    mMusicVolText = new TextControl(mFont, ui::Rectanglei(top, bottom, left, right));
    mMusicVolText->setAlignement(TextControl::TextAlignement::center);
    mMusicVolText->setText("Music volume: ");

    top += dy;
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
    delete mMusicVolText;
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
    geWorld.isGameRunning = false;
    SetBlinkText(mCurrentSelection, true);
}

void OptionsState::leaveState()
{
    Settings settings;
    settings.soundVol = geWorld.soundVol;
    settings.musicVol = geWorld.musicVol;
    settings.save();
    LOG_INF("Leaving OptionsState");
}

void OptionsState::onKeyDown(WPARAM wKey)
{
    switch (wKey)
    {
        case VK_DOWN:
            selectionDown();
            break;
        case VK_UP:
            selectionUp();
            break;
        case VK_LEFT:
            leftArrow();
            break;
        case VK_RIGHT:
            rightArrow();
            break;
        case VK_ESCAPE:
            changeState(CMenuState::getInstance(stateManager));
            break;
    }
}

void OptionsState::update(double timeStep)
{
    TextControl* txtCtrl = GetTextControl(mCurrentSelection);
    if (txtCtrl) txtCtrl->update(timeStep);
}

void OptionsState::draw()
{
    static const int BUF_SIZE = 128;
    static char buf[BUF_SIZE];

    glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
    glLoadIdentity(); // Reset The Projection Matrix
    glOrtho(0, geWorld.scrWidth, geWorld.scrHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
    glLoadIdentity(); // Reset The Modelview Matrix

    mTitleText->draw();

    sprintf_s(buf, BUF_SIZE, "Music volume: %d", geWorld.musicVol);
    mMusicVolText->setText(std::string(buf).c_str());
    mMusicVolText->draw();
    sprintf_s(buf, BUF_SIZE, "Sound volume: %d", geWorld.soundVol);
    mSoundVolText->setText(std::string(buf).c_str());
    mSoundVolText->draw();
}

TextControl* OptionsState::GetTextControl(const int id)
{
    switch (id)
    {
        case 0:
            return mMusicVolText;
        case 1:
            return mSoundVolText;
        default:
            return nullptr;
    }
}

void OptionsState::SetBlinkText(int id, bool isBlink)
{
    TextControl* txtCtrl = GetTextControl(mCurrentSelection);
    if (txtCtrl) txtCtrl->setBlink(isBlink);
}

void OptionsState::selectionUp()
{
    SetBlinkText(mCurrentSelection, false);
    mCurrentSelection--;
    if (mCurrentSelection == -1) mCurrentSelection = 1;

    SetBlinkText(mCurrentSelection, true);
}

void OptionsState::selectionDown()
{
    SetBlinkText(mCurrentSelection, false);
    mCurrentSelection++;
    if (mCurrentSelection == 2) mCurrentSelection = 0;
    SetBlinkText(mCurrentSelection, true);
}

void OptionsState::leftArrow()
{
    switch (mCurrentSelection)
    {
        case 0:
            geWorld.musicVol = std::max(0, geWorld.musicVol - 1);
            geMusic.setVolume(0.1f * geWorld.musicVol);
            break;
        case 1:
            geWorld.soundVol = std::max(0, geWorld.soundVol - 1);
            geSound.setVolume(0.1f * geWorld.soundVol);
            sndTest.play();
            break;
    }
}

void OptionsState::rightArrow()
{
    switch (mCurrentSelection)
    {
        case 0:
            geWorld.musicVol = std::min(10, geWorld.musicVol + 1);
            geMusic.setVolume(0.1f * geWorld.musicVol);
            break;
        case 1:
            geWorld.soundVol = std::min(10, geWorld.soundVol + 1);
            geSound.setVolume(0.1f * geWorld.soundVol);
            sndTest.play();
            break;
    }
}
} // namespace ui
