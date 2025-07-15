#pragma once

#include <string>
#include "game/types/Point.hpp"
#include "game/types/Types.hpp"

// liczba losowa od 0 do n-1
#define RAND(n) (rand() % (n))

PointF geRotate(PointF& pt, Float alfa);
bool setVSync(int interval = 1);
std::string getAppDir();
