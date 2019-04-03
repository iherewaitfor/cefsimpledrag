// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "cefsimple/simple_handler.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

namespace {

SimpleHandler* g_instance = NULL;

}  // namespace

SimpleHandler::SimpleHandler(bool use_views)
    : use_views_(use_views), is_closing_(false) {
  DCHECK(!g_instance);
  g_instance = this;
}

SimpleHandler::~SimpleHandler() {
  g_instance = NULL;
}

// static
SimpleHandler* SimpleHandler::GetInstance() {
  return g_instance;
}

void SimpleHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                  const CefString& title) {
  CEF_REQUIRE_UI_THREAD();

  if (use_views_) {
    // Set the title of the window using the Views framework.
    CefRefPtr<CefBrowserView> browser_view =
        CefBrowserView::GetForBrowser(browser);
    if (browser_view) {
      CefRefPtr<CefWindow> window = browser_view->GetWindow();
      if (window)
        window->SetTitle(title);
    }
  } else {
    // Set the title of the window using platform APIs.
    PlatformTitleChange(browser, title);
  }
}


WNDPROC m_windowProcPrev;
static int     cxClient, cyClient;
#define DIVISIONS 5
static BOOL state[DIVISIONS][DIVISIONS];
std::map< HWND, WNDPROC> wndProcmap;

static HWND g_browserHwnd;

static const UINT_PTR IDT_TIMER1 = 1;

#define CAPTION_X 500
#define CAPTION_Y 200

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MyWndProcChild(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK hookChild(HWND hWnd, LPARAM lParam) {
	if (wndProcmap.find(hWnd) == wndProcmap.end()) {

		LONG_PTR Style = ::GetWindowLongPtr(hWnd, GWL_STYLE);
		Style = Style &~WS_CAPTION  &~WS_SYSMENU &~WS_SIZEBOX & ~WS_VSCROLL & ~WS_HSCROLL;
		::SetWindowLongPtr(hWnd, GWL_STYLE, Style);

		m_windowProcPrev = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG)MyWndProcChild);
		wndProcmap[hWnd] = m_windowProcPrev;
		EnumChildWindows(hWnd, hookChild, lParam);
	}
	return TRUE;
}


LRESULT CALLBACK MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
		if (wParam == IDT_TIMER1) {
			EnumChildWindows(hWnd, hookChild, 0);
			KillTimer(hWnd, IDT_TIMER1);
			return 0;
		}
		break;
	case WM_NCLBUTTONDBLCLK:
		return 0;
	case WM_NCHITTEST:
		POINT p{ (SHORT)LOWORD(lParam), (SHORT)HIWORD(lParam) };
		MapWindowPoints(HWND_DESKTOP, hWnd, &p, 1);
		if (p.x < CAPTION_X && p.y < CAPTION_Y) {
			return HTCAPTION;
		}
		POINT pt;
		pt.x = (int)(short)LOWORD(lParam);
		pt.y = (int)(short)HIWORD(lParam);
		::ScreenToClient(hWnd, &pt);
		RECT rcClient = { 0 };
		::GetClientRect(hWnd, &rcClient);
		if (pt.x<rcClient.left + 2 && pt.y<rcClient.top + 2)//左上角,判断是不是在左上角，就是看当前坐标是不是即在左边拖动的范围内，又在上边拖动的范围内，其它角判断方法类似
		{
			return HTTOPLEFT;
		}
		else if (pt.x>rcClient.right - 2 && pt.y<rcClient.top + 2)//右上角
		{
			return HTTOPRIGHT;
		}
		else if (pt.x<rcClient.left + 2 && pt.y>rcClient.bottom - 2)//左下角
		{
			return HTBOTTOMLEFT;
		}
		else if (pt.x>rcClient.right - 2 && pt.y>rcClient.bottom - 2)//右下角
		{
			return HTBOTTOMRIGHT;
		}
		else if (pt.x<rcClient.left + 2)
		{
			return HTLEFT;
		}
		else if (pt.x>rcClient.right - 2)
		{
			return HTRIGHT;
		}
		else if (pt.y<rcClient.top + 2)
		{
			return HTTOP;
		}if (pt.y>rcClient.bottom - 2)
		{
			return HTBOTTOM;          //以上这四个是上、下、左、右四个边
		}
		break;
	}
	m_windowProcPrev = wndProcmap[hWnd];
	return CallWindowProc(m_windowProcPrev, hWnd, message, wParam, lParam);
}
LRESULT CALLBACK MyWndProcChild(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_NCLBUTTONDBLCLK:
		return 0;
	case WM_NCHITTEST:

		POINT p{ (SHORT)LOWORD(lParam), (SHORT)HIWORD(lParam) };
		MapWindowPoints(HWND_DESKTOP, hWnd, &p, 1);
		if (p.x < CAPTION_X && p.y < CAPTION_Y) {
			return HTTRANSPARENT;
		}
		POINT pt;
		pt.x = (int)(short)LOWORD(lParam);
		pt.y = (int)(short)HIWORD(lParam);
		::ScreenToClient(hWnd, &pt);
		RECT rcClient = { 0 };
		::GetClientRect(hWnd, &rcClient);
		if (pt.x<rcClient.left + 2 && pt.y<rcClient.top + 2)//左上角,判断是不是在左上角，就是看当前坐标是不是即在左边拖动的范围内，又在上边拖动的范围内，其它角判断方法类似
		{
			return HTTRANSPARENT;
		}
		else if (pt.x>rcClient.right - 2 && pt.y<rcClient.top + 2)//右上角
		{
			return HTTRANSPARENT;
		}
		else if (pt.x<rcClient.left + 2 && pt.y>rcClient.bottom - 2)//左下角
		{
			return HTTRANSPARENT;
		}
		else if (pt.x>rcClient.right - 2 && pt.y>rcClient.bottom - 2)//右下角
		{
			return HTTRANSPARENT;
		}
		else if (pt.x<rcClient.left + 2)
		{
			return HTTRANSPARENT;
		}
		else if (pt.x>rcClient.right - 2)
		{
			return HTTRANSPARENT;
		}
		else if (pt.y<rcClient.top + 2)
		{
			return HTTRANSPARENT;
		}if (pt.y>rcClient.bottom - 2)
		{
			return HTTRANSPARENT;          //以上这四个是上、下、左、右四个边
		}
		break;
	}
	m_windowProcPrev = wndProcmap[hWnd];
	return CallWindowProc(m_windowProcPrev, hWnd, message, wParam, lParam);
}
// Define a function.
void MyFunc(HWND hwnd, HANDLE waitEvent) {
	//CEF_REQUIRE_UI_THREAD();
	m_windowProcPrev = (WNDPROC)::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG)MyWndProc);
	wndProcmap[hwnd] = m_windowProcPrev;
	EnumChildWindows(hwnd, hookChild, (LONG_PTR)NULL);
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  HWND hwnd = browser->GetHost()->GetWindowHandle();
  g_browserHwnd = hwnd;
  m_windowProcPrev = (WNDPROC)::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG)MyWndProc);
  wndProcmap[hwnd] = m_windowProcPrev;
  //EnumChildWindows(hwnd, hookChild, (LONG_PTR)this);
  SetTimer(hwnd,             // handle to main window 
	  IDT_TIMER1,            // timer identifier 
	  50,                 // 10-second interval 
	  (TIMERPROC)NULL);     // no timer callback 

  LONG_PTR Style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
  //Style = Style &~WS_CAPTION &~WS_SYSMENU &~WS_SIZEBOX;
  Style = Style &~WS_CAPTION  &~WS_SYSMENU &~WS_SIZEBOX & ~WS_VSCROLL;
  Style = Style &~WS_SYSMENU &~WS_SIZEBOX & ~WS_VSCROLL;
  ::SetWindowLongPtr(hwnd, GWL_STYLE, Style);

  // Add to the list of existing browsers.
  browser_list_.push_back(browser);
}

bool SimpleHandler::DoClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Closing the main window requires special handling. See the DoClose()
  // documentation in the CEF header for a detailed destription of this
  // process.
  if (browser_list_.size() == 1) {
    // Set a flag to indicate that the window close should be allowed.
    is_closing_ = true;
  }

  // Allow the close. For windowed browsers this will result in the OS close
  // event being sent.
  return false;
}

void SimpleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Remove from the list of existing browsers.
  BrowserList::iterator bit = browser_list_.begin();
  for (; bit != browser_list_.end(); ++bit) {
    if ((*bit)->IsSame(browser)) {
      browser_list_.erase(bit);
      break;
    }
  }

  if (browser_list_.empty()) {
    // All browser windows have closed. Quit the application message loop.
    CefQuitMessageLoop();
  }
}

void SimpleHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& errorText,
                                const CefString& failedUrl) {
  CEF_REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;

  // Display a load error message.
  std::stringstream ss;
  ss << "<html><body bgcolor=\"white\">"
        "<h2>Failed to load URL "
     << std::string(failedUrl) << " with error " << std::string(errorText)
     << " (" << errorCode << ").</h2></body></html>";
  frame->LoadString(ss.str(), failedUrl);
}

void SimpleHandler::CloseAllBrowsers(bool force_close) {
  if (!CefCurrentlyOn(TID_UI)) {
    // Execute on the UI thread.
    CefPostTask(TID_UI, base::Bind(&SimpleHandler::CloseAllBrowsers, this,
                                   force_close));
    return;
  }

  if (browser_list_.empty())
    return;

  BrowserList::const_iterator it = browser_list_.begin();
  for (; it != browser_list_.end(); ++it)
    (*it)->GetHost()->CloseBrowser(force_close);
}
