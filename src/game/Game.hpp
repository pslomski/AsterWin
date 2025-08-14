#pragma once

#include <memory>
#include "GameConsts.hpp"
#include "ScoreCounter.hpp"
#include "audio/SfxSample.hpp"
#include "game/objects/Asteroids.hpp"
#include "game/objects/Bonuses.hpp"
#include "game/objects/Bullets.hpp"
#include "game/objects/Objects.hpp"
#include "game/objects/Ship.hpp"
#include "game/objects/TempObjects.hpp"
#include "game/objects/Ufo.hpp"
#include "types/GameState.hpp"
#include "types/Types.hpp"
#include "utils/TimeInterval.hpp"

namespace game
{
class Game
{
public:
    Game();
    ~Game();

    int gameLevel{1};
    ScoreCounter scoreCounter;
    int lives{GE_INITIAL_LIVES};
    bool isMusic;
    Float fps{0};

    bool key[256]; // Array Used For The Keyboard Routine
    bool keypress[256]; // Array Used For The Keyboard Routine

    void clear();
    bool reset();
    void update();
    void draw();
    bool isGameOver() { return GameState::GameOver == gameState; }
    void enterState();
    void leaveState();

private:
    SfxSample sndBroom;
    SfxSample sndStartBeep;
    SfxSample sndBonusBeep;
    SfxSample sndPowerUp;
    objects::Asteroids asteroids; // asteroids objects
    objects::Bullets bullets; // our ship shots
    objects::Bullets bulletsUfo; // enemy ufo shots
    objects::TempObjects shards; // particles of destroyed objects
    objects::Bonuses bonuses; // bonuses from destroyed asteroids
    objects::Objects starBlinks;
    std::unique_ptr<objects::Ship> ship; // our ship
    std::unique_ptr<objects::Ufo> ufo; // enemy ufo
    GameState gameState{GameState::Run};
    int astersCount; // Asteroids starting count. Increases by 1 for level to max 6
    GLuint listBkg1{};
    GLuint listBkg2{};

    utils::TimeInterval tiPause{GE_PAUSE_TIME};
    utils::TimeInterval tiGameStart{1.2f};
    utils::TimeInterval tiUfoRespawn{GE_BASE_UFO_TIME};
    utils::TimeInterval tiFPS{1.0f};
    int frameCount{0};
    int beepCount{0};
    float pitch{0.5f};
    float gain{0.5f};
    utils::TimeInterval tiBroomSound;
    bool bPitchBroomSound;
    utils::TimeInterval tiChangeBroomSoundFreq{GE_TI_CHANGE_BROOM_FREQ};

    void playStartBeep(float pitch, float gain);
    void generateAsters(int iCount, int iGameLevel);
    void clearBackground();
    void generateBackground();
    void processUserInput();
    void analyzeGameState();
    void updateObjects();
    void checkCollisions();
    void onIncrementLives() { lives++; }
};
} // namespace game
