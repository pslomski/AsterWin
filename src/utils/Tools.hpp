#pragma once

#include <GL/gl.h> //TODO Change to gl/gl.h
#include <algorithm>
#include <string>
#include "game/types/Box.hpp"
#include "game/types/Color.hpp"
#include "game/types/Point.hpp"
#include "game/types/Types.hpp"

// liczba losowa od 0 do n-1
#define RAND(n) (rand() % (n))

bool isPointInPolygon(const int nvert, const PointsF& verts, const Float testx, const Float testy);
int linesIntersection(const BoxF& o1, const BoxF& o2, Float& x, Float& y);
PointF geRotate(PointF& pt, Float alfa);
bool setVSync(int interval = 1);
std::string getAppDir();

inline void setGlColor(const Color& color)
{
    glColor3f(color.red, color.green, color.blue);
}

inline void setGlColor(const float gray)
{
    glColor3f(gray, gray, gray);
}
