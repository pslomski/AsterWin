#include "Font.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include "gl/Utils.hpp"
#include "utils/Exception.hpp"

Font::~Font()
{
    // States that own fonts are static-lifetime singletons, so this destructor
    // can run after the GL context and SDL_ttf have already been torn down at
    // program exit. Only release resources while their owners are still alive.
    if (texture && SDL_GL_GetCurrentContext())
    {
        glDeleteTextures(1, &texture);
    }
    if (font && TTF_WasInit())
    {
        TTF_CloseFont(font);
    }
}

void Font::createFont(const int height)
{
    static constexpr auto fontFile{"Vectorb.ttf"};
    font = TTF_OpenFont(fontFile, float(height));
    if (!font)
    {
        throw ECannotCreateFont("Impossible to create the font: TTF_OpenFont failed");
    }
    ascent = TTF_GetFontAscent(font);

    // Render all printable ASCII glyphs and pack them into a single texture atlas.
    static constexpr SDL_Color white{255, 255, 255, 255};
    std::array<SDL_Surface*, glyphCount> surfaces{};
    int atlasWidth = 1;
    int atlasHeight = 1;
    for (int i = 0; i < glyphCount; ++i)
    {
        const Uint32 ch = Uint32(firstGlyph + i);
        int minx{}, maxx{}, miny{}, maxy{}, advance{};
        TTF_GetGlyphMetrics(font, ch, &minx, &maxx, &miny, &maxy, &advance);
        glyphs[i].xOffset = minx;
        glyphs[i].advance = advance;
        surfaces[i] = TTF_RenderGlyph_Blended(font, ch, white);
        if (surfaces[i])
        {
            atlasWidth += surfaces[i]->w + 1;
            atlasHeight = std::max(atlasHeight, surfaces[i]->h);
        }
    }

    SDL_Surface* atlas = SDL_CreateSurface(atlasWidth, atlasHeight, SDL_PIXELFORMAT_RGBA32);
    if (!atlas)
    {
        throw ECannotCreateFont("Impossible to create the font: cannot create the atlas surface");
    }
    int penX = 0;
    for (int i = 0; i < glyphCount; ++i)
    {
        if (!surfaces[i]) continue;
        // Copy straight (un-premultiplied) alpha into the atlas; a real alpha
        // blit onto transparent black would premultiply and crush the thin
        // anti-aliased strokes of this font to near-black in GL_MODULATE.
        SDL_SetSurfaceBlendMode(surfaces[i], SDL_BLENDMODE_NONE);
        SDL_Rect dstRect{penX, 0, surfaces[i]->w, surfaces[i]->h};
        SDL_BlitSurface(surfaces[i], nullptr, atlas, &dstRect);
        glyphs[i].width = surfaces[i]->w;
        glyphs[i].height = surfaces[i]->h;
        glyphs[i].u0 = float(penX) / float(atlasWidth);
        glyphs[i].v0 = 0.0f;
        glyphs[i].u1 = float(penX + surfaces[i]->w) / float(atlasWidth);
        glyphs[i].v1 = float(surfaces[i]->h) / float(atlasHeight);
        penX += surfaces[i]->w + 1;
        SDL_DestroySurface(surfaces[i]);
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, atlas->pitch / 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas->pixels);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_DestroySurface(atlas);
}

ui::Rectanglei Font::GetTextSize(const std::string& strText) const
{
    int width{};
    int height{};
    if (font)
    {
        TTF_GetStringSize(font, strText.c_str(), strText.size(), &width, &height);
    }
    return ui::Rectanglei(0, height, 0, width);
}

void Font::drawText(const std::string& text, int x, int y, const Color& color) const
{
    if (!texture) return;
    setGlColor(color);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    float penX = float(x);
    const float top = float(y - ascent); // (x, y) is the text baseline
    for (const char ch : text)
    {
        if (ch < firstGlyph or ch > lastGlyph) continue;
        const Glyph& glyph = glyphs[ch - firstGlyph];
        const float x0 = penX + float(glyph.xOffset);
        const float x1 = x0 + float(glyph.width);
        const float y1 = top + float(glyph.height);
        glTexCoord2f(glyph.u0, glyph.v0);
        glVertex2f(x0, top);
        glTexCoord2f(glyph.u1, glyph.v0);
        glVertex2f(x1, top);
        glTexCoord2f(glyph.u1, glyph.v1);
        glVertex2f(x1, y1);
        glTexCoord2f(glyph.u0, glyph.v1);
        glVertex2f(x0, y1);
        penX += float(glyph.advance);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
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
