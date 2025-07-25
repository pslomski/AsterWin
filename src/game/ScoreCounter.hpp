#pragma once

#include <functional>

namespace game
{
class ScoreCounter
{
public:
    using OnIncrementLives = std::function<void()>;

    ScoreCounter(OnIncrementLives onIncrementLives);

    void reset();
    int get() { return score; }
    void inc(const int points);

private:
    int score;
    int nextLife;
    OnIncrementLives onIncrementLives;
};
} // namespace game
