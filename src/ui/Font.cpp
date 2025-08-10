#include "Font.hpp"
#include "gl/Utils.hpp"
#include "utils/Exception.hpp"

HDC Font::hDeviceContext = NULL;

Font::Font() : listBase(0) {}

Font::~Font()
{
    if (listBase)
    {
        glDeleteLists(listBase, 255);
    }
    DeleteObject(hFont);
}

void Font::createFont(const int height, const int weight)
{
    static constexpr auto fontFace{"vector battle"};
    if (!hDeviceContext)
    {
        throw ECannotCreateFont("Impossible to create the font: DeviceContext not created");
    }
    listBase = glGenLists(255);
    if (listBase == 0)
    {
        throw ECannotCreateFont("Impossible to create the font: glGenLists failed");
    }
    hFont = ::CreateFont(
        -height,
        0,
        0,
        0,
        weight,
        FALSE,
        FALSE,
        FALSE,
        ANSI_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,
        FF_DONTCARE | DEFAULT_PITCH,
        fontFace);
    if (hFont == NULL)
    {
        listBase = 0;
        throw ECannotCreateFont("createFont failed");
    }
    HFONT hOldFont = (HFONT)SelectObject(hDeviceContext, hFont);
    wglUseFontBitmaps(hDeviceContext, 0, 255, listBase);
    SelectObject(hDeviceContext, hOldFont);
}

ui::Rectanglei Font::GetTextSize(const std::string& strText) const
{
    SIZE textSize;
    HFONT hOldFont = (HFONT)SelectObject(hDeviceContext, hFont);
    GetTextExtentPoint32(hDeviceContext, strText.c_str(), (int)strText.size(), &textSize);
    SelectObject(hDeviceContext, hOldFont);
    ui::Rectanglei newTextSize(0, textSize.cy, 0, textSize.cx);
    return newTextSize;
}

void Font::drawText(const std::string& text, int x, int y, const Color& color) const
{
    setGlColor(color);
    glRasterPos2i(x, y);
    glPushAttrib(GL_LIST_BIT);
    glListBase(listBase);
    glCallLists(text.size(), GL_UNSIGNED_BYTE, text.c_str());
    glPopAttrib();
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
    vsprintf_s(text, fmt, list);
    va_end(list);
    drawText(std::string(text), x, y, color);
}
