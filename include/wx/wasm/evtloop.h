///////////////////////////////////////////////////////////////////////////////
// Name:        wx/wasm/evtloop.h
// Purpose:     declares wxEventLoop class
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WASM_EVTLOOP_H_
#define _WX_WASM_EVTLOOP_H_

#if wxUSE_CONSOLE_EVENTLOOP

#include "wx/evtloop.h"

// ----------------------------------------------------------------------------
// wxConsoleEventLoop
// ----------------------------------------------------------------------------

// Note: if/when emscripten select/poll/epoll works on fd's this can
// 	can use wxUnixConcosleEventLoop as the base class and do nothing
// 	also remove the code that disallows adding an fd input in wasm/utils.cpp
class wxConsoleEventLoop;
class wxEventLoopSource;

class WXDLLIMPEXP_BASE wxWASMConsoleEventLoop : public wxEventLoopManual
{
public:
    wxWASMConsoleEventLoop();
    virtual ~wxWASMConsoleEventLoop();

    virtual void DoYieldFor(long eventsToProcess) wxOVERRIDE;
    virtual bool Pending() const wxOVERRIDE;
    virtual bool Dispatch() wxOVERRIDE;
    virtual int DispatchTimeout(unsigned long timeout) wxOVERRIDE;
    virtual void WakeUp() wxOVERRIDE;
private:
    // thread and condition to wakeup from threads
    wxSemaphore m_wakeSem;
    wxDECLARE_NO_COPY_CLASS(wxWASMConsoleEventLoop);
};

#if wxUSE_GUI

class WXDLLIMPEXP_CORE wxGUIEventLoop : public wxWASMConsoleEventLoop 
{
public:
    wxGUIEventLoop();
};

#endif // wxUSE_GUI

#endif // wxUSE_CONSOLE_EVENTLOOP

#endif // _WX_WASM_EVTLOOP_H_

