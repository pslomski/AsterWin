#pragma once

#include "game/objects/TempObject.hpp"
#include "game/types/BonusType.hpp"
#include "utils/TimeInterval.hpp"
#include "utils/Tools.hpp"

class BonusObject : public TempObject
{
public:
    void update() override;
    void OnRender() override;

    BonusType bonusType;
    BonusObject(BonusType in_Type);

private:
    utils::TimeInterval tiRot;
    Float m_ColRatio;
};

class TGEBonusBulletSpeed : public BonusObject
{
public:
    TGEBonusBulletSpeed();

private:
    void Create();
};

class TGEBonusAddBullets : public BonusObject
{
public:
    TGEBonusAddBullets();

private:
    void Create();
};

class TGEBonusPoints : public BonusObject
{
public:
    TGEBonusPoints();

private:
    void Create();
};

typedef std::list<BonusObject*> TvecBonus;
typedef std::list<BonusObject*>::iterator TvecBonusIt;

BonusObject* geCreateBonusObj(BonusType in_Type);
