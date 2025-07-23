#include "Game.hpp"
#include <algorithm>
#include <cassert>
#include <process.h>
#include <time.h>
#include "GameConsts.hpp"
#include "World.hpp"
#include "audio/Sound.hpp"
#include "game/Rand.hpp"
#include "game/ScoreCounter.hpp"
#include "game/objects/Asteroid.hpp"
#include "game/objects/Objects.hpp"
#include "game/objects/StarBlink.hpp"
#include "gl/Utils.hpp"

namespace
{
template <class Container>
void clear(Container& cont)
{
    for (auto o : cont)
    {
        delete o;
    }
    cont.clear();
}

template <class Container>
void update(Container& objects)
{
    for (auto& object : objects)
    {
        if (object)
        {
            object->update();
        }
    }
}

template <class Container>
void draw(const Container& objects)
{
    for (const auto& object : objects)
    {
        if (object)
        {
            object->draw();
        }
    }
}
} // namespace

namespace game
{
Game::Game() : scoreCounter(std::bind(&Game::onIncrementLives, this))
{
    sndBroom.init(SND_BROOM, SND_VOL_BROOM);
    sndStartBeep.init(SND_START_BEEP, SND_VOL_START_BEEP);
    sndBonusBeep.init(SND_BONUS_BEEP, SND_VOL_BONUS_BEEP);
    sndPowerUp.init(SND_POWERUP, SND_VOL_POWERUP);
    tiChangeBroomSoundFreq.interval = GE_TI_CHANGE_BROOM_FREQ;
    tiFPS.interval = 1.0;
    ship = nullptr;
    pUfo = nullptr;
    gameState = GameState::Run;
    gameLevel = 1;
    lives = GE_INITIAL_LIVES;
    astersCount = GE_INITIAL_ASTER_COUNT; //(4) poczatkowa ilosc asteroidow. wzrasta o 1 z
                                          // kazdym poziomem do max 6
    listBkg1 = 0;
    listBkg2 = 0;
    tiPause.reset(GE_PAUSE_TIME);
    tiGameStart.reset(1.2);
    tiUfoRespawn.interval = GE_BASE_UFO_TIME;
    beepCount = 0;
    pitch = 0.5;
    gain = 0.5;
}

Game::~Game() {}

void Game::generateBackground()
{
    int w = int(geWorld.getWidth());
    int h = int(geWorld.getHeight());
    Float col;

    if (!listBkg1)
    {
        listBkg1 = glGenLists(1);
        glNewList(listBkg1, GL_COMPILE);
        col = 0.4 + RAND(21) / 20;
        setGlColor(col);
        glBegin(GL_POINTS);
        for (int i = 0; i < 50; ++i)
        {
            glVertex2d(rand() % w, rand() % h);
        }
        glEnd();
        glEndList();
    }
    if (!listBkg2)
    {
        listBkg2 = glGenLists(1);
        glNewList(listBkg2, GL_COMPILE);
        glBegin(GL_POINTS);
        col = 0.6 + RAND(21) / 20;
        setGlColor(col);
        for (int i = 0; i < 50; ++i)
        {
            glVertex2d(rand() % w, rand() % h);
        }
        glEnd();
        glEndList();
    }
}

void Game::clearBackground()
{
    if (listBkg1) glDeleteLists(listBkg1, 1);
    listBkg1 = 0;
    if (listBkg2) glDeleteLists(listBkg2, 1);
    listBkg2 = 0;
}

void Game::enterState()
{
    if (pUfo)
    {
        pUfo->sndEngine.play();
    }
    if (ship)
    {
        ship->sndEngine.stop();
    }
}

void Game::leaveState()
{
    if (pUfo)
    {
        pUfo->sndEngine.pause();
    }
}

void Game::update()
{
    Object::dt = geWorld.dt;
    ++frameCount;
    if (tiFPS.inc(Object::dt))
    {
        fps = frameCount / tiFPS.elapsed;
        tiFPS.reset();
        frameCount = 0;
    }
    processUserInput();
    analyzeGameState();
    checkCollisions();
    updateObjects();
}

void Game::clear()
{
    geSound.stop();
    for (int i = 0; i < 256; ++i)
        key[i] = false;
    for (int i = 0; i < 256; ++i)
        keypress[i] = false;

    clearBackground();

    if (ship) delete ship;
    ship = nullptr;

    if (pUfo) delete pUfo;
    pUfo = nullptr;

    ::clear(bullets);
    ::clear(bulletsUfo);
    ::clear(vecAsters);
    ::clear(vecDebris);
    ::clear(bonuses);
    ::clear(vecStarBlink);
};

void Game::generateAsters(int iCount, int iGameLevel)
{
    for (int i = 0; i < iCount; ++i)
    {
        objects::Asteroid* pAster = new objects::Asteroid(1);
        int iSide = i % 4;
        int iPart = rand() % 4;
        int iAngle = std::min(170, 110 + 10 * iGameLevel);
        iAngle = rand() % iAngle - iAngle / 2;
        if (0 == iSide)
        {
            pAster->setXY(geWorld.bounds.x0, iPart / 4.0 * (geWorld.bounds.y0 + geWorld.bounds.y1));
            pAster->setAlfa(iAngle);
        }
        else if (1 == iSide)
        {
            pAster->setXY(geWorld.bounds.x1, iPart / 4.0 * (geWorld.bounds.y0 + geWorld.bounds.y1));
            pAster->setAlfa(iAngle + 180.0);
        }
        else if (2 == iSide)
        {
            pAster->setXY(iPart / 4.0 * (geWorld.bounds.x0 + geWorld.bounds.x1), geWorld.bounds.y0);
            pAster->setAlfa(iAngle + 90);
        }
        else if (3 == iSide)
        {
            pAster->setXY(iPart / 4.0 * (geWorld.bounds.x0 + geWorld.bounds.x1), geWorld.bounds.y1);
            pAster->setAlfa(iAngle - 90);
        }
        pAster->setV(3.0 + rand() % 5 + iGameLevel * 0.25);
        vecAsters.push_back(pAster);
    }
};

void Game::playStartBeep(float pitch, float gain)
{
    sndStartBeep.setVolume(gain);
    sndStartBeep.setPitch(pitch);
    sndStartBeep.play();
}

bool Game::reset()
{
    clear();
    srand((unsigned)time(NULL));
    astersCount = GE_INITIAL_ASTER_COUNT; //(4) poczatkowa liczba asteroidow. wzrasta o 1 z
                                          // kazdym poziomem do max 6
    beepCount = 0;
    gameState = GameState::StartGame;
    gameLevel = 1;
    scoreCounter.reset();
    lives = GE_INITIAL_LIVES;
    tiPause.reset(GE_PAUSE_TIME);

    PointF pt = geWorld.getCenter();
    ship = new objects::Ship(pt.x, pt.y, 90.0);
    generateBackground();
    geSound.unmute();
    geMusic.stop();
    tiBroomSound.reset(GE_TI_BROOM_SOUND);
    bPitchBroomSound = false;
    tiChangeBroomSoundFreq.reset(GE_TI_CHANGE_BROOM_FREQ);
    tiUfoRespawn.reset(GE_BASE_UFO_TIME + rand() % 4);
    for (int i = 0; i < 20; ++i)
        vecStarBlink.push_back(new objects::StarBlink());
    return true;
};

void Game::processUserInput()
{
    if (key[VK_UP])
    {
        if (ship) ship->AccelerationOn();
    }
    if (!key[VK_UP])
    {
        if (ship) ship->AccelerationOff();
    }
    if (key[VK_LEFT])
    {
        if (ship) ship->RotateLeft();
    }
    else
    {
        if (ship) ship->RotateLeftStop();
    }
    if (key[VK_RIGHT])
    {
        if (ship) ship->RotateRight();
    }
    else
    {
        if (ship) ship->RotateRightStop();
    }

    if (key[VK_SPACE] && !keypress[VK_SPACE])
    {
        keypress[VK_SPACE] = true;
        if (ship)
        {
            if (bullets.size() < ship->MaxBullets)
            {
                objects::Bullet* pB = ship->FireBullet();
                if (pB) bullets.push_back(pB);
            }
        }
    }
    if (!key[VK_SPACE])
    {
        keypress[VK_SPACE] = false;
    }
}

void threadStartMusic(void* p)
{
    Sleep(1000);
    geMusic.play(TRUE);
}

void Game::analyzeGameState()
{
    switch (gameState)
    {
        case GameState::StartGame:
        {
            if (tiGameStart.inc(Object::dt))
            {
                tiGameStart.reset();
                float pitch, gain;
                if (beepCount > 2)
                {
                    pitch = pitch * 2.0f;
                    gain = gain * 1.5f;
                    gameState = GameState::Run;
                    generateAsters(astersCount, gameLevel);
                    _beginthread(threadStartMusic, 0, NULL);
                }
                else
                {
                    pitch = pitch;
                    gain = gain;
                }
                playStartBeep(pitch, gain);
                ++beepCount;
            }
        }
        break;
        case GameState::Run:
        {
            if (tiChangeBroomSoundFreq.inc(Object::dt))
            {
                tiChangeBroomSoundFreq.reset();
                tiBroomSound.interval -= 1;
                tiBroomSound.interval = std::max(tiBroomSound.interval, 0.7f);
            }

            if (tiBroomSound.inc(Object::dt))
            {
                tiBroomSound.reset();
                if (bPitchBroomSound)
                {
                    sndBroom.setPitch(1.05f);
                }
                else
                {
                    sndBroom.setPitch(1.0f);
                }
                if (!isMusic) sndBroom.play();
                bPitchBroomSound = !bPitchBroomSound;
            }

            if (nullptr == ship)
            {
                // statek zostal zniszczony
                --lives;
                if (lives > 0)
                {
                    gameState = GameState::ShipDestroyed;
                    tiPause.reset(GE_PAUSE_TIME);
                }
                else
                {
                    gameState = GameState::GameOver;
                    tiPause.reset(GE_GAMEOVER_PAUSE_TIME);
                }
            }
            else if (vecAsters.empty() && !pUfo)
            {
                // przejscie na wyzszy poziom
                gameState = GameState::NextLevelPause;
                tiPause.reset(GE_PAUSE_TIME);
                tiUfoRespawn.reset(GE_BASE_UFO_TIME + rand() % 4 - 2);
            }
            else
            {
                if (!pUfo)
                {
                    if (tiUfoRespawn.inc(Object::dt))
                    {
                        const Float maxRespownTime{15};
                        tiUfoRespawn.reset(std::max(maxRespownTime, tiUfoRespawn.interval - 1));
                        pUfo = new objects::Ufo;
                        pUfo->setXY(geWorld.getRandomPosAtEdge());
                    }
                }
            }
        }
        break;
        case GameState::NextLevelPause:
            if (tiPause.inc(Object::dt))
            {
                tiPause.reset();
                gameState = GameState::Run;
                if (vecAsters.empty())
                {
                    ++astersCount;
                    astersCount = std::min(astersCount, GE_MAX_ASTER_COUNT);
                    generateAsters(astersCount, gameLevel++);
                    tiBroomSound.reset(GE_TI_BROOM_SOUND);
                    tiChangeBroomSoundFreq.interval += 2;
                }
            }
            break;
        case GameState::ShipDestroyed:
            if (tiPause.inc(Object::dt))
            {
                tiPause.reset();
                assert(nullptr == ship);
                if (nullptr == ship)
                {
                    gameState = GameState::Run;
                    PointF pt = geWorld.getCenter();
                    ship = new objects::Ship(pt.x, pt.y, 90.0);
                    ship->Respawning = true;
                }
            }
            break;
        case GameState::GameOver:
            if (tiPause.inc(Object::dt))
            {
                geSound.stop();
            }
            break;
    }
}

void Game::updateObjects()
{
    if (ship)
    {
        if (ship->Respawning) ship->Respawn();
        ship->update();
    }

    if (pUfo)
    {
        pUfo->update();
        pUfo->pAster = nullptr;
    }

    Float Rmin = 1e6;
    for (auto itAster = vecAsters.begin(); itAster != vecAsters.end(); itAster++)
    {
        (*itAster)->update();
        if (pUfo)
        {
            Float Dist = pUfo->distance(*itAster);
            if (Dist < Rmin)
            {
                Rmin = Dist;
                pUfo->pAster = (*itAster);
            }
        }
    }

    for (auto itBullet = bullets.begin(); itBullet != bullets.end();)
    {
        if ((*itBullet)->expired())
        {
            delete (*itBullet);
            itBullet = bullets.erase(itBullet);
        }
        else
        {
            (*itBullet)->update();
            ++itBullet;
        }
    }

    for (auto itBullet = bulletsUfo.begin(); itBullet != bulletsUfo.end();)
    {
        if ((*itBullet)->expired())
        {
            delete (*itBullet);
            itBullet = bulletsUfo.erase(itBullet);
        }
        else
        {
            (*itBullet)->update();
            ++itBullet;
        }
    }

    for (auto itOb = vecDebris.begin(); itOb != vecDebris.end();)
    {
        if ((*itOb)->expired())
        {
            delete (*itOb);
            itOb = vecDebris.erase(itOb);
        }
        else
        {
            (*itOb)->update();
            ++itOb;
        }
    }

    for (auto it = bonuses.begin(); it != bonuses.end();)
    {
        if ((*it)->expired())
        {
            delete (*it);
            it = bonuses.erase(it);
        }
        else
        {
            (*it)->update();
            ++it;
        }
    }

    if (pUfo)
    {
        pUfo->pShip = ship;
        pUfo->Action(bulletsUfo);
    }

    ::update(vecStarBlink);
}

void Game::checkCollisions()
{
    objects::Asteroids vecAstersTmp;

    // ufo-ship collision
    if (pUfo)
    {
        if (ship && !ship->Respawning && ship->checkCollision(pUfo))
        {
            ship->Crash(vecDebris);
            delete ship;
            ship = nullptr;
            pUfo->Crash(vecDebris);
            delete pUfo;
            pUfo = nullptr;
            tiUfoRespawn.reset();
        }
    }

    // ufo-our_shot collision
    if (pUfo)
    {
        for (auto itBullet = bullets.begin(); itBullet != bullets.end();)
        {
            if (pUfo->checkCollision(*itBullet))
            {
                scoreCounter.inc(pUfo->scoreReward);
                delete (*itBullet);
                itBullet = bullets.erase(itBullet);
                pUfo->Crash(vecDebris);
                delete pUfo;
                pUfo = nullptr;
                break;
            }
            else
            {
                ++itBullet;
            }
        }
    }

    // ufo_shoot-ship collision
    if (ship and not ship->Respawning)
    {
        for (auto itBullet = bulletsUfo.begin(); itBullet != bulletsUfo.end();)
        {
            if (ship->checkCollision(*itBullet))
            {
                delete (*itBullet);
                itBullet = bulletsUfo.erase(itBullet);
                ship->Crash(vecDebris);
                delete ship;
                ship = nullptr;
                break;
            }
            else
            {
                ++itBullet;
            }
        }
    }

    // tutaj sprawdzanie kolizji z asteroidami i ew. strzalami przeciwnika
    for (auto itAster = vecAsters.begin(); itAster != vecAsters.end();)
    {
        bool bIncrement = true;
        if (ship && !ship->Respawning && ship->checkCollision(*itAster))
        {
            ship->Crash(vecDebris);
            delete ship;
            ship = nullptr;

            (*itAster)->crash(vecAstersTmp, vecDebris, bonuses, true);
            delete (*itAster);
            itAster = vecAsters.erase(itAster);
            bIncrement = false;
            if (itAster == vecAsters.end()) break;
        };

        if (pUfo && pUfo->checkCollision(*itAster))
        {
            pUfo->Crash(vecDebris);
            delete pUfo;
            pUfo = nullptr;
            tiUfoRespawn.reset();

            (*itAster)->crash(vecAstersTmp, vecDebris, bonuses, false);
            delete (*itAster);
            itAster = vecAsters.erase(itAster);
            bIncrement = false;
            if (itAster == vecAsters.end()) break;
        };

        for (auto itBullet = bullets.begin(); itBullet != bullets.end();)
        {
            if ((*itAster)->checkCollision(*itBullet))
            {
                delete (*itBullet);
                itBullet = bullets.erase(itBullet);
                scoreCounter.inc((*itAster)->scoreReward);
                (*itAster)->crash(vecAstersTmp, vecDebris, bonuses, true);
                delete (*itAster);
                itAster = vecAsters.erase(itAster);
                bIncrement = false;
                if (itAster == vecAsters.end()) break;
            }
            else
            {
                ++itBullet;
            }
        }

        if (itAster != vecAsters.end())
        {
            for (auto itBullet = bulletsUfo.begin(); itBullet != bulletsUfo.end();)
            {
                if ((*itAster)->checkCollision(*itBullet))
                {
                    delete (*itBullet);
                    itBullet = bulletsUfo.erase(itBullet);

                    (*itAster)->crash(vecAstersTmp, vecDebris, bonuses, false);
                    delete (*itAster);
                    itAster = vecAsters.erase(itAster);
                    bIncrement = false;
                    if (itAster == vecAsters.end()) break;
                }
                else
                {
                    ++itBullet;
                }
            }
        }
        if (bIncrement) ++itAster;
    }

    for (auto itAster = vecAstersTmp.begin(); itAster != vecAstersTmp.end(); itAster++)
    {
        vecAsters.push_back(*itAster);
    }

    // ship-bonus collision
    if (ship)
    {
        for (auto it = bonuses.begin(); it != bonuses.end();)
        {
            if (ship->checkCollision(*it))
            {
                scoreCounter.inc((*it)->scoreReward);
                if ((*it)->getBonusType() == BonusType::Points)
                {
                    sndBonusBeep.play();
                }
                else
                {
                    ship->AddBonus((*it)->getBonusType());
                    sndPowerUp.play();
                }
                delete (*it);
                it = bonuses.erase(it);
                break;
            }
            else
            {
                ++it;
            }
        }
    }
}

void Game::draw()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, geWorld.getWidth(), 0, geWorld.getHeight(), -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPointSize(1);
    glCallList(listBkg1);
    glPointSize(2);
    glCallList(listBkg2);

    if (ship) ship->draw();
    if (pUfo) pUfo->draw();
    ::draw(vecAsters);
    ::draw(bullets);
    ::draw(bulletsUfo);
    ::draw(vecDebris);
    ::draw(bonuses);
    ::draw(vecStarBlink);
}
} // namespace game
