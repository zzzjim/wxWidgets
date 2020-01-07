/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/window.cpp
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

#include "wx/window.h"

#ifndef WX_PRECOMP
    #include "wx/hash.h"
    #include "wx/log.h"
    #include "wx/app.h"
    #include "wx/utils.h"
    #include "wx/panel.h"
    #include "wx/frame.h"
    #include "wx/dc.h"
    #include "wx/dcclient.h"
    #include "wx/button.h"
    #include "wx/menu.h"
    #include "wx/dialog.h"
    #include "wx/timer.h"
    #include "wx/settings.h"
    #include "wx/msgdlg.h"
    #include "wx/scrolbar.h"
    #include "wx/listbox.h"
    #include "wx/scrolwin.h"
    #include "wx/layout.h"
    #include "wx/menuitem.h"
    #include "wx/module.h"
#endif

#include "wx/fontutil.h"

#if wxUSE_DRAG_AND_DROP
    #include "wx/dnd.h"
#endif

#include <string.h>

// ----------------------------------------------------------------------------
// global variables for this module
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------
wxIMPLEMENT_ABSTRACT_CLASS(wxWindowWASM, wxWindowBase);

wxBEGIN_EVENT_TABLE(wxWindowWASM, wxWindowBase)
    EVT_SYS_COLOUR_CHANGED(wxWindowWASM::OnSysColourChanged)
wxEND_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// helper functions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// constructors
// ----------------------------------------------------------------------------

void wxWindowWASM::Init()
{
    m_mainWindow = (WXWindow) 0;
    m_clientWindow = (WXWindow) 0;
    m_insertIntoMain = false;
    m_updateNcArea = false;

    m_winCaptured = false;
    m_needsInputFocus = false;
    m_isShown = true;
    m_lastTS = 0;
    m_lastButton = 0;
    printf("wxWindowWASM::Init\n");
}

// real construction (Init() must have been called before!)
bool wxWindowWASM::Create(wxWindow *parent, wxWindowID id,
                         const wxPoint& pos,
                         const wxSize& size,
                         long style,
                         const wxString& name)
{
    wxCHECK_MSG( parent, false, wxT("can't create wxWindow without parent") );
    CreateBase(parent, id, pos, size, style, wxDefaultValidator, name);

    parent->AddChild(this);

    return true;
}

// Destructor
wxWindowWASM::~wxWindowWASM()
{
}

// ---------------------------------------------------------------------------
// basic operations
// ---------------------------------------------------------------------------

void wxWindowWASM::SetFocus()
{
}

// Kill focus
void wxWindowWASM::KillFocus()
{
}

// Get the window with the focus
wxWindow *wxWindowBase::DoFindFocus()
{
	return NULL;
}

// Enabling/disabling handled by event loop, and not sending events
// if disabled.
bool wxWindowWASM::Enable(bool enable)
{
    if ( !wxWindowBase::Enable(enable) )
        return false;

    return true;
}

bool wxWindowWASM::Show(bool show)
{
    wxWindowBase::Show(show);
    return true;
}

// Raise the window to the top of the Z order
void wxWindowWASM::Raise()
{
}

// Lower the window to the bottom of the Z order
void wxWindowWASM::Lower()
{
}

void wxWindowWASM::DoCaptureMouse()
{
}

void wxWindowWASM::DoReleaseMouse()
{
}

bool wxWindowWASM::SetFont(const wxFont& font)
{
    if ( !wxWindowBase::SetFont(font) )
    {
        // nothing to do
        return false;
    }

    return true;
}

bool wxWindowWASM::SetCursor(const wxCursor& cursor)
{
    if ( !wxWindowBase::SetCursor(cursor) )
    {
        // no change
        return false;
    }

    return true;
}

// Coordinates relative to the window
void wxWindowWASM::WarpPointer (int x, int y)
{
}

// Does a physical scroll
void wxWindowWASM::ScrollWindow(int dx, int dy, const wxRect *rect)
{
}

void wxWindowWASM::SetScrollbar( int orient, int pos, int thumbVisible,
                               int range, bool refresh)
{
	printf("wxWindowWASM::SetScrollbar\n");
}

// ---------------------------------------------------------------------------
// drag and drop
// ---------------------------------------------------------------------------

#if wxUSE_DRAG_AND_DROP

void wxWindowWASM::SetDropTarget(wxDropTarget * WXUNUSED(pDropTarget))
{
    // TODO
}

#endif

// Old style file-manager drag&drop
void wxWindowWASM::DragAcceptFiles(bool WXUNUSED(accept))
{
    // TODO
}

// ----------------------------------------------------------------------------
// tooltips
// ----------------------------------------------------------------------------

#if wxUSE_TOOLTIPS

void wxWindowWASM::DoSetToolTip(wxToolTip * WXUNUSED(tooltip))
{
    // TODO
}

#endif // wxUSE_TOOLTIPS

// ---------------------------------------------------------------------------
// moving and resizing
// ---------------------------------------------------------------------------

bool wxWindowWASM::PreResize()
{
    return true;
}

// Get total size
void wxWindowWASM::DoGetSize(int *x, int *y) const
{
}

void wxWindowWASM::DoGetPosition(int *x, int *y) const
{
}

void wxWindowWASM::DoScreenToClient(int *x, int *y) const
{
}

void wxWindowWASM::DoClientToScreen(int *x, int *y) const
{
}

// Get size *available for subwindows* i.e. excluding menu bar etc.
void wxWindowWASM::DoGetClientSize(int *x, int *y) const
{
}

void wxWindowWASM::DoSetSize(int x, int y, int width, int height, int sizeFlags)
{
}

void wxWindowWASM::DoSetClientSize(int width, int height)
{
}

void wxWindowWASM::DoMoveWindow(int x, int y, int width, int height)
{
}

void wxWindowWASM::DoSetSizeHints(int minW, int minH, int maxW, int maxH, int incW, int incH)
{
}

// ---------------------------------------------------------------------------
// text metrics
// ---------------------------------------------------------------------------

int wxWindowWASM::GetCharHeight() const
{
    return 10;
}

int wxWindowWASM::GetCharWidth() const
{
    return 10;
}

void wxWindowWASM::DoGetTextExtent(const wxString& string,
                                  int *x, int *y,
                                  int *descent,
                                  int *externalLeading,
                                  const wxFont *theFont) const
{
}

// ----------------------------------------------------------------------------
// painting
// ----------------------------------------------------------------------------

void wxWindowWASM::Refresh(bool eraseBack, const wxRect *rect)
{
}

void wxWindowWASM::Update()
{
}

void wxWindowWASM::SendEraseEvents()
{
}

void wxWindowWASM::SendPaintEvents()
{
}

void wxWindowWASM::SendNcPaintEvents()
{
}

// ----------------------------------------------------------------------------
// event handlers
// ----------------------------------------------------------------------------

// Responds to colour changes: passes event on to children.
void wxWindowWASM::OnSysColourChanged(wxSysColourChangedEvent& event)
{
    wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
    while ( node )
    {
        // Only propagate to non-top-level windows
        wxWindow *win = node->GetData();
        if ( win->GetParent() )
        {
            wxSysColourChangedEvent event2;
            event.SetEventObject(win);
            win->HandleWindowEvent(event2);
        }

        node = node->GetNext();
    }
}

void wxWindowWASM::OnInternalIdle()
{
    printf("wxWindowWASM::OnInternalIdle\n");
}

// ----------------------------------------------------------------------------
// Colour stuff
// ----------------------------------------------------------------------------

bool wxWindowWASM::SetBackgroundColour(const wxColour& col)
{
    wxWindowBase::SetBackgroundColour(col);
    return true;
}

bool wxWindowWASM::SetForegroundColour(const wxColour& col)
{
    if ( !wxWindowBase::SetForegroundColour(col) )
        return false;

    return true;
}

// ----------------------------------------------------------------------------
// global functions
// ----------------------------------------------------------------------------

wxWindow * wxGetActiveWindow()
{
    return NULL;
}

/* static */
wxWindow *wxWindowBase::GetCapture()
{
    return NULL;
}

wxWindow* wxFindWindowAtPointer(wxPoint& pt)
{
    return NULL;
}
wxWindow* wxFindWindowAtPoint(const wxPoint& pt)
{
    return NULL;
}

void wxGetMouseState(int& rootX, int& rootY, unsigned& maskReturn)
{
}

wxPoint wxGetMousePosition()
{
	return wxPoint(0, 0);
}

wxMouseState wxGetMouseState()
{
    return wxMouseState();
}


class wxWinModule : public wxModule
{
public:
    wxWinModule()
    {
    }

    virtual bool OnInit();
    virtual void OnExit();

private:
    wxDECLARE_DYNAMIC_CLASS(wxWinModule);
};

wxIMPLEMENT_DYNAMIC_CLASS(wxWinModule, wxModule);

bool wxWinModule::OnInit()
{
    return true;
}

void wxWinModule::OnExit()
{
}

