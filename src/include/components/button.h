#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <functional>
#include "image.h"

enum ButtonState {
    kButtonHover = 1,
    kButtonClick = 2
};

typedef std::function<void()> ButtonClickHandlerFunc;

class Button : public Image {
private:
    ButtonClickHandlerFunc callback;
    
public:
    Button(float relativeWidth, float relativeHeight);
    Button(std::string filename);
    void destroy();
    bool handleState(float normalizedX, float normalizedY, ButtonState state);
    
    void setClickHandler(ButtonClickHandlerFunc callback);
};

#endif
