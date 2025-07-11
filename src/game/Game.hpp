#pragma once

#include "Asteroid.hpp"
#include "GameConsts.hpp"
#include "Object.hpp"
#include "ObjectSound.hpp"
#include "ScoreCounter.hpp"
#include "Ship.hpp"
#include "Sound.hpp"
#include "Ufo.hpp"
#include "types/BonusType.hpp"
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

    int gameLevel;
    ScoreCounter scoreCounter;
    int lives;
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
    ObjectSound sndBroom;
    ObjectSound sndStartBeep;
    ObjectSound sndBonusBeep;
    ObjectSound sndPowerUp;
    TvecAster vecAsters; // asteroids objects
    TvecBullet vecBullets; // our ship shots
    TvecBullet vecUfoBullets; // enemy ufo shots
    ObjectList vecDebris; // particles of destroyed objects
    TvecBonus vecBonus; // bonuses from destroyed asteroids
    ObjectList vecStarBlink;
    Ship* ship; // our ship
    Ufo* pUfo; // enemy ufo
    GameState gameState;
    int astersCount; // (4) poczatkowa ilosc asteroidow. wzrasta o 1 z
                     // kazdym poziomem do max 6
    GLuint listBkg1;
    GLuint listBkg2;

    TimeInterval tiPause;
    TimeInterval tiGameStart;
    TimeInterval tiUfoRespawn;
    TimeInterval tiFPS;
    int frameCount{0};
    int beepCount{0};
    float pitch{0};
    float gain{0};
    TimeInterval tiBroomSound;
    bool bPitchBroomSound;
    TimeInterval tiChangeBroomSoundFreq;

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
