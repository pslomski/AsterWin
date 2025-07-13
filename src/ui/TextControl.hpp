#pragma once

#include <string>
#include "Rectangle.hpp"
#include "game/types/Color.hpp"
#include "ui/Font.hpp"
#include "utils/TimeInterval.hpp"
#include "utils/Tools.hpp"

class TextControl
{
public:
    TextControl(const Font* pFont, const ui::Rectanglei& rectPosition);
    ~TextControl();

    enum class TextAlignement
    {
        left,
        center,
        right
    };

    void setText(const std::string& strText) { text = strText; }
    void setAlignement(const TextAlignement Alignement) { alignement = Alignement; }
    void update(const Float timeDelta);
    void draw();
    void setTextColor(Float fRed, Float fGreen, Float fBlue);
    void setBlink(bool in_bBlink);
    ui::Rectanglei position;

private:
    const Font* font;
    std::string text;
    TextAlignement alignement;
    Color color{};
    bool isBlink;
    utils::TimeInterval tiBlink;
};
