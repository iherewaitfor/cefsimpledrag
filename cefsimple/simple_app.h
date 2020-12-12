// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include "include/cef_app.h"
#include"include/cef_v8.h"



class MyV8Handler : public CefV8Handler {
public:
	MyV8Handler() {}

	virtual bool Execute(const CefString& name,
		CefRefPtr<CefV8Value> object,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval,
		CefString& exception) OVERRIDE {
		if (name == "myfunc") {
			// Return my string value.
			retval = CefV8Value::CreateString("My myfunc return Value!");
			return true;
		}

		// Function does not exist.
		return false;
	}

	// Provide the reference counting implementation for this class.
	IMPLEMENT_REFCOUNTING(MyV8Handler);
};


// Implement application-level callbacks for the browser process.
class SimpleApp : public CefApp, public CefBrowserProcessHandler{
 public:
  SimpleApp();

  // CefApp methods:
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
      OVERRIDE {
    return this;
  }

  // CefBrowserProcessHandler methods:
  virtual void OnContextInitialized() OVERRIDE;

 private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(SimpleApp);
};

//render进程的各种回调处理。
class SimpleRenderApp :public CefApp,public  CefRenderProcessHandler {
public:
	SimpleRenderApp():m_cefV8handler(new MyV8Handler){
	}
	// CefApp methods:
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()
		OVERRIDE {
		return this;
	}

	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) OVERRIDE;
private:
	CefRefPtr<MyV8Handler> m_cefV8handler;
	IMPLEMENT_REFCOUNTING(SimpleRenderApp);
};


#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
