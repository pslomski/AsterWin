#pragma once

#include <list>
#include "Bonuses.hpp"
#include "ObjectSound.hpp"
#include "game/objects/Object.hpp"
#include "game/objects/TempObjects.hpp"

namespace game
{
class Asteroid;

typedef std::list<Asteroid*> TvecAster;
typedef std::list<Asteroid*>::iterator TvecAsterIt;

class Asteroid : public Object
{
private:
    int m_iSound;

public:
    Asteroid(int in_iLevel);
    virtual ~Asteroid();
    void Create();
    void Crash(TvecAster& vecAster, TempObjects& vecDebris, TvecBonus& vecBonus);
    static bool CreateBonus;
    int Level;
    bool HasBonus;
    ObjSoundBASS sndCrash;

protected:
    void OnRender() override;
};
} // namespace game
