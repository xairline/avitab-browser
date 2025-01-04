#include "button.h"
#include "appstate.h"
#include "config.h"
#include <XPLMGraphics.h>
#include <XPLMUtilities.h>
#include <XPLMDisplay.h>

Button::Button(float aWidth, float aHeight) : Image("") {
    callback = nullptr;
    relativeWidth = aWidth;
    relativeHeight = aHeight;
    pixelsWidth = AppState::getInstance()->tabletDimensions.width * aWidth;
    pixelsHeight = AppState::getInstance()->tabletDimensions.height * aHeight;
    AppState::getInstance()->registerButton(this);
}

Button::Button(std::string filename) : Image(filename) {
    AppState::getInstance()->registerButton(this);
}

void Button::destroy() {
    AppState::getInstance()->unregisterButton(this);
    Image::destroy();
}

bool Button::handleState(float normalizedX, float normalizedY, ButtonState state) {
    if (opacity < __FLT_EPSILON__) {
        return false;
    }
    
    float mouseX = AppState::getInstance()->tabletDimensions.width * normalizedX;
    float halfWidth = pixelsWidth / 2.0f;
    
    if (mouseX >= (x - halfWidth) && mouseX <= (x + halfWidth)) {
        float mouseY = AppState::getInstance()->tabletDimensions.height * normalizedY;
        float halfHeight = pixelsHeight / 2.0f;
        if (mouseY >= (y - halfHeight) && mouseY <= (y + halfHeight)) {
            if (state == kButtonClick) {
                if (callback) {
                    callback();
                }
            }
            
            return true;
        }
    }
    
    return false;
}

void Button::setClickHandler(ButtonClickHandlerFunc cb) {
    callback = cb;
}
