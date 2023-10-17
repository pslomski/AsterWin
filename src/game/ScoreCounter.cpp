#include "ScoreCounter.hpp"
#include "GameConsts.hpp"

namespace ge
{
ScoreCounter::ScoreCounter(OnIncrementLives onIncrementLives) : onIncrementLives(onIncrementLives)
{
    reset();
}

void ScoreCounter::reset()
{
    score = 0;
    nextLife = GE_NEXT_LIFE_SCORE;
    level = 1;
}

void ScoreCounter::inc(const int points)
{
    score += points;
    if (score >= nextLife)
    {
        nextLife += GE_NEXT_LIFE_SCORE;
        onIncrementLives();
        level++;
    }
}
} // namespace ge
