#pragma once

#include <array>
#include <string>
#include "Rectangle.hpp"
#include "game/types/Color.hpp"
#include "gl/OpenGl.hpp"

typedef struct TTF_Font TTF_Font;

class Font
{
public:
    Font() = default;
    ~Font();

    void createFont(const int height);
    void drawText(const std::string& strText, int XPos, int YPos, const Color& color) const;
    void drawTextFmt(const int x, const int y, const Color& color, const char* fmt, ...) const;
    ui::Rectanglei GetTextSize(const std::string& text) const;

private:
    struct Glyph
    {
        float u0{}, v0{}, u1{}, v1{}; // texture coordinates in the atlas
        int width{};                  // bitmap size in pixels
        int height{};
        int xOffset{}; // left side bearing
        int advance{}; // pen advance to the next glyph
    };

    static constexpr char firstGlyph{32};
    static constexpr char lastGlyph{126};
    static constexpr int glyphCount{lastGlyph - firstGlyph + 1};

    TTF_Font* font{};
    GLuint texture{};
    std::array<Glyph, glyphCount> glyphs;
    int ascent{};
};
