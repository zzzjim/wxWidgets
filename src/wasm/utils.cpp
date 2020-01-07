/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/utils.cpp
// Purpose:     Various utilities
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if defined(__BORLANDC__)
    #pragma hdrstop
#endif

#include "wx/app.h"
#include "wx/utils.h"
#include "wx/apptrait.h"
#include "wx/evtloop.h"

#include "wx/apptrait.h"
#include "wx/scopedptr.h"
#include "wx/thread.h"
#include "wx/module.h"
#include "wx/unix/private/timer.h"
#include "wx/unix/private/epolldispatcher.h"
#include "wx/unix/private/wakeuppipe.h"
#include "wx/private/selectdispatcher.h"
#include "wx/private/eventloopsourcesmanager.h"
#include "wx/private/fdioeventloopsourcehandler.h"
#include "wx/private/eventloopsourcesmanager.h"


void wxBell()
{
}

#if wxUSE_EVENTLOOP_SOURCE
class wxWASMEventLoopSourcesManager : public wxEventLoopSourcesManagerBase
{
public:
    wxEventLoopSource *
    AddSourceForFD(int fd, wxEventLoopSourceHandler* handler, int flags)
    {
/*
        // leaving this here, in the future when select blocks this will be usable
        wxCHECK_MSG( fd != -1, NULL, "can't monitor invalid fd" );

        wxLogTrace(wxTRACE_EVT_SOURCE,
                    "Adding event loop source for fd=%d", fd);

        wxScopedPtr<wxFDIOHandler>
            fdioHandler(new wxFDIOEventLoopSourceHandler(handler));

        if ( !wxFDIODispatcher::Get()->RegisterFD(fd, fdioHandler.get(), flags) )
            return NULL;

        return new wxUnixEventLoopSource(wxFDIODispatcher::Get(), fdioHandler.release(),
                                         fd, handler, flags);
*/
        return NULL;
    }
};

wxEventLoopSourcesManagerBase* wxGUIAppTraits::GetEventLoopSourcesManager()
{
    static wxWASMEventLoopSourcesManager s_eventLoopSourcesManager;

    return &s_eventLoopSourcesManager;
}

#endif // wxUSE_EVENTLOOP_SOURCE

wxTimerImpl *wxGUIAppTraits::CreateTimerImpl(wxTimer *timer)
{
    return new wxUnixTimerImpl(timer);
}


bool wxGetKeyState(wxKeyCode key)
{
    // track this with sdl or a global key event listener
    switch ( key )
    {
        default:
            return false;
    }
}

void wxGetMousePosition( int* x, int* y )
{
    *x = 0;
    *y = 0;
}


