#pragma once

#include <GL/gl.h>
#include <string>
#include <windows.h>
#include "Rectangle.hpp"
#include "gl/Color.hpp"

class Font
{
public:
    Font();
    ~Font();

    void createFont(const int height, const int weight);
    void drawText(const std::string& strText, int XPos, int YPos, const gl::Color& color) const;
    void drawTextFmt(const int x, const int y, const gl::Color& color, const char* fmt, ...) const;
    ui::Rectanglei GetTextSize(const std::string& text) const;
    static void SetDeviceContext(HDC hdc) { hDeviceContext = hdc; }

private:
    static HDC hDeviceContext;
    GLuint listBase;
    HFONT hFont;
};
