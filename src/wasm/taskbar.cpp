/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/taskbar.cpp
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/event.h"
#include "wx/taskbar.h"

#if wxUSE_TASKBARICON

//=============================================================================

bool wxTaskBarIconBase::IsAvailable()
{
    return false;
}

//=============================================================================

wxIMPLEMENT_DYNAMIC_CLASS(wxTaskBarIcon, wxEvtHandler);

wxTaskBarIcon::wxTaskBarIcon(wxTaskBarIconType WXUNUSED(iconType))
{
}

wxTaskBarIcon::~wxTaskBarIcon()
{
}

bool wxTaskBarIcon::SetIcon(const wxIcon& WXUNUSED(icon),
             const wxString& WXUNUSED(tooltip))
{
    return false;
}

bool wxTaskBarIcon::RemoveIcon()
{
    return false;
}

bool wxTaskBarIcon::PopupMenu(wxMenu *WXUNUSED(menu))
{
    return false;
}

#endif //wxUSE_TASKBARICON
