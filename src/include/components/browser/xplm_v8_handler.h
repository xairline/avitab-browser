//
//  xplm_v8_handler.h
//  Avitab-Browser
//
//  Created by Di Zou on 2025-01-08.
//
#include <XPLMUtilities.h>
#include <include/cef_base.h>
#include <include/cef_v8.h>

class XplmV8Handler : public CefV8Handler {
  private:
    IMPLEMENT_REFCOUNTING(XplmV8Handler);

  public:
    bool Execute(const CefString &name, CefRefPtr<CefV8Value> object,
                 const CefV8ValueList &arguments, CefRefPtr<CefV8Value> &retval,
                 CefString &exception) override;
};
