#pragma once

#include "game/types/Point.hpp"
#include "game/types/Types.hpp"

bool isPointInPolygon(const int nvert, const PointsF& verts, const Float testx, const Float testy);
