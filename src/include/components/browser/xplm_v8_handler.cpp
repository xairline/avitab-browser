//
//  xplm_v8_handler.cpp
//  Avitab-Browser
//
//  Created by Di Zou on 2025-01-08.
//

#include "xplm_v8_handler.h"


bool XplmV8Handler::Execute(const CefString& name, CefRefPtr< CefV8Value > object, const CefV8ValueList& arguments, CefRefPtr< CefV8Value >& retval, CefString& exception){
    // Example debug message
    std::string debugMessage = "Execute method called with name: " + name.ToString();

    // Send the debug message to X-Plane's log
    XPLMDebugString((debugMessage + "\n").c_str());

//    // Optionally, handle your logic here
//    if (!arguments.empty()) {
//        std::string argDebug = "First argument value: " + arguments[0]->GetStringValue().ToString();
//        XPLMDebugString((argDebug + "\n").c_str());
//    }
    return false;
}
