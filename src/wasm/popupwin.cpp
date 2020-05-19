/////////////////////////////////////////////////////////////////////////////
// Name:        srx/wasm/popupwin.cpp
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_POPUPWIN

#include "wx/popupwin.h"

#ifndef WX_PRECOMP
    #include "wx/log.h"
    #include "wx/app.h"
    #include "wx/settings.h"
#endif

//-----------------------------------------------------------------------------
// wxPopupWindow
//-----------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(wxPopupWindow,wxPopupWindowBase)
wxEND_EVENT_TABLE()

wxPopupWindow::~wxPopupWindow()
{
}

bool wxPopupWindow::Create( wxWindow *parent, int style )
{
    if (!CreateBase( parent, -1, wxDefaultPosition, wxDefaultSize, style, wxDefaultValidator, wxT("popup") ))
    {
        wxFAIL_MSG( wxT("wxPopupWindow creation failed") );
        return false;
    }

    return true;
}

void wxPopupWindow::DoMoveWindow(int x, int y, int width, int height )
{
    wxWindowWASM::DoMoveWindow( x, y, width, height );
}

void wxPopupWindow::DoSetSize( int x, int y, int width, int height, int sizeFlags )
{
    wxWindowWASM::DoSetSize(x, y, width, height, sizeFlags);
}

bool wxPopupWindow::Show( bool show )
{
    bool ret = wxWindowWASM::Show( show );

    Raise();

    return ret;
}

#endif // wxUSE_POPUPWIN
