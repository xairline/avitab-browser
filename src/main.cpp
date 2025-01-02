#ifndef XPLM410
    #error This is made to be compiled against the XPLM410 SDK
#endif

#include "config.h"
#include "appstate.h"
#include "dataref.h"
#include "path.h"
#include <XPLMDisplay.h>
#include <XPLMPlugin.h>
#include <XPLMMenus.h>
#include <XPLMProcessing.h>
#include <XPLMMenus.h>
#include <cmath>

#if APL
#include "cursor.h"
#endif

#if IBM
#include <windows.h>
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    
    return TRUE;
}
#endif

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID from, long msg, void* params);
int draw(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon);
float update(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void *inRefcon);
void menuAction(void* mRef, void* iRef);
void registerWindow();

unsigned short pressedKeyCode = 0;
double pressedKeyTime = 0;
XPLMWindowID window;

#if DEBUG
void criticalErrorOccured(const char *inMessage) {
    debug("X-Plane reported an error with our plugin: %s\n", inMessage);
}
#endif

PLUGIN_API int XPluginStart(char * name, char * sig, char * desc)
{
    strcpy(name, FRIENDLY_NAME);
    strcpy(sig, BUNDLE_ID);
    strcpy(desc, "Browser extension for the Avitab");
    XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);
    XPLMEnableFeature("XPLM_USE_NATIVE_WIDGET_WINDOWS", 1);
    
    int item = XPLMAppendMenuItem(XPLMFindPluginsMenu(), FRIENDLY_NAME, nullptr, 1);
    XPLMMenuID id = XPLMCreateMenu(FRIENDLY_NAME, XPLMFindPluginsMenu(), item, menuAction, nullptr);
    XPLMAppendMenuItem(id, "Reload configuration", (void *)"ActionReloadConfig", 0);
    XPLMAppendMenuItem(id, "About", (void *)"ActionAbout", 0);

    XPLMRegisterFlightLoopCallback(update, REFRESH_INTERVAL_SECONDS_SLOW, nullptr);
    XPLMRegisterDrawCallback(draw, xplm_Phase_Gauges, 0, nullptr);
    
    XPluginReceiveMessage(0, XPLM_MSG_PLANE_LOADED, nullptr);
    
#if DEBUG
    XPLMSetErrorCallback(criticalErrorOccured);
#endif
    
    debug("Plugin started.\n");
    return 1;
}

PLUGIN_API void XPluginStop(void) {
    XPLMUnregisterDrawCallback(draw, xplm_Phase_Gauges, 0, nullptr);
    XPLMUnregisterFlightLoopCallback(update, nullptr);
    XPLMDestroyWindow(window);
    window = nullptr;
    
    AppState::getInstance()->deinitialize();
    debug("Plugin stopped.\n");
}

PLUGIN_API int XPluginEnable(void) {
    Path::getInstance()->reloadPaths();
    
    if (window) {
        XPLMBringWindowToFront(window);
    }
    
    return 1;
}

PLUGIN_API void XPluginDisable(void) {
    debug("Disabling plugin...\n");
    //AppState::getInstance()->deinitialize();
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID from, long msg, void* params) {
    switch (msg) {
        case XPLM_MSG_PLANE_LOADED:
            if ((intptr_t)params != 0) {
                // It was not the user's plane that was loaded. Ignore.
                return;
            }

            if (AppState::getInstance()->initialize()) {                
                registerWindow();
            }
            break;
            
        case XPLM_MSG_PLANE_CRASHED:
            break;
            
        case XPLM_MSG_PLANE_UNLOADED:
            AppState::getInstance()->deinitialize();
            break;
            
        default:
            break;
    }
}

void menuAction(void* mRef, void* iRef) {
    if (!strcmp((char *)iRef, "ActionAbout")) {
        int winLeft, winTop, winRight, winBot;
        XPLMGetScreenBoundsGlobal(&winLeft, &winTop, &winRight, &winBot);
        XPLMCreateWindow_t params;
        float width = fabs(winLeft - winRight);
        float height = fabs(winTop - winBot);

        // Calculate centered position for the window
        params.structSize = sizeof(params);
        params.left = (int)(winLeft + (width - 100) / 2);
        params.right = params.left + 100;
        params.top = (int)(winTop - (height - 100) / 2);
        params.bottom = params.top - 100;
        params.visible = 1;
        params.refcon = nullptr;
        params.drawWindowFunc = [](XPLMWindowID, void *){};
        params.handleMouseClickFunc = nullptr;
        params.handleRightClickFunc = nullptr;
        params.handleMouseWheelFunc = nullptr;
        params.handleKeyFunc = nullptr;
        params.handleCursorFunc = nullptr;
        params.layer = xplm_WindowLayerFloatingWindows;
        params.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle;
        XPLMWindowID aboutWindow = XPLMCreateWindowEx(&params);
        XPLMSetWindowTitle(aboutWindow, "About us");
        XPLMSetWindowPositioningMode(aboutWindow, xplm_WindowPositionFree, -1);
        XPLMBringWindowToFront(aboutWindow);
    }
    else if (!strcmp((char *)iRef, "ActionReloadConfig")) {
        AppState::getInstance()->loadConfig();
        
        if (window) {
            XPLMBringWindowToFront(window);
        }
    }
}

void keyPressed(XPLMWindowID inWindowID, char key, XPLMKeyFlags flags, char virtualKey, void* inRefcon, int losingFocus) {
    if ((flags & xplm_DownFlag) == xplm_DownFlag) {
        pressedKeyCode = key;
        pressedKeyTime = XPLMGetElapsedTime();
        AppState::getInstance()->browser->key(key, flags);
    }
    
    if ((flags & xplm_UpFlag) == xplm_UpFlag) {
        pressedKeyCode = 0;
        pressedKeyTime = 0;
        AppState::getInstance()->browser->key(key, flags);
    }
    
    if (losingFocus) {
        AppState::getInstance()->browser->setFocus(false);
    }
}

int mouseClicked(XPLMWindowID inWindowID, int x, int y, XPLMMouseStatus status, void* inRefcon) {
    if (!AppState::getInstance()->hasPower) {
        return 0;
    }
    
    float mouseX, mouseY;
    if (!Dataref::getInstance()->getMouse(&mouseX, &mouseY, x, y)) {
        return 0;
    }
    
    if (status == xplm_MouseDown) {
        bool didConsume = AppState::getInstance()->updateButtons(mouseX, mouseY, kButtonClick);
        if (didConsume) {
            return 1;
        }
    }
    
    if (!AppState::getInstance()->browserVisible) {
        return 0;
    }
        
    if (mouseX < 0 || mouseY < 0 || mouseX > 1 || mouseY > 1) {
        return 0;
    }
    
    if (AppState::getInstance()->browser->click(status, mouseX, mouseY)) {
        return 1;
    }
    
    return 0;
}

int mouseWheel(XPLMWindowID inWindowID, int x, int y, int wheel, int clicks, void* inRefcon) {
    if (!AppState::getInstance()->browserVisible) {
        return 0;
    }
    
    float mouseX, mouseY;
    if (!Dataref::getInstance()->getMouse(&mouseX, &mouseY, x, y)) {
        return 0;
    }
    
    bool horizontal = wheel == 1;
    AppState::getInstance()->browser->scroll(mouseX, mouseY, clicks * 3, horizontal);
    return 1;
}

int mouseCursor(XPLMWindowID inWindowID, int x, int y, void* inRefcon) {
    if (!AppState::getInstance()->hasPower) {
        AppState::getInstance()->activeCursor = CursorDefault;
        return xplm_CursorDefault;
    }
    
    float mouseX, mouseY;
    if (!Dataref::getInstance()->getMouse(&mouseX, &mouseY, x, y)) {
        AppState::getInstance()->activeCursor = CursorDefault;
        return xplm_CursorDefault;
    }
    
    CursorType wantedCursor = CursorDefault;
    if (AppState::getInstance()->browserVisible && AppState::getInstance()->browser->cursor() != CursorDefault) {
        wantedCursor = AppState::getInstance()->browser->cursor();
    }
    else if (AppState::getInstance()->updateButtons(mouseX, mouseY, kButtonHover)) {
        wantedCursor = CursorHand;
    }
    
    if (wantedCursor == CursorDefault) {
        AppState::getInstance()->activeCursor = CursorDefault;
        return xplm_CursorDefault;
    }
    
    if (wantedCursor != AppState::getInstance()->activeCursor) {
        AppState::getInstance()->activeCursor = wantedCursor;
        switch (wantedCursor) {
#if APL
            case CursorHand:
                setPointingHandCursor();
                break;
                
            case CursorText:
                setIBeamCursor();
                break;
#elif LIN
#elif IBM
#endif
            default:
                break;
        }
    }
    
    return xplm_CursorCustom;
}

float update(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void *inRefcon) {
    if (!AppState::getInstance()->pluginInitialized) {
        return REFRESH_INTERVAL_SECONDS_SLOW;
    }
    
    Dataref::getInstance()->update();
    AppState::getInstance()->update();
    AppState::getInstance()->statusbar->update();
    
    AppState::getInstance()->browser->update();
    if (!AppState::getInstance()->browserVisible) {
        return REFRESH_INTERVAL_SECONDS_FAST;
    }
    
    if (pressedKeyTime > 0 && XPLMGetElapsedTime() > pressedKeyTime + 0.3f) {
        AppState::getInstance()->browser->key(pressedKeyCode);
    }
    
    if (AppState::getInstance()->browser->hasInputFocus() != XPLMHasKeyboardFocus(window)) {
        if (AppState::getInstance()->browser->hasInputFocus()) {
            AppState::getInstance()->browser->setFocus(true);
            XPLMBringWindowToFront(window);
            XPLMTakeKeyboardFocus(window);
        }
        else {
            AppState::getInstance()->browser->setFocus(false);
            XPLMTakeKeyboardFocus(0);
        }
    }
    
    float mouseX, mouseY;
    if (Dataref::getInstance()->getMouse(&mouseX, &mouseY)) {
        AppState::getInstance()->browser->mouseMove(mouseX, mouseY);
    }
    
    return REFRESH_INTERVAL_SECONDS_FAST;
}

int draw(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon) {
    AppState::getInstance()->draw();
    return 1;
}

void registerWindow() {
    int winLeft, winTop, winRight, winBot;
    XPLMGetScreenBoundsGlobal(&winLeft, &winTop, &winRight, &winBot);
    XPLMCreateWindow_t params;
    params.structSize = sizeof(params);
    params.left = winLeft;
    params.right = winRight;
    params.top = winTop;
    params.bottom = winBot;
    params.visible = 1;
    params.refcon = nullptr;
    params.drawWindowFunc = [](XPLMWindowID, void *){};
    params.handleMouseClickFunc = mouseClicked;
    params.handleRightClickFunc = nullptr;
    params.handleMouseWheelFunc = mouseWheel;
    params.handleKeyFunc = keyPressed;
    params.handleCursorFunc = mouseCursor;
    params.layer = xplm_WindowLayerFlightOverlay;
    //params.layer = xplm_WindowLayerFloatingWindows;
    params.decorateAsFloatingWindow = xplm_WindowDecorationNone;
    window = XPLMCreateWindowEx(&params);
    XPLMSetWindowPositioningMode(window, xplm_WindowFullScreenOnMonitor, -1);
    
    XPLMBringWindowToFront(window);
}

