#pragma once

#include <gl/gl.h>
#include <string>
#include <windows.h>
#include "Rectangle.hpp"
#include "game/types/Color.hpp"

class Font
{
public:
    Font();
    ~Font();

    void createFont(const int height, const int weight);
    void drawText(const std::string& strText, int XPos, int YPos, const Color& color) const;
    void drawTextFmt(const int x, const int y, const Color& color, const char* fmt, ...) const;
    ui::Rectanglei GetTextSize(const std::string& text) const;
    static void SetDeviceContext(HDC hdc) { hDeviceContext = hdc; }

private:
    static HDC hDeviceContext;
    GLuint listBase;
    HFONT hFont;
};
