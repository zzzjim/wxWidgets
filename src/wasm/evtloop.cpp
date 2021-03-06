/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/evtloop.cpp
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/app.h"
#include "wx/apptrait.h"
#include "wx/evtloop.h"
#include "wx/thread.h"
#include "wx/unix/private/timer.h"
#include "wx/private/eventloopsourcesmanager.h"

#include <iostream>

#if wxUSE_GUI
wxGUIEventLoop::wxGUIEventLoop()
{

}
#endif // wxUSE_GUI


wxWASMConsoleEventLoop::wxWASMConsoleEventLoop() :  m_wakeMutex(), m_wakeCondition(m_wakeMutex)
{
    wxLog* logger = new wxLogStream(&std::cerr);
    wxLog::SetActiveTarget(logger);

    m_wakeMutex.Lock();

}
wxWASMConsoleEventLoop::~wxWASMConsoleEventLoop()
{
}

bool wxWASMConsoleEventLoop::Pending() const
{
#if wxUSE_TIMER
    wxUsecClock_t nextTimer;
    if ( wxTimerScheduler::Get().GetNext(&nextTimer) &&
            !wxMilliClockToLong(nextTimer) )
        return true;
#endif // wxUSE_TIMER

    return false;
}

bool wxWASMConsoleEventLoop::Dispatch()
{
    DispatchTimeout(100000);
    return true;
}

int wxWASMConsoleEventLoop::DispatchTimeout(unsigned long timeout)
{
#if wxUSE_TIMER
    // check if we need to decrease the timeout to account for a timer
    wxUsecClock_t nextTimer;
    if ( wxTimerScheduler::Get().GetNext(&nextTimer) )
    {
        unsigned long timeUntilNextTimer = wxMilliClockToLong(nextTimer / 1000);
        unsigned long something = wxMilliClockToLong(nextTimer);
        if ( timeUntilNextTimer < timeout )
        {
            timeout = timeUntilNextTimer;
        }
    }
#endif // wxUSE_TIMER
    wxCondError hadEvent;
    if ( timeout == 0 || timeout == -1 )
    {
        hadEvent = m_wakeCondition.WaitTimeout(1000000);
    }
    else
    {
        hadEvent = m_wakeCondition.WaitTimeout(timeout);
    }

#if wxUSE_TIMER
    if ( wxTimerScheduler::Get().NotifyExpired() )
    {
        hadEvent = wxCOND_NO_ERROR;
    }
#endif // wxUSE_TIMER
	
    switch ( hadEvent )
    {
        case wxCOND_TIMEOUT:
        case wxCOND_INVALID:
        case wxCOND_MISC_ERROR:
            return -1;
        case wxCOND_NO_ERROR:
            return 1;
    }
}

void wxWASMConsoleEventLoop::WakeUp()
{
    m_wakeMutex.Unlock();
    m_wakeCondition.Broadcast();
}

void wxWASMConsoleEventLoop::DoYieldFor(long eventsToProcess)
{
    wxEventLoopBase::DoYieldFor(eventsToProcess);
}


/*
#ifdef USE_BASE_LOOP

int wxWASMConsoleEventLoop::DoRun() 
{
	WTRACE("wxWASMConsoleEventLoop::DoRun\n");
    while ( true )
    {
        if ( !Dispatch() )
        {
            break;
        }
    }

    return 0;//m_exitcode;
}

void wxWASMConsoleEventLoop::ScheduleExit(int rc)
{
	WTRACE("wxWASMConsoleEventLoop::ScheduleExit\n");
    wxCHECK_RET( IsInsideRun(), wxT("can't call ScheduleExit() if not running") );

    OnExit();

    WakeUp();
}


#endif // USE BASE LOOP

*/
