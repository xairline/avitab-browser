#ifndef BROWSER_H
#define BROWSER_H

#include <XPLMDefs.h>
#include "button.h"

#include <include/cef_app.h>
#include "browser_handler.h"

class Browser {
private:
    int textureId;
    unsigned short yOffset;
    unsigned short width;
    unsigned short height;
    float lastGpsUpdateTime;
    Button *backButton;
    CefRefPtr<BrowserHandler> handler;
    std::string pendingUrl;
    bool createBrowser();
    void updateGPSLocation();
    
public:
    Browser();
    void initialize();
    void destroy();
    void visibilityWillChange(bool becomesVisible);
    void update();
    void draw();
    void loadUrl(std::string url);
    bool hasInputFocus();
    void setFocus(bool focus);
    void mouseMove(float normalizedX, float normalizedY);
    bool click(XPLMMouseStatus status, float normalizedX, float normalizedY);
    void scroll(float normalizedX, float normalizedY, int clicks, bool horizontal);
    void key(unsigned short key, XPLMKeyFlags flags = 0);
    bool goBack();
    CursorType cursor();
};

#endif
