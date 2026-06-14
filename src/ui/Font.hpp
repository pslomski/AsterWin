#pragma once

#include <string>
#include "Rectangle.hpp"
#include "game/types/Color.hpp"

// Vector (stroke) font in the style of the original Asteroids arcade machine.
// Glyphs are drawn as line segments, which renders crisply at any size and
// avoids the thin-stroke artefacts of rasterised outline fonts.
class Font
{
public:
    void createFont(const int height);
    void drawText(const std::string& strText, int XPos, int YPos, const Color& color) const;
    void drawTextFmt(const int x, const int y, const Color& color, const char* fmt, ...) const;
    ui::Rectanglei GetTextSize(const std::string& text) const;

private:
    float scale{1.0f}; // pixels per grid unit
};
