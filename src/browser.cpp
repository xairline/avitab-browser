#include "browser.h"
#include <iomanip>
#include <sstream>
#include <include/cef_base.h>
#include <include/base/cef_callback.h>
#include <include/wrapper/cef_library_loader.h>
#include <include/cef_version.h>
#include <include/cef_base.h>
#include <include/cef_app.h>
#include <include/cef_browser.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/cef_command_line.h>
#include <include/wrapper/cef_helpers.h>
#include <include/base/cef_bind.h>
#include <include/base/cef_callback.h>
#include <include/wrapper/cef_closure_task.h>
#include <include/cef_request_context_handler.h>
#include <XPLMUtilities.h>
#include <XPLMGraphics.h>
#include <XPLMProcessing.h>
#include "config.h"
#include "path.h"
#include "appstate.h"
#include "dataref.h"
#include "browser_handler.h"
#include "keycodes.h"

Browser::Browser() {
}

void Browser::initialize() {
    if (textureId || handler) {
        return;
    }

    if (AppState::getInstance()->aircraftVariant == VariantZibo738) {
        width = AppState::getInstance()->tabletDimensions.width;
        height = ceil(AppState::getInstance()->tabletDimensions.height * 0.947f);
        yOffset = ceil(AppState::getInstance()->tabletDimensions.height * 0.03f);
        
        backButton = new Button(0.27f, 0.09f);
        backButton->setPosition(0.15f, -0.019f);
        backButton->setClickHandler([]() {
            if (!AppState::getInstance()->browserVisible) {
                return;
            }
            
            bool didGoBack = AppState::getInstance()->browser->goBack();
            if (!didGoBack) {
                if (AppState::getInstance()->aircraftVariant == VariantZibo738) {
                    Dataref::getInstance()->executeCommand("laminar/B738/tab/home");
                }
            }
        });
    }
    else {
        width = AppState::getInstance()->tabletDimensions.width;
        height = ceil(AppState::getInstance()->tabletDimensions.height * 0.935f);
        yOffset = 0;
        
        backButton = new Button(Path::getInstance()->pluginDirectory + "/assets/icons/arrow-left-circle.svg");
        backButton->setPosition(backButton->relativeWidth / 2.0f + 0.01f, 0.967f);
        backButton->setClickHandler([]() {
            if (!AppState::getInstance()->browserVisible) {
                return;
            }
            
            bool didGoBack = AppState::getInstance()->browser->goBack();
            if (!didGoBack) {
                if (AppState::getInstance()->aircraftVariant == VariantZibo738) {
                    Dataref::getInstance()->executeCommand("AviTab/Home");
                }
            }
        });
    }
    
    XPLMGenerateTextureNumbers(&textureId, 1);
    XPLMBindTexture2d(textureId, 0);
    std::vector<unsigned char> whiteTextureData(
        AppState::getInstance()->tabletDimensions.textureWidth *
        AppState::getInstance()->tabletDimensions.textureHeight *
        AppState::getInstance()->tabletDimensions.bytesPerPixel
    );
    std::fill(whiteTextureData.begin(), whiteTextureData.end(), 0xFF);

    glTexImage2D(
                 GL_TEXTURE_2D,
                 0,                   // mipmap level
                 GL_RGBA,             // internal format for the GL to use.  (We could ask for a floating point tex or 16-bit tex if we were crazy!)
                 AppState::getInstance()->tabletDimensions.textureWidth,
                 AppState::getInstance()->tabletDimensions.textureHeight,
                 0,                   // border size
                 GL_BGRA,             // format of color we are giving to GL
                 GL_UNSIGNED_BYTE,    // encoding of our data
                 whiteTextureData.data()
                 );
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Browser::destroy() {
    if (handler) {
//        if (CefCurrentlyOn(TID_UI)) {
//            CefShutdown();
//        }
        
        handler->browserInstance->GetHost()->CloseBrowser(true);
        float graceTimeout = XPLMGetElapsedTime() + 5.0f;
        while (handler->browserInstance && XPLMGetElapsedTime() < graceTimeout) {
            CefDoMessageLoopWork();
        }
        
        handler->destroy();
        handler = nullptr;
    }
    
    if (textureId) {
        XPLMBindTexture2d(textureId, 0);
        glDeleteTextures(1, (GLuint *)&textureId);
        textureId = 0;
    }
    
    if (backButton) {
        backButton->destroy();
        backButton = nullptr;
    }
}

void Browser::visibilityWillChange(bool becomesVisible) {
    if (becomesVisible && !handler) {
        createBrowser();
    }
    
    lastGpsUpdateTime = becomesVisible ? XPLMGetElapsedTime() : 0.0f;
}

void Browser::update() {
    if (!textureId) {
        return;
    }
    
    if (handler && AppState::getInstance()->browserVisible) {
        CefDoMessageLoopWork();
    }
    
    if (backButton) {
        backButton->opacity = AppState::getInstance()->browserVisible ? 1.0f : 0.0f;
    }
    
    if (lastGpsUpdateTime > __FLT_EPSILON__ && XPLMGetElapsedTime() > lastGpsUpdateTime + 1.0f) {
        updateGPSLocation();
    }
}

void Browser::draw() {
    if (!textureId) {
        return;
    }
    
    XPLMSetGraphicsState(
                         0, // No fog, equivalent to glDisable(GL_FOG);
                         1, // One texture, equivalent to glEnable(GL_TEXTURE_2D);
                         0, // No lighting, equivalent to glDisable(GL_LIGHT0);
                         0, // No alpha testing, e.g glDisable(GL_ALPHA_TEST);
                         1, // Use alpha blending, e.g. glEnable(GL_BLEND);
                         0, // No depth read, e.g. glDisable(GL_DEPTH_TEST);
                         0 // No depth write, e.g. glDepthMask(GL_FALSE);
    );
    
    XPLMBindTexture2d(textureId, 0);
    
    int x1 = AppState::getInstance()->tabletDimensions.x;
    int y1 = AppState::getInstance()->tabletDimensions.y + yOffset;
    int x2 = x1 + width;
    int y2 = y1 + height;
    
    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);
    
    float u = (float)width / AppState::getInstance()->tabletDimensions.textureWidth;
    float v = (float)height / AppState::getInstance()->tabletDimensions.textureHeight;
    glTexCoord2f(0, v);
    glVertex2f(x1,y1);
    glTexCoord2f(0, 0);
    glVertex2f(x1,y2);
    glTexCoord2f(u, 0);
    glVertex2f(x2,y2);
    glTexCoord2f(u, v);
    glVertex2f(x2,y1);
    glEnd();
    
    if (backButton) {
        backButton->draw();
    }
}

void Browser::mouseMove(float normalizedX, float normalizedY) {
    if (!textureId || !handler) {
        return;
    }
    
    CefMouseEvent mouseEvent;
    mouseEvent.x = AppState::getInstance()->tabletDimensions.width * normalizedX;
    mouseEvent.y = AppState::getInstance()->tabletDimensions.height * normalizedY - (AppState::getInstance()->tabletDimensions.height - height - yOffset);
    
    handler->browserInstance->GetHost()->SendMouseMoveEvent(mouseEvent, false);
}

bool Browser::click(XPLMMouseStatus status, float normalizedX, float normalizedY) {
    if (!textureId || !handler) {
        return false;
    }
    
    CefMouseEvent mouseEvent;
    mouseEvent.x = AppState::getInstance()->tabletDimensions.width * normalizedX;
    mouseEvent.y = AppState::getInstance()->tabletDimensions.height * normalizedY - (AppState::getInstance()->tabletDimensions.height - height - yOffset);
    if (mouseEvent.y < 0) {
        return false;
    }
    
    if (status == xplm_MouseDown) {
        handler->browserInstance->GetHost()->SendMouseClickEvent(mouseEvent, MBT_LEFT, false, 1);
    }
    else if (status == xplm_MouseDrag) {
        // Yes, we already send this event in mouseMove(). Adding the line below makes it more responsive.
        handler->browserInstance->GetHost()->SendMouseMoveEvent(mouseEvent, false);
    }
    else {
        handler->browserInstance->GetHost()->SendMouseClickEvent(mouseEvent, MBT_LEFT, true, 1);
    }
    
    return true;
}

void Browser::scroll(float normalizedX, float normalizedY, int clicks, bool horizontal = false) {
    if (!textureId || !handler) {
        return;
    }
    
    CefMouseEvent mouseEvent;
    mouseEvent.x = AppState::getInstance()->tabletDimensions.width * normalizedX;
    mouseEvent.y = AppState::getInstance()->tabletDimensions.height * normalizedY - (AppState::getInstance()->tabletDimensions.height - height - yOffset);
    mouseEvent.modifiers = EVENTFLAG_NONE;
    handler->browserInstance->GetHost()->SendMouseWheelEvent(mouseEvent, horizontal ? clicks : 0, horizontal ? 0 : clicks);
}

void Browser::loadUrl(std::string url) {
    if (!textureId || !handler) {
        pendingUrl = url;
        return;
    }
    
    if (handler->browserInstance) {
        handler->browserInstance->GetMainFrame()->LoadURL(url);
        pendingUrl = "";
    }
    else {
        pendingUrl = url;
    }
}

bool Browser::hasInputFocus() {
    if (!textureId || !handler) {
        return false;
    }
    
    return handler->hasInputFocus;
}

void Browser::setFocus(bool focus) {
    if (!textureId || !handler) {
        return;
    }
    
    handler->browserInstance->GetHost()->SetFocus(focus);
}

void Browser::key(unsigned short key, XPLMKeyFlags flags) {
    if (!textureId || !handler) {
        return;
    }
    
    CefKeyEvent keyEvent;
    keyEvent.type = (flags == 0 || (flags & xplm_DownFlag) == xplm_DownFlag) ? KEYEVENT_KEYDOWN : KEYEVENT_KEYUP;
    
//#if APL
//    if (keyEvent.type == KEYEVENT_KEYUP) {
//        return;
//    }
//#endif
    
    if (keyEvent.type == KEYEVENT_KEYDOWN) {
        auto it = KeyCodes::table.find(key);
        if (it != KeyCodes::table.end()) {
            int keyCode = it->second;
            keyEvent.windows_key_code = keyCode;
            keyEvent.native_key_code = keyCode;
        } else {
            // Missing key
            debug("Missing key (ascii): %02X\n", key);
            keyEvent.windows_key_code = KeyCodes::VKEY_UNKNOWN;
            keyEvent.native_key_code = KeyCodes::VKEY_UNKNOWN;
        }
        
        keyEvent.is_system_key = false;
        keyEvent.modifiers = 0;
        if ((flags & xplm_ShiftFlag) == xplm_ShiftFlag) {
            keyEvent.modifiers |= EVENTFLAG_SHIFT_DOWN;
        }
        
        if ((flags & xplm_OptionAltFlag) == xplm_OptionAltFlag) {
            keyEvent.modifiers |= EVENTFLAG_ALT_DOWN;
        }
        
        if ((flags & xplm_ControlFlag) == xplm_ControlFlag) {
            keyEvent.modifiers |= EVENTFLAG_CONTROL_DOWN;
            //keyEvent.modifiers |= EVENTFLAG_COMMAND_DOWN;
        }
    }
    
    handler->browserInstance->GetHost()->SendKeyEvent(keyEvent);
    
    if (isprint(key)) {
        CefKeyEvent textEvent;
        textEvent.type = KEYEVENT_CHAR;
        textEvent.character = key;
        textEvent.unmodified_character = key;
        
        if (keyEvent.type == KEYEVENT_KEYDOWN) {
            handler->browserInstance->GetHost()->SendKeyEvent(textEvent);
        }
    }
}

bool Browser::goBack() {
    if (!textureId || !handler) {
        return false;
    }
    
    if (!handler->browserInstance->CanGoBack()) {
        return false;
    }
    
    handler->browserInstance->GoBack();
    return true;
}

CursorType Browser::cursor() {
    if (!handler) {
        return CursorDefault;
    }
    
    return handler->cursorState;
}

bool Browser::createBrowser() {
    if (handler && handler->browserInstance) {
        return false;
    }
    
    CefScopedLibraryLoader library_loader;
    if (!library_loader.LoadInMain()) {
        debug("Could not load CEF library!\n");
        return false;
    }
    
    debug("Loading CEF library: %s\n", CEF_VERSION);
    
    CefRequestContextSettings context_settings;
    CefString(&context_settings.cache_path) = Path::getInstance()->pluginDirectory + "/cache";
    
    std::string language = "";
    switch (XPLMLanguageCode()) {
        case xplm_Language_English:
            language = "en-US,en";
            break;
            
        case xplm_Language_French:
            language = "fr-FR,fr";
            break;
            
        case xplm_Language_German:
            language = "de-DE,de";
            break;
            
        case xplm_Language_Italian:
            language = "it-IT,it";
            break;
            
        case xplm_Language_Spanish:
            language = "es-ES,es";
            break;
            
        case xplm_Language_Korean:
            language = "ko-KR,ko";
            break;
            
        case xplm_Language_Russian:
            language = "ru-RU,ru";
            break;
            
        case xplm_Language_Greek:
            language = "el-GR,el";
            break;
            
        case xplm_Language_Japanese:
            language = "ja-JP,ja";
            break;
            
        case xplm_Language_Chinese:
            language = "zh-CN,zh";
            break;
            
        case xplm_Language_Ukrainian:
            language = "uk-UA,uk";
            break;
            
        case xplm_Language_Unknown:
        default:
            break;
    }
    
    if (!AppState::getInstance()->config.forced_language.empty()) {
        language = AppState::getInstance()->config.forced_language;
    }
    
    if (!language.empty()) {
        CefString(&context_settings.accept_language_list) = language;
    }
    
    context_settings.persist_user_preferences = true;
    context_settings.persist_session_cookies = true;
    CefRefPtr<CefRequestContext> request_context = CefRequestContext::CreateContext(context_settings, nullptr);
    
    CefBrowserSettings browser_settings;
    browser_settings.windowless_frame_rate = AppState::getInstance()->config.framerate;
    browser_settings.background_color = CefColorSetARGB(0xFF, 0xFF, 0xFF, 0xFF);
    // Initialize CEF for the browser process. (this is already done by X-Plane)
//    if (!CefInitialize(CefMainArgs(), settings, nullptr, nullptr)) {
//        debug("CefInitialize failed.\n");
//        return false;
//    }
    
    handler = CefRefPtr<BrowserHandler>(new BrowserHandler(textureId, width, height));
    
    CefWindowInfo window_info;
    window_info.SetAsWindowless(nullptr);
    //window_info.shared_texture_enabled
    window_info.windowless_rendering_enabled = true;
    CefBrowserHost::CreateBrowser(window_info, handler, !pendingUrl.empty() ? pendingUrl : AppState::getInstance()->config.homepage, browser_settings, nullptr, request_context);
    pendingUrl = "";
    
    return true;
}

void Browser::updateGPSLocation() {
    if (!handler || !handler->browserInstance) {
        return;
    }
    
    float latitude = Dataref::getInstance()->get<float>("sim/flightmodel/position/latitude");
    float longitude = Dataref::getInstance()->get<float>("sim/flightmodel/position/longitude");
    float speedMetersSecond = Dataref::getInstance()->get<float>("sim/flightmodel/position/groundspeed");
    float altitudeMeters = Dataref::getInstance()->get<float>("sim/flightmodel/position/y_agl");
    float magneticHeading = Dataref::getInstance()->get<float>("sim/flightmodel/position/mag_psi");
    
    std::stringstream stream;
    stream << "window.avitab_location = { ";
    stream << "coords: { ";
    stream << "latitude: " << std::fixed << std::setprecision(6) << latitude << ", ";
    stream << "longitude: " << std::fixed << std::setprecision(6) << longitude << ", ";
    stream << "accuracy: 10, ";
    stream << "altitude: " << std::fixed << std::setprecision(0) << altitudeMeters << ", ";
    stream << "altitudeAccuracy: 10, ";
    stream << "heading: " << std::fixed << std::setprecision(0) << magneticHeading << ", ";
    stream << "speed: " << std::fixed << std::setprecision(0) << speedMetersSecond << ", ";
    stream <<  "}, timestamp: Date.now() }; for (let key in window.avitab_watchers) { window.avitab_watchers[key](window.avitab_location); }";
    
    handler->browserInstance->GetMainFrame()->ExecuteJavaScript(stream.str(), "about:blank", 0);
    lastGpsUpdateTime = XPLMGetElapsedTime();
}
