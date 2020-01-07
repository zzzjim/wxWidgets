/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/settings.cpp
// Purpose:     wxSettings
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/settings.h"

#ifndef WX_PRECOMP
    #include "wx/gdicmn.h"
#endif

wxColour wxSystemSettingsNative::GetColour(wxSystemColour index)
{
    switch (index)
    {
        case wxSYS_COLOUR_APPWORKSPACE:
            return wxColour( 0xc0c0c0 );

        default:
            break;
    }

    return *wxWHITE;
}

wxFont wxSystemSettingsNative::GetFont(wxSystemFont index)
{
    switch (index)
    {
        case wxSYS_SYSTEM_FIXED_FONT:
            return wxFontInfo(12).Family(wxFONTFAMILY_MODERN);

        case wxSYS_DEVICE_DEFAULT_FONT:
        case wxSYS_SYSTEM_FONT:
        case wxSYS_DEFAULT_GUI_FONT:
        default:
            return wxFontInfo(12).Family(wxFONTFAMILY_SWISS);
    }

    return wxFont();
}

int wxSystemSettingsNative::GetMetric(wxSystemMetric index, wxWindow* WXUNUSED(win))
{
    switch ( index)
    {
        default:
            return -1;  // unsupported metric
    }
}

bool wxSystemSettingsNative::HasFeature(wxSystemFeature index)
{
    switch (index)
    {
        default:
            return false;
    }
}
