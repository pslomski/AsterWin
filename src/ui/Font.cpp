#include "Font.hpp"
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <unordered_map>
#include <vector>
#include "gl/Utils.hpp"

namespace
{
struct Pt
{
    float x, y;
};
using Stroke = std::vector<Pt>;  // a polyline (pen down between consecutive points)
using Glyph = std::vector<Stroke>; // pen lifts between strokes

constexpr float gridHeight = 6.0f; // cap height in grid units
constexpr float advance = 5.0f;    // monospaced cell width in grid units

// Glyphs are defined on a grid: x in [0,4], y in [0,6] with y pointing up
// (y = 0 is the baseline). Lowercase is mapped to uppercase before lookup.
const std::unordered_map<char, Glyph>& glyphTable()
{
    static const std::unordered_map<char, Glyph> table = {
        {' ', {}},
        {'A', {{{0, 0}, {0, 4}, {2, 6}, {4, 4}, {4, 0}}, {{0, 2}, {4, 2}}}},
        {'B', {{{0, 0}, {0, 6}, {3, 6}, {4, 5}, {4, 4}, {3, 3}, {0, 3}}, {{3, 3}, {4, 2}, {4, 1}, {3, 0}, {0, 0}}}},
        {'C', {{{4, 5}, {3, 6}, {1, 6}, {0, 5}, {0, 1}, {1, 0}, {3, 0}, {4, 1}}}},
        {'D', {{{0, 0}, {0, 6}, {2, 6}, {4, 4}, {4, 2}, {2, 0}, {0, 0}}}},
        {'E', {{{4, 6}, {0, 6}, {0, 0}, {4, 0}}, {{0, 3}, {3, 3}}}},
        {'F', {{{0, 0}, {0, 6}, {4, 6}}, {{0, 3}, {3, 3}}}},
        {'G', {{{4, 5}, {3, 6}, {1, 6}, {0, 5}, {0, 1}, {1, 0}, {3, 0}, {4, 1}, {4, 3}, {2, 3}}}},
        {'H', {{{0, 0}, {0, 6}}, {{4, 0}, {4, 6}}, {{0, 3}, {4, 3}}}},
        {'I', {{{0, 6}, {4, 6}}, {{2, 6}, {2, 0}}, {{0, 0}, {4, 0}}}},
        {'J', {{{4, 6}, {4, 1}, {3, 0}, {1, 0}, {0, 1}, {0, 2}}}},
        {'K', {{{0, 0}, {0, 6}}, {{4, 6}, {0, 3}, {4, 0}}}},
        {'L', {{{0, 6}, {0, 0}, {4, 0}}}},
        {'M', {{{0, 0}, {0, 6}, {2, 3}, {4, 6}, {4, 0}}}},
        {'N', {{{0, 0}, {0, 6}, {4, 0}, {4, 6}}}},
        {'O', {{{1, 0}, {3, 0}, {4, 1}, {4, 5}, {3, 6}, {1, 6}, {0, 5}, {0, 1}, {1, 0}}}},
        {'P', {{{0, 0}, {0, 6}, {3, 6}, {4, 5}, {4, 4}, {3, 3}, {0, 3}}}},
        {'Q', {{{1, 0}, {3, 0}, {4, 1}, {4, 5}, {3, 6}, {1, 6}, {0, 5}, {0, 1}, {1, 0}}, {{2, 2}, {4, 0}}}},
        {'R', {{{0, 0}, {0, 6}, {3, 6}, {4, 5}, {4, 4}, {3, 3}, {0, 3}}, {{2, 3}, {4, 0}}}},
        {'S', {{{4, 5}, {3, 6}, {1, 6}, {0, 5}, {0, 4}, {1, 3}, {3, 3}, {4, 2}, {4, 1}, {3, 0}, {1, 0}, {0, 1}}}},
        {'T', {{{0, 6}, {4, 6}}, {{2, 6}, {2, 0}}}},
        {'U', {{{0, 6}, {0, 1}, {1, 0}, {3, 0}, {4, 1}, {4, 6}}}},
        {'V', {{{0, 6}, {2, 0}, {4, 6}}}},
        {'W', {{{0, 6}, {1, 0}, {2, 3}, {3, 0}, {4, 6}}}},
        {'X', {{{0, 0}, {4, 6}}, {{0, 6}, {4, 0}}}},
        {'Y', {{{0, 6}, {2, 3}, {4, 6}}, {{2, 3}, {2, 0}}}},
        {'Z', {{{0, 6}, {4, 6}, {0, 0}, {4, 0}}}},
        {'0', {{{1, 0}, {3, 0}, {4, 1}, {4, 5}, {3, 6}, {1, 6}, {0, 5}, {0, 1}, {1, 0}}, {{0, 1}, {4, 5}}}},
        {'1', {{{1, 5}, {2, 6}, {2, 0}}, {{1, 0}, {3, 0}}}},
        {'2', {{{0, 5}, {1, 6}, {3, 6}, {4, 5}, {4, 4}, {0, 0}, {4, 0}}}},
        {'3', {{{0, 5}, {1, 6}, {3, 6}, {4, 5}, {4, 4}, {3, 3}, {1, 3}}, {{3, 3}, {4, 2}, {4, 1}, {3, 0}, {1, 0}, {0, 1}}}},
        {'4', {{{3, 0}, {3, 6}, {0, 2}, {4, 2}}}},
        {'5', {{{4, 6}, {0, 6}, {0, 3}, {3, 3}, {4, 2}, {4, 1}, {3, 0}, {0, 0}}}},
        {'6', {{{4, 6}, {2, 6}, {0, 4}, {0, 1}, {1, 0}, {3, 0}, {4, 1}, {4, 2}, {3, 3}, {0, 3}}}},
        {'7', {{{0, 6}, {4, 6}, {2, 0}}}},
        {'8', {{{1, 3}, {3, 3}, {4, 4}, {4, 5}, {3, 6}, {1, 6}, {0, 5}, {0, 4}, {1, 3}}, {{1, 3}, {0, 2}, {0, 1}, {1, 0}, {3, 0}, {4, 1}, {4, 2}, {3, 3}}}},
        {'9', {{{4, 3}, {1, 3}, {0, 4}, {0, 5}, {1, 6}, {3, 6}, {4, 5}, {4, 1}, {3, 0}, {1, 0}}}},
        {':', {{{2, 1}, {2, 2}}, {{2, 4}, {2, 5}}}},
        {',', {{{2, 1}, {2, 0}, {1, -1}}}},
        {'.', {{{2, 0}, {2, 1}}}},
        {'-', {{{1, 3}, {3, 3}}}},
        {'!', {{{2, 6}, {2, 2}}, {{2, 0}, {2, 1}}}},
        {'?', {{{0, 5}, {1, 6}, {3, 6}, {4, 5}, {4, 4}, {2, 3}, {2, 2}}, {{2, 0}, {2, 1}}}},
        {'/', {{{0, 0}, {4, 6}}}},
        {'(', {{{3, 6}, {1, 4}, {1, 2}, {3, 0}}}},
        {')', {{{1, 6}, {3, 4}, {3, 2}, {1, 0}}}},
        {'\'', {{{2, 6}, {2, 5}}}},
    };
    return table;
}
} // namespace

void Font::createFont(const int height)
{
    scale = float(height) / gridHeight;
}

ui::Rectanglei Font::GetTextSize(const std::string& strText) const
{
    const int width = int(float(strText.size()) * advance * scale);
    const int height = int(gridHeight * scale);
    return ui::Rectanglei(0, height, 0, width);
}

void Font::drawText(const std::string& text, int x, int y, const Color& color) const
{
    setGlColor(color);
    const auto& table = glyphTable();
    const float baseY = float(y); // baseline; grid y points up, screen y points down
    float penX = float(x);
    glBegin(GL_LINES);
    for (const char ch : text)
    {
        const char upper = char(std::toupper(static_cast<unsigned char>(ch)));
        const auto it = table.find(upper);
        if (it != table.end())
        {
            for (const Stroke& stroke : it->second)
            {
                for (size_t i = 0; i + 1 < stroke.size(); ++i)
                {
                    glVertex2f(penX + stroke[i].x * scale, baseY - stroke[i].y * scale);
                    glVertex2f(penX + stroke[i + 1].x * scale, baseY - stroke[i + 1].y * scale);
                }
            }
        }
        penX += advance * scale;
    }
    glEnd();
}

void Font::drawTextFmt(const int x, const int y, const Color& color, const char* fmt, ...) const
{
    if (fmt == nullptr)
    {
        return;
    }
    char text[256];
    va_list list;
    va_start(list, fmt);
    vsnprintf(text, sizeof(text), fmt, list);
    va_end(list);
    drawText(std::string(text), x, y, color);
}
