#ifndef BUTTON_H
#define BUTTON_H

#include "image.h"
#include <functional>
#include <string>

enum ButtonState { kButtonHover = 1, kButtonClick = 2 };

typedef std::function<void()> ButtonClickHandlerFunc;

class Button : public Image {
  private:
    ButtonClickHandlerFunc callback;

  public:
    Button(float relativeWidth, float relativeHeight);
    Button(std::string filename, unsigned int width = 16,
           unsigned int height = 16);
    void destroy();
    bool handleState(float normalizedX, float normalizedY, ButtonState state);

    void setClickHandler(ButtonClickHandlerFunc callback);
};

#endif
