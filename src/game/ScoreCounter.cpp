#include "ScoreCounter.hpp"
#include "GameConsts.hpp"

namespace game
{
ScoreCounter::ScoreCounter(OnIncrementLives onIncrementLives) : onIncrementLives(onIncrementLives)
{
    reset();
}

void ScoreCounter::reset()
{
    score = 0;
    nextLife = GE_NEXT_LIFE_SCORE;
}

void ScoreCounter::inc(const int points)
{
    score += points;
    if (score >= nextLife)
    {
        nextLife += GE_NEXT_LIFE_SCORE;
        onIncrementLives();
    }
}
} // namespace game
