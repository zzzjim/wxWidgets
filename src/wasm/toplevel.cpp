///////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/toplevel.cpp
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/toplevel.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/string.h"
    #include "wx/log.h"
    #include "wx/intl.h"
    #include "wx/frame.h"
    #include "wx/menu.h"
    #include "wx/statusbr.h"
    #include "wx/settings.h"
#endif //WX_PRECOMP

// ----------------------------------------------------------------------------
// wxTopLevelWindowWASM creation
// ----------------------------------------------------------------------------

void wxTopLevelWindowWASM::Init()
{
    m_iconized =
    m_maximizeOnShow = false;

    // unlike (almost?) all other windows, frames are created hidden
    m_isShown = false;

    // Data to save/restore when calling ShowFullScreen
    m_fsStyle = 0;
    m_fsIsMaximized = false;
    m_fsIsShowing = false;

    m_needResizeInIdle = false;

    m_x = wxDefaultCoord;
    m_y = wxDefaultCoord;
    m_width = 20;
    m_height = 20;
    printf("wxTopLevelWindowWASM::Init\n");
}

bool wxTopLevelWindowWASM::Create(wxWindow *parent,
                                 wxWindowID id,
                                 const wxString& title,
                                 const wxPoint& pos,
                                 const wxSize& size,
                                 long style,
                                 const wxString& name)
{
    Init();

    SetTitle(title);

    return true;
}

wxTopLevelWindowWASM::~wxTopLevelWindowWASM()
{
    wxTopLevelWindows.DeleteObject(this);

    // If this is the last top-level window, exit.
    if ( wxTheApp && (wxTopLevelWindows.GetCount() == 0) )
    {
        wxTheApp->SetTopWindow(NULL);

        if (wxTheApp->GetExitOnFrameDelete())
        {
            // Signal to the app that we're going to close
            wxTheApp->ExitMainLoop();
        }
    }
}

void wxTopLevelWindowWASM::OnInternalIdle()
{
    wxWindow::OnInternalIdle();

    // Do this only after the last idle event so that
    // all windows have been updated before a new
    // round of size events is sent
    if (m_needResizeInIdle && !wxTheApp->Pending())
    {
        wxSizeEvent event( GetClientSize(), GetId() );
        event.SetEventObject( this );
        HandleWindowEvent( event );

        m_needResizeInIdle = false;
    }
}

// ----------------------------------------------------------------------------
// wxTopLevelWindowX11 showing
// ----------------------------------------------------------------------------

bool wxTopLevelWindowWASM::Show(bool show)
{
    if (show)
    {
        wxSizeEvent event(GetSize(), GetId());

        event.SetEventObject(this);
        HandleWindowEvent(event);

        m_needResizeInIdle = false;
    }

    bool ret = wxWindowWASM::Show(show);

    return ret;
}

// ----------------------------------------------------------------------------
// wxTopLevelWindowX11 maximize/minimize
// ----------------------------------------------------------------------------

void wxTopLevelWindowWASM::Maximize(bool WXUNUSED(maximize))
{
    // TODO
}

bool wxTopLevelWindowWASM::IsMaximized() const
{
    // TODO
    return true;
}

void wxTopLevelWindowWASM::Iconize(bool iconize)
{
    if ( !iconize )
    {
        Restore();
        return;
    }

}

bool wxTopLevelWindowWASM::IsIconized() const
{
    return m_iconized;
}

void wxTopLevelWindowWASM::Restore()
{
}

// ----------------------------------------------------------------------------
// wxTopLevelWindowX11 fullscreen
// ----------------------------------------------------------------------------

bool wxTopLevelWindowWASM::ShowFullScreen(bool show, long style)
{
	printf("WARN: This may be possible with SDL\n");
	return false;
}

// ----------------------------------------------------------------------------
// wxTopLevelWindowX11 misc
// ----------------------------------------------------------------------------

void wxTopLevelWindowWASM::DoSetIcon(const wxIcon& icon)
{
}

void wxTopLevelWindowWASM::SetIcons(const wxIconBundle& icons )
{
}

bool wxTopLevelWindowWASM::SetShape(const wxRegion& region)
{
	return false;
}

void wxTopLevelWindowWASM::SetTitle(const wxString& title)
{
}

wxString wxTopLevelWindowWASM::GetTitle() const
{
    return m_title;
}

// For implementation purposes - sometimes decorations make the client area
// smaller
wxPoint wxTopLevelWindowWASM::GetClientAreaOrigin() const
{
    // wxFrame::GetClientAreaOrigin
    // does the required calculation already.
    return wxPoint(0, 0);
}

void wxTopLevelWindowWASM::DoGetClientSize( int *width, int *height ) const
{
    if (width)
       *width = m_width;
    if (height)
       *height = m_height;
}

void wxTopLevelWindowWASM::DoGetSize( int *width, int *height ) const
{
    // TODO add non-client size

    if (width)
       *width = m_width;
    if (height)
       *height = m_height;
}

void wxTopLevelWindowWASM::DoSetClientSize(int width, int height)
{
    int old_width = m_width;
    int old_height = m_height;

    m_width = width;
    m_height = height;

    if (m_width == old_width && m_height == old_height)
        return;

    // wxLogDebug("DoSetClientSize: %s (%ld) %dx%d", GetClassInfo()->GetClassName(), GetId(), width, height);


    wxWindowWASM::DoSetClientSize(width, height);
}

void wxTopLevelWindowWASM::DoSetSize(int x, int y, int width, int height, int sizeFlags)
{
}

void wxTopLevelWindowWASM::DoGetPosition(int *x, int *y) const
{
}


