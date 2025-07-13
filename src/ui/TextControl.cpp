#include "TextControl.hpp"
#include <math.h>
#include "game/Consts.hpp"
#include "game/World.hpp"

TextControl::TextControl(const Font* pFont, const ui::Rectanglei& rectPosition)
    : font(pFont), position(rectPosition), text(""), tiBlink(0.5)
{
    isBlink = false;
}

TextControl::~TextControl() {}

void TextControl::update(const Float timeDelta)
{
    if (tiBlink.Inc(timeDelta))
    {
        tiBlink.reset();
    }
}

void TextControl::draw()
{
    ui::Rectanglei textSize = font->GetTextSize(text);
    int yOffset = position.getHeight() / 2 + textSize.getHeight() / 2;
    int xOffset = 0;
    switch (alignement)
    {
        case TextAlignement::left:
            break;
        case TextAlignement::center:
            xOffset = (position.getWidth() - textSize.getWidth()) / 2;
            break;
        case TextAlignement::right:
            xOffset = position.getWidth() - textSize.getWidth();
            break;
    }
    int xPos = position.left + xOffset;
    int yPos = position.top + yOffset - 5;
    GLfloat fFact = 1.0;
    if (isBlink)
    {
        fFact = GLfloat(0.25 * (2.0 + sin(2.0 * GE_PI * tiBlink.Ratio())));
    }
    gl::Color glColor(fFact * color.red, fFact * color.green, fFact * color.blue);
    font->drawText(text, xPos, yPos, glColor);
}

void TextControl::setTextColor(Float fRed, Float fGreen, Float fBlue)
{
    color.red = fRed;
    color.green = fGreen;
    color.blue = fBlue;
}

void TextControl::setBlink(bool in_bBlink)
{
    isBlink = in_bBlink;
}
