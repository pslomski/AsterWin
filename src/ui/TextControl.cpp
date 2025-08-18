#include "TextControl.hpp"
#include <cmath>
#include "game/Consts.hpp"

TextControl::TextControl(Font* pFont, const ui::Rectanglei& rectPosition)
    : position(rectPosition), font{pFont}, tiBlink(0.5)
{
}

TextControl::~TextControl() {}

void TextControl::update(const float timeDelta)
{
    if (tiBlink.inc(timeDelta))
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
    float factor = 1.0f;
    if (isBlink)
    {
        factor = 0.25f * (2.0f + std::sinf(2.0f * pi * tiBlink.ratio()));
    }
    font->drawText(text, xPos, yPos, color * factor);
}

void TextControl::setTextColor(const float red, const float green, const float blue)
{
    color.red = red;
    color.green = green;
    color.blue = blue;
}

void TextControl::setBlink(bool in_bBlink)
{
    isBlink = in_bBlink;
}
