#include "drawing.h"
#include "config.h"
#include "appstate.h"
#include <vector>
#include <algorithm>

float Drawing::AbsoluteX(float normalizedX) {
    return AppState::getInstance()->tabletDimensions.x + AppState::getInstance()->tabletDimensions.width * normalizedX;
}

float Drawing::AbsoluteY(float normalizedY) {
    return AppState::getInstance()->tabletDimensions.y + AppState::getInstance()->tabletDimensions.height * normalizedY;
}

void Drawing::DrawLine(float x1, float y1, float x2, float y2, float thickness) {
    glLineWidth(thickness);
    glBegin(GL_LINES);
    glVertex2f(AbsoluteX(x1), AbsoluteY(y1));
    glVertex2f(AbsoluteX(x2), AbsoluteY(y2));
    glEnd();
}

void Drawing::DrawRect(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(AbsoluteX(x1), AbsoluteY(y1));
    glVertex2f(AbsoluteX(x1), AbsoluteY(y2));
    glVertex2f(AbsoluteX(x2), AbsoluteY(y2));
    glVertex2f(AbsoluteX(x2), AbsoluteY(y1));
    
    glEnd();
}

void Drawing::DrawRoundedRect(float x1, float y1, float x2, float y2, float radius) {
    constexpr int cornerSegments = 16;
    glBegin(GL_QUADS);
    glVertex2f(AbsoluteX(x1) + radius, AbsoluteY(y1));
    glVertex2f(AbsoluteX(x1) + radius, AbsoluteY(y2));
    glVertex2f(AbsoluteX(x2) - radius, AbsoluteY(y2));
    glVertex2f(AbsoluteX(x2) - radius, AbsoluteY(y1));
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(AbsoluteX(x1) + radius, AbsoluteY(y2) - radius);
    for (int i = 0; i <= cornerSegments; ++i) {
        float angle = (M_PI_2 * 2.0f) + i * (M_PI_2 / cornerSegments);
        glVertex2f(AbsoluteX(x1) + radius + cos(angle) * radius, AbsoluteY(y2 )- radius - sin(angle) * radius);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(AbsoluteX(x2) - radius, AbsoluteY(y2) - radius);
    for (int i = 0; i <= cornerSegments; ++i) {
        float angle = (M_PI_2 * 3.0f) + i * (M_PI_2 / cornerSegments);
        glVertex2f(AbsoluteX(x2) - radius + cos(angle) * radius, AbsoluteY(y2) - radius - sin(angle) * radius);
    }
    glEnd();
    
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(AbsoluteX(x1) + radius, AbsoluteY(y1) + radius);
    for (int i = 0; i <= cornerSegments; ++i) {
        float angle = (M_PI_2 * 1.0f) + i * (M_PI_2 / cornerSegments);
        glVertex2f(AbsoluteX(x1) + radius + cos(angle) * radius, AbsoluteY(y1) + radius - sin(angle) * radius);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(AbsoluteX(x2) - radius, AbsoluteY(y1) + radius);
    for (int i = 0; i <= cornerSegments; ++i) {
        float angle = (M_PI_2 * 0.0f) + i * (M_PI_2 / cornerSegments);
        glVertex2f(AbsoluteX(x2) - radius + cos(angle) * radius, AbsoluteY(y1) + radius - sin(angle) * radius);
    }
    glEnd();

    glLineWidth(2);
    int numPasses = radius / 2;
    for (int i = 0; i < numPasses; ++i) {
        glBegin(GL_LINES);
        glVertex2f(ceilf(AbsoluteX(x1)) + 1.0f + i * 2, AbsoluteY(y1) + radius);
        glVertex2f(ceilf(AbsoluteX(x1)) + 1.0f + i * 2, AbsoluteY(y2) - radius);
        glEnd();
        
        glBegin(GL_LINES);
        glVertex2f(floorf(AbsoluteX(x2)) - 1.0f - i * 2, AbsoluteY(y2) - radius);
        glVertex2f(floorf(AbsoluteX(x2)) - 1.0f - i * 2, AbsoluteY(y1) + radius);
        glEnd();
    }
}

std::vector<std::string> Drawing::SplitTextToWords(const std::string & text) {
    auto is_wrap_char = [](char c) -> bool { return c == ' '; };
    size_t word_count = 1 + std::count_if(text.cbegin(), text.cend(), is_wrap_char);

    std::vector<std::string> ret;
    ret.reserve(word_count);

    size_t current_position = 0;
    while(true) {
        size_t split_position = text.find_first_of(' ', current_position);
        if(split_position == std::string::npos) {
            ret.push_back(text.substr(current_position));
            break;
        }
        ret.push_back(text.substr(current_position, split_position - current_position));
        current_position = split_position + 1;
    }

    return ret;
}

std::vector<std::string> Drawing::WrapWordsToLines(XPLMFontID font, const std::string & text, float normalizedWidth) {
    std::vector<std::string> words = SplitTextToWords(text);
    float spaceWidth = XPLMMeasureString(font, " ", 1);
    float spaceLeft = 0;
    std::vector<std::string> lines;
    for(const std::string & word : words) {
        float wordWidth = XPLMMeasureString(font, word.c_str(), (int)word.size());

        if(wordWidth + spaceWidth <= spaceLeft) {
            lines.back() += " ";
            lines.back() += word;

            spaceLeft -= wordWidth + spaceWidth;
        } else {
            lines.push_back(word);
            float widthPixels = AppState::getInstance()->tabletDimensions.x + AppState::getInstance()->tabletDimensions.width * normalizedWidth;
            spaceLeft = widthPixels - wordWidth;
        }
    }

    return lines;
}

float Drawing::TextWidth(std::string text, float scale) {
    float textWidth = XPLMMeasureString(xplmFont_Proportional, text.c_str(), (int)text.length()) * scale;
    return textWidth / AppState::getInstance()->tabletDimensions.width;
}

float Drawing::DrawText(std::string text, float x, float y, float scale, std::array<float, 3> color) {
    float textWidth = TextWidth(text, scale);
    
    glPushMatrix();
    glTranslatef(AbsoluteX(x - textWidth / 2.0f), AbsoluteY(y) - 3.0f, 0.0f);
    glScalef(scale, scale, 1.0f);
    XPLMDrawString(color.data(), 0, 0, text.c_str(), nullptr, xplmFont_Proportional);
    glPopMatrix();
    
    return textWidth;
}

