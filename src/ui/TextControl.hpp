#pragma once

#include <string>
#include "Rectangle.hpp"
#include "game/types/Color.hpp"
#include "ui/Font.hpp"
#include "utils/TimeInterval.hpp"

class TextControl
{
public:
    TextControl(Font* pFont, const ui::Rectanglei& rectPosition);
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
    Font* font;
    utils::TimeInterval tiBlink;
    TextAlignement alignement{TextAlignement::left};
    std::string text;
    Color color{};
    bool isBlink{false};
};
