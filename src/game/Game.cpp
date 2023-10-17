#include "Game.hpp"
#include <algorithm>
#include <cassert>
#include <process.h>
#include <time.h>
#include "GameConsts.hpp"
#include "Sound.hpp"
#include "World.hpp"
#include "game/ScoreCounter.hpp"

namespace ge
{
Game::Game() : scoreCounter(std::bind(&Game::onIncrementLives, this))
{
    sndBroom.Init(SND_BROOM, SND_VOL_BROOM);
    sndStartBeep.Init(SND_START_BEEP, SND_VOL_START_BEEP);
    sndBonusBeep.Init(SND_BONUS_BEEP, SND_VOL_BONUS_BEEP);
    sndPowerUp.Init(SND_POWERUP, SND_VOL_POWERUP);
    tiChangeBroomSoundFreq.Interval = GE_TI_CHANGE_BROOM_FREQ;
    tiFPS.Interval = 1.0;
    ship = nullptr;
    pUfo = nullptr;
    gameState = GameState::Run;
    gameLevel = 1;
    lives = GE_INITIAL_LIVES;
    astersCount = GE_INITIAL_ASTER_COUNT; //(4) poczatkowa ilosc asteroidow. wzrasta o 1 z
                                          // kazdym poziomem do max 6
    listBkg1 = 0;
    listBkg2 = 0;
    tiPause.Reset(GE_PAUSE_TIME);
    tiGameStart.Reset(1.2);
    tiUfoRespawn.Interval = GE_BASE_UFO_TIME;
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
        pUfo->sndEngine.Play();
    }
    if (ship)
    {
        ship->sndEngine.Stop();
    }
}

void Game::leaveState()
{
    if (pUfo)
    {
        pUfo->sndEngine.Pause();
    }
}

void Game::update()
{
    Object::dt = geWorld.dt;
    ++frameCount;
    if (tiFPS.Inc(Object::dt))
    {
        fps = frameCount / tiFPS.Elapsed;
        tiFPS.Reset();
        frameCount = 0;
    }
    processUserInput();
    analyzeGameState();
    checkCollisions();
    updateObjects();
}

template <class Container>
void clearContainer(Container& cont)
{
    for (auto o : cont)
    {
        delete o;
    }
    cont.clear();
}

void Game::clear()
{
    geSound.Stop();
    for (int i = 0; i < 256; ++i)
        key[i] = false;
    for (int i = 0; i < 256; ++i)
        keypress[i] = false;

    clearBackground();

    if (ship) delete ship;
    ship = nullptr;

    if (pUfo) delete pUfo;
    pUfo = nullptr;

    clearContainer(vecBullets);
    clearContainer(vecUfoBullets);
    clearContainer(vecAsters);
    clearContainer(vecDebris);
    clearContainer(vecBonus);
    clearContainer(vecStarBlink);
};

void Game::generateAsters(int iCount, int iGameLevel)
{
    for (int i = 0; i < iCount; ++i)
    {
        Asteroid* pAster = new Asteroid(1);
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
    sndStartBeep.SetVolume(gain);
    sndStartBeep.SetPitch(pitch);
    sndStartBeep.Play();
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
    tiPause.Reset(GE_PAUSE_TIME);

    PointF pt = geWorld.getCenter();
    ship = new Ship(pt.x, pt.y, 90.0);
    generateBackground();
    geSound.Unmute();
    geMusic.Stop();
    tiBroomSound.Reset(5.0);
    bPitchBroomSound = false;
    tiChangeBroomSoundFreq.Reset(GE_TI_CHANGE_BROOM_FREQ);
    tiUfoRespawn.Reset(GE_BASE_UFO_TIME + rand() % 4);
    for (int i = 0; i < 20; ++i)
        vecStarBlink.push_back(new StarBlink());
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
            if (vecBullets.size() < ship->MaxBullets)
            {
                Bullet* pB = ship->FireBullet();
                if (pB) vecBullets.push_back(pB);
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
    geMusic.Play(TRUE);
}

void Game::analyzeGameState()
{
    switch (gameState)
    {
        case GameState::StartGame:
        {
            if (tiGameStart.Inc(Object::dt))
            {
                tiGameStart.Reset();
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
            if (tiChangeBroomSoundFreq.Inc(Object::dt))
            {
                constexpr Float maxInterval{0.7};
                tiChangeBroomSoundFreq.Reset();
                tiBroomSound.Interval -= 1;
                tiBroomSound.Interval = std::max(tiBroomSound.Interval, maxInterval);
            }

            if (tiBroomSound.Inc(Object::dt))
            {
                tiBroomSound.Reset();
                if (bPitchBroomSound)
                {
                    sndBroom.SetPitch(1.05f);
                }
                else
                {
                    sndBroom.SetPitch(1.0f);
                }
                if (!isMusic) sndBroom.Play();
                bPitchBroomSound = !bPitchBroomSound;
            }

            if (nullptr == ship)
            {
                // statek zostal zniszczony
                --lives;
                if (lives > 0)
                {
                    gameState = GameState::ShipDestroyed;
                    tiPause.Reset(GE_PAUSE_TIME);
                }
                else
                {
                    gameState = GameState::GameOver;
                    tiPause.Reset(GE_GAMEOVER_PAUSE_TIME);
                }
            }
            else if (vecAsters.empty() && !pUfo)
            {
                // przejscie na wyzszy poziom
                gameState = GameState::NextLevelPause;
                tiPause.Reset(GE_PAUSE_TIME);
                tiUfoRespawn.Reset(GE_BASE_UFO_TIME + rand() % 4 - 2);
            }
            else
            {
                if (!pUfo)
                {
                    if (tiUfoRespawn.Inc(Object::dt))
                    {
                        const Float maxRespownTime{15};
                        tiUfoRespawn.Reset(std::max(maxRespownTime, tiUfoRespawn.Interval - 1));
                        pUfo = new Ufo;
                        pUfo->setXY(geWorld.getRandomPosAtEdge());
                    }
                }
            }
        }
        break;
        case GameState::NextLevelPause:
            if (tiPause.Inc(Object::dt))
            {
                tiPause.Reset();
                gameState = GameState::Run;
                if (vecAsters.empty())
                {
                    ++astersCount;
                    astersCount = std::min(astersCount, GE_MAX_ASTER_COUNT);
                    generateAsters(astersCount, gameLevel++);
                    tiBroomSound.Reset();
                    tiBroomSound.Interval = 5.0;
                    tiChangeBroomSoundFreq.Interval += 2;
                }
            }
            break;
        case GameState::ShipDestroyed:
            if (tiPause.Inc(Object::dt))
            {
                tiPause.Reset();
                assert(nullptr == ship);
                if (nullptr == ship)
                {
                    gameState = GameState::Run;
                    PointF pt = geWorld.getCenter();
                    ship = new Ship(pt.x, pt.y, 90.0);
                    ship->Respawning = true;
                }
            }
            break;
        case GameState::GameOver:
            if (tiPause.Inc(Object::dt)) geSound.Stop();
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
    TvecAsterIt itAster;
    for (itAster = vecAsters.begin(); itAster != vecAsters.end(); itAster++)
    {
        (*itAster)->update();
        if (pUfo)
        {
            Float Dist = calculateDistance(pUfo, (*itAster));
            if (Dist < Rmin)
            {
                Rmin = Dist;
                pUfo->pAster = (*itAster);
            }
        }
    }

    TvecBulletIt itBullet;
    for (itBullet = vecBullets.begin(); itBullet != vecBullets.end();)
    {
        if ((*itBullet)->expired())
        {
            delete (*itBullet);
            itBullet = vecBullets.erase(itBullet);
        }
        else
        {
            (*itBullet)->update();
            ++itBullet;
        }
    }

    for (itBullet = vecUfoBullets.begin(); itBullet != vecUfoBullets.end();)
    {
        if ((*itBullet)->expired())
        {
            delete (*itBullet);
            itBullet = vecUfoBullets.erase(itBullet);
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

    for (TvecBonusIt it = vecBonus.begin(); it != vecBonus.end();)
    {
        if ((*it)->expired())
        {
            delete (*it);
            it = vecBonus.erase(it);
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
        pUfo->Action(vecUfoBullets);
    }

    for (TvecObiektIt it = vecStarBlink.begin(); it != vecStarBlink.end(); ++it)
    {
        (*it)->update();
    }
}

void Game::checkCollisions()
{
    TvecAster vecAstersTmp;

    // kolizja Statek-Ufo
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
            tiUfoRespawn.Reset();
        }
    }

    // kolizja Strzal_nasz-Ufo
    if (pUfo)
    {
        TvecBulletIt itBullet;
        for (itBullet = vecBullets.begin(); itBullet != vecBullets.end();)
        {
            if (pUfo->checkCollision(*itBullet))
            {
                scoreCounter.inc(pUfo->scoreReward);
                delete (*itBullet);
                itBullet = vecBullets.erase(itBullet);
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

    // kolizja Strzal_Ufo-Statek
    if (ship && !ship->Respawning)
    {
        for (TvecBulletIt itBullet = vecUfoBullets.begin(); itBullet != vecUfoBullets.end();)
        {
            if (ship->checkCollision(*itBullet))
            {
                delete (*itBullet);
                itBullet = vecUfoBullets.erase(itBullet);
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
    TvecAsterIt itAster;
    for (itAster = vecAsters.begin(); itAster != vecAsters.end();)
    {
        bool bIncrement = true;
        if (ship && !ship->Respawning && ship->checkCollision(*itAster))
        {
            ship->Crash(vecDebris);
            delete ship;
            ship = nullptr;

            (*itAster)->Crash(vecAstersTmp, vecDebris, vecBonus);
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
            tiUfoRespawn.Reset();

            Asteroid::CreateBonus = false; // ufo nie generuje bonusow
            (*itAster)->Crash(vecAstersTmp, vecDebris, vecBonus);
            Asteroid::CreateBonus = true;
            delete (*itAster);
            itAster = vecAsters.erase(itAster);
            bIncrement = false;
            if (itAster == vecAsters.end()) break;
        };

        TvecBulletIt itBullet;
        for (itBullet = vecBullets.begin(); itBullet != vecBullets.end();)
        {
            if ((*itAster)->checkCollision(*itBullet))
            {
                delete (*itBullet);
                itBullet = vecBullets.erase(itBullet);
                scoreCounter.inc((*itAster)->scoreReward);
                (*itAster)->Crash(vecAstersTmp, vecDebris, vecBonus);
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
            for (itBullet = vecUfoBullets.begin(); itBullet != vecUfoBullets.end();)
            {
                if ((*itAster)->checkCollision(*itBullet))
                {
                    delete (*itBullet);
                    itBullet = vecUfoBullets.erase(itBullet);

                    Asteroid::CreateBonus = false; // ufo nie generuje bonusow
                    (*itAster)->Crash(vecAstersTmp, vecDebris, vecBonus);
                    Asteroid::CreateBonus = true;
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

    for (itAster = vecAstersTmp.begin(); itAster != vecAstersTmp.end(); itAster++)
    {
        vecAsters.push_back(*itAster);
    }

    // kolizja Statek-Bonus
    if (ship)
    {
        for (TvecBonusIt it = vecBonus.begin(); it != vecBonus.end();)
        {
            if (ship->checkCollision(*it))
            {
                scoreCounter.inc((*it)->scoreReward);
                if ((*it)->bonusType == BonusType::Points)
                {
                    sndBonusBeep.Play();
                }
                else
                {
                    ship->AddBonus((*it)->bonusType);
                    sndPowerUp.Play();
                }
                delete (*it);
                it = vecBonus.erase(it);
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

    TvecAsterIt itAster;
    for (itAster = vecAsters.begin(); itAster != vecAsters.end(); itAster++)
    {
        (*itAster)->draw();
    }

    TvecBulletIt itBullet;
    for (itBullet = vecBullets.begin(); itBullet != vecBullets.end(); ++itBullet)
    {
        (*itBullet)->draw();
    }

    for (itBullet = vecUfoBullets.begin(); itBullet != vecUfoBullets.end(); ++itBullet)
    {
        (*itBullet)->draw();
    }

    for (TvecObiektIt itOb = vecDebris.begin(); itOb != vecDebris.end(); ++itOb)
    {
        (*itOb)->draw();
    }

    for (TvecBonusIt it = vecBonus.begin(); it != vecBonus.end(); ++it)
    {
        (*it)->draw();
    }

    for (TvecObiektIt it = vecStarBlink.begin(); it != vecStarBlink.end(); ++it)
    {
        (*it)->draw();
    }
}

} // namespace ge
