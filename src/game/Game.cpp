#include "Game.hpp"
#include <algorithm>
#include <cassert>
#include <process.h>
#include "GameConsts.hpp"
#include "audio/Sound.hpp"
#include "game/GameArea.hpp"
#include "game/Rand.hpp"
#include "game/ScoreCounter.hpp"
#include "game/Time.hpp"
#include "game/geom/CheckCollision.hpp"
#include "game/geom/Distance.hpp"
#include "game/objects/Asteroid.hpp"
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
}

Game::~Game() {}

void Game::generateBackground()
{
    float col;
    if (!listBkg1)
    {
        listBkg1 = glGenLists(1);
        glNewList(listBkg1, GL_COMPILE);
        col = 0.4f + randi(4) / 10.0f;
        setGlColor(col);
        glBegin(GL_POINTS);
        for (int i = 0; i < 50; ++i)
        {
            glVertex2d(randi(gameArea.widthi()), randi(gameArea.heighti()));
        }
        glEnd();
        glEndList();
    }
    if (!listBkg2)
    {
        listBkg2 = glGenLists(1);
        glNewList(listBkg2, GL_COMPILE);
        glBegin(GL_POINTS);
        col = 0.6f + randi(4) / 10.0f;
        setGlColor(col);
        for (int i = 0; i < 50; ++i)
        {
            glVertex2d(randi(gameArea.widthi()), randi(gameArea.heighti()));
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
    if (ufo)
    {
        ufo->sndEngine.play();
    }
    if (ship)
    {
        ship->sndEngine.stop();
    }
}

void Game::leaveState()
{
    if (ufo)
    {
        ufo->sndEngine.pause();
    }
}

void Game::update()
{
    ++frameCount;
    if (tiFPS.inc(time.dt))
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

    ship.reset();
    ufo.reset();

    ::clear(bullets);
    ::clear(bulletsUfo);
    ::clear(asteroids);
    ::clear(shards);
    ::clear(bonuses);
    ::clear(starBlinks);
};

void Game::generateAsters(int iCount, int iGameLevel)
{
    const auto& bounds{gameArea.bounds};
    for (int i = 0; i < iCount; ++i)
    {
        objects::Asteroid* pAster = new objects::Asteroid(1);
        int iSide = i % 4;
        int iPart = randi(4);
        int iAngle = std::min(170, 110 + 10 * iGameLevel);
        iAngle = randi(iAngle) - iAngle / 2;
        if (0 == iSide)
        {
            pAster->setPosition(bounds.x0, iPart / 4.0f * (bounds.y0 + bounds.y1));
            pAster->setAngleDeg(iAngle);
        }
        else if (1 == iSide)
        {
            pAster->setPosition(bounds.x1, iPart / 4.0f * (bounds.y0 + bounds.y1));
            pAster->setAngleDeg(iAngle + 180.0f);
        }
        else if (2 == iSide)
        {
            pAster->setPosition(iPart / 4.0f * (bounds.x0 + bounds.x1), bounds.y0);
            pAster->setAngleDeg(iAngle + 90.0f);
        }
        else if (3 == iSide)
        {
            pAster->setPosition(iPart / 4.0f * (bounds.x0 + bounds.x1), bounds.y1);
            pAster->setAngleDeg(iAngle - 90.0f);
        }
        pAster->setV(3.0f + randi(5) + iGameLevel * 0.25f);
        asteroids.push_back(pAster);
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
    astersCount = GE_INITIAL_ASTER_COUNT;
    beepCount = 0;
    gameState = GameState::StartGame;
    gameLevel = 1;
    scoreCounter.reset();
    lives = GE_INITIAL_LIVES;
    tiPause.reset(GE_PAUSE_TIME);

    const auto pt = gameArea.center();
    ship = std::make_unique<objects::Ship>(pt.x, pt.y, 90.0f);
    generateBackground();
    geSound.unmute();
    geMusic.stop();
    tiBroomSound.reset(GE_TI_BROOM_SOUND);
    bPitchBroomSound = false;
    tiChangeBroomSoundFreq.reset(GE_TI_CHANGE_BROOM_FREQ);
    tiUfoRespawn.reset(GE_BASE_UFO_TIME + randi(4));
    for (int i = 0; i < 20; ++i)
        starBlinks.push_back(new objects::StarBlink());
    return true;
};

void Game::processUserInput()
{
    if (key[VK_UP])
    {
        if (ship) ship->accelerationOn();
    }
    if (!key[VK_UP])
    {
        if (ship) ship->accelerationOff();
    }
    if (key[VK_LEFT])
    {
        if (ship) ship->rotateLeft();
    }
    else
    {
        if (ship) ship->rotateLeftStop();
    }
    if (key[VK_RIGHT])
    {
        if (ship) ship->rotateRight();
    }
    else
    {
        if (ship) ship->rotateRightStop();
    }

    if (key[VK_SPACE] && !keypress[VK_SPACE])
    {
        keypress[VK_SPACE] = true;
        if (ship)
        {
            if (bullets.size() < ship->maxBullets)
            {
                objects::Bullet* pB = ship->fireBullet();
                if (pB) bullets.push_back(pB);
            }
        }
    }
    if (!key[VK_SPACE])
    {
        keypress[VK_SPACE] = false;
    }
}

void threadStartMusic(void*)
{
    Sleep(1000);
    geMusic.play();
}

void Game::analyzeGameState()
{
    switch (gameState)
    {
        case GameState::StartGame:
        {
            if (tiGameStart.inc(time.dt))
            {
                tiGameStart.reset();
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
                    pitch = 0.5f;
                    gain = 0.5f;
                }
                playStartBeep(pitch, gain);
                ++beepCount;
            }
        }
        break;
        case GameState::Run:
        {
            if (tiChangeBroomSoundFreq.inc(time.dt))
            {
                tiChangeBroomSoundFreq.reset();
                tiBroomSound.interval -= 1;
                tiBroomSound.interval = std::max(tiBroomSound.interval, 0.7f);
            }

            if (tiBroomSound.inc(time.dt))
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
            else if (asteroids.empty() && !ufo)
            {
                // przejscie na wyzszy poziom
                gameState = GameState::NextLevelPause;
                tiPause.reset(GE_PAUSE_TIME);
                tiUfoRespawn.reset(GE_BASE_UFO_TIME + randi(4) - 2);
            }
            else
            {
                if (!ufo)
                {
                    if (tiUfoRespawn.inc(time.dt))
                    {
                        const Float maxRespownTime{15};
                        tiUfoRespawn.reset(std::max(maxRespownTime, tiUfoRespawn.interval - 1));
                        ufo = std::make_unique<objects::Ufo>();
                        ufo->setPosition(gameArea.randomPosAtEdge());
                    }
                }
            }
        }
        break;
        case GameState::NextLevelPause:
            if (tiPause.inc(time.dt))
            {
                tiPause.reset();
                gameState = GameState::Run;
                if (asteroids.empty())
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
            if (tiPause.inc(time.dt))
            {
                tiPause.reset();
                assert(not ship);
                if (not ship)
                {
                    gameState = GameState::Run;
                    const auto pt = gameArea.center();
                    ship = std::make_unique<objects::Ship>(pt.x, pt.y, 90.0f);
                    ship->respawning = true;
                }
            }
            break;
        case GameState::GameOver:
            if (tiPause.inc(time.dt))
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
        if (ship->respawning) ship->respawn();
        ship->update();
    }

    if (ufo)
    {
        ufo->update();
        ufo->pAster = nullptr;
    }

    Float Rmin = 1e6;
    for (auto itAster = asteroids.begin(); itAster != asteroids.end(); itAster++)
    {
        (*itAster)->update();
        if (ufo)
        {
            const auto dist = geom::distance(ufo->state.pos, (*itAster)->state.pos);
            if (dist < Rmin)
            {
                Rmin = dist;
                ufo->pAster = (*itAster);
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

    for (auto itOb = shards.begin(); itOb != shards.end();)
    {
        if ((*itOb)->expired())
        {
            delete (*itOb);
            itOb = shards.erase(itOb);
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

    if (ufo)
    {
        ufo->pShip = ship.get();
        ufo->action(bulletsUfo);
    }

    ::update(starBlinks);
}

void Game::checkCollisions()
{
    objects::Asteroids vecAstersTmp;

    // ufo-ship collision
    if (ufo)
    {
        if (ship && !ship->respawning && geom::checkCollision(ship.get(), ufo.get()))
        {
            ship->crash(shards);
            ship.reset();
            ufo->crash(shards);
            ufo.reset();
            tiUfoRespawn.reset();
        }
    }

    // ufo-our_shot collision
    if (ufo)
    {
        for (auto itBullet = bullets.begin(); itBullet != bullets.end();)
        {
            if (geom::checkCollision(ufo.get(), *itBullet))
            {
                scoreCounter.inc(ufo->scoreReward);
                delete (*itBullet);
                itBullet = bullets.erase(itBullet);
                ufo->crash(shards);
                ufo.reset();
                break;
            }
            else
            {
                ++itBullet;
            }
        }
    }

    // ufo_shoot-ship collision
    if (ship and not ship->respawning)
    {
        for (auto itBullet = bulletsUfo.begin(); itBullet != bulletsUfo.end();)
        {
            if (geom::checkCollision(ship.get(), *itBullet))
            {
                delete (*itBullet);
                itBullet = bulletsUfo.erase(itBullet);
                ship->crash(shards);
                ship.reset();
                break;
            }
            else
            {
                ++itBullet;
            }
        }
    }

    // tutaj sprawdzanie kolizji z asteroidami i ew. strzalami przeciwnika
    for (auto itAster = asteroids.begin(); itAster != asteroids.end();)
    {
        bool bIncrement = true;
        if (ship && !ship->respawning && geom::checkCollision(ship.get(), *itAster))
        {
            ship->crash(shards);
            ship.reset();

            (*itAster)->crash(vecAstersTmp, shards, bonuses, true);
            delete (*itAster);
            itAster = asteroids.erase(itAster);
            bIncrement = false;
            if (itAster == asteroids.end()) break;
        };

        if (ufo && geom::checkCollision(ufo.get(), *itAster))
        {
            ufo->crash(shards);
            ufo.reset();
            tiUfoRespawn.reset();

            (*itAster)->crash(vecAstersTmp, shards, bonuses, false);
            delete (*itAster);
            itAster = asteroids.erase(itAster);
            bIncrement = false;
            if (itAster == asteroids.end()) break;
        };

        for (auto itBullet = bullets.begin(); itBullet != bullets.end();)
        {
            if (geom::checkCollision(*itAster, *itBullet))
            {
                delete (*itBullet);
                itBullet = bullets.erase(itBullet);
                scoreCounter.inc((*itAster)->scoreReward);
                (*itAster)->crash(vecAstersTmp, shards, bonuses, true);
                delete (*itAster);
                itAster = asteroids.erase(itAster);
                bIncrement = false;
                if (itAster == asteroids.end()) break;
            }
            else
            {
                ++itBullet;
            }
        }

        if (itAster != asteroids.end())
        {
            for (auto itBullet = bulletsUfo.begin(); itBullet != bulletsUfo.end();)
            {
                if (geom::checkCollision(*itAster, *itBullet))
                {
                    delete (*itBullet);
                    itBullet = bulletsUfo.erase(itBullet);

                    (*itAster)->crash(vecAstersTmp, shards, bonuses, false);
                    delete (*itAster);
                    itAster = asteroids.erase(itAster);
                    bIncrement = false;
                    if (itAster == asteroids.end()) break;
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
        asteroids.push_back(*itAster);
    }

    // ship-bonus collision
    if (ship)
    {
        for (auto it = bonuses.begin(); it != bonuses.end();)
        {
            if (geom::checkCollision(ship.get(), *it))
            {
                scoreCounter.inc((*it)->scoreReward);
                if ((*it)->getBonusType() == BonusType::Points)
                {
                    sndBonusBeep.play();
                }
                else
                {
                    ship->addBonus((*it)->getBonusType());
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
    setGlView(gameArea.width(), gameArea.height(), false);

    glPointSize(1);
    glCallList(listBkg1);
    glPointSize(2);
    glCallList(listBkg2);

    if (ship) ship->draw();
    if (ufo) ufo->draw();
    ::draw(asteroids);
    ::draw(bullets);
    ::draw(bulletsUfo);
    ::draw(shards);
    ::draw(bonuses);
    ::draw(starBlinks);
}
} // namespace game
