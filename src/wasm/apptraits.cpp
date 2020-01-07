/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/apptraits.cpp
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#include "wx/apptrait.h"
#include "wx/stdpaths.h"
#include "wx/evtloop.h"

#include "wx/timer.h"
#include "wx/unix/private/timer.h"

wxEventLoopBase * wxGUIAppTraits::CreateEventLoop()
{
    return new wxGUIEventLoop();
}

wxPortId wxGUIAppTraits::GetToolkitVersion(int *majVer,
                                           int *minVer,
                                           int *microVer) const
{
    if ( majVer )
        *majVer = 0;
    if ( minVer )
        *minVer = 0;
    if ( microVer )
        *microVer = 1;

    return wxPORT_UNKNOWN;
}
