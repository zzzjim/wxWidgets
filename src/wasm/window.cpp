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

#include <emscripten.h>
#include <string.h>

#define TRACE(msg...)
//#define TRACE(msg...) printf(msg)

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
    m_isShown = true;

    m_js_id = wxString::Format("wxWindow%d", (int)this);//m_windowId.GetValue());
    m_jid = m_js_id.mb_str();

    TRACE("wxWindowWASM::Init %s\n", m_jid);
}

// real construction (Init() must have been called before!)
bool wxWindowWASM::Create(wxWindow *parent, wxWindowID id,
                         const wxPoint& pos,
                         const wxSize& size,
                         long style,
                         const wxString& name)
{
    SetName(name);

    m_js_id = wxString::Format("wxWindow-%s-%d", name, (int)this);//m_windowId.GetValue());
    m_jid = m_js_id.mb_str();
/*
    wxWindow * tlw = parent;
    while ( 1 ) {
        if ( tlw->GetParent() == NULL )
        {
            break;
        }
        tlw = tlw->GetParent();
    }
    m_js_cid = wxString::Format("%s", tlw->m_cid);
    m_cid = m_js_cid.mb_str();;
 */   

    // one canvas per window
    m_js_cid = wxString::Format("wxWindowDC-%s-%d", name, (int)this);//m_windowId.GetValue());
    m_cid = m_js_cid.mb_str();;

    CreateBase(parent, id, pos, size, style, wxDefaultValidator, name);


    const char * pjid = NULL;
    if ( parent ) {
        pjid = parent->m_jid;
    }

    TRACE("wxWindowWASM::Create:(%s) Parent(%s) CID(%s)\n", m_jid, pjid, m_cid);

    int x = pos.x;
    int y = pos.y;
    int width = size.x;
    int height = size.y;

    wxPoint pos2(pos);
/*
    if (pos2.x == wxDefaultCoord)
        pos2.x = 0;
    if (pos2.y == wxDefaultCoord)
        pos2.y = 0;
*/
    wxSize size2(size);
/*
    if (size2.x <= 0)
        size2.x = 20;
    if (size2.y <= 0)
        size2.y = 20;
*/
    AdjustForParentClientOrigin(pos2.x, pos2.y);

    x = pos2.x;
    y = pos2.y;
    width = size2.x;
    height = size2.y;

    MAIN_THREAD_EM_ASM_INT({
        var parent = document.body;
        var id = UTF8ToString($0);
        var cid = UTF8ToString($1);
        var pid = UTF8ToString($2);
        var x = $3;
        var y = $4;
        var width = $5;
        var height = $6;

        var bgcolor = 'blue';
        if ( pid ) {
            var pnew = document.getElementById(pid);
            if ( pnew != null ) {
                parent = pnew;
                bgcolor = 'red';
                console.log("Found parent div: ", pnew);
            }
        }

        var div = document.createElement('div');
        div.setAttribute('id', id);
	div.style.position = 'absolute';
	div.style.display = 'block';
        div.style.visibility = "visible";
        //div.style.visibility = "none";
        console.log("wxWindowWASM::Create Making Window:" + id + " " + pid + " " + cid + " " + x + " " + y + " " + width + " " + height);
	//div.style.backgroundColor = bgcolor;
	div.style.left = x + "px";
	div.style.top = y + "px";
	div.style.width = width + "px";
	div.style.height = height + "px";

	///////////// SWITCH THIS BACK
        //document.body.appendChild(div);
        parent.appendChild(div);
        console.log("wxWindowWASM::Create Adding window div: ", div);

        var canvas = document.createElement('canvas');
        canvas.setAttribute('id', cid);
	canvas.width = width;
	canvas.height = height;
        console.log("Adding window DC canvas: ", canvas);
        div.appendChild(canvas);
        window.wx.bitmaps.set(cid, canvas);

    }, m_jid, m_cid, pjid, x, y, width, height);

    parent->AddChild(this);
    //Refresh();

/*
    wxWindowCreateEvent event((wxWindow*)this);
    HandleWindowEvent(event);

    wxNcPaintEvent eventNc(GetId());
    eventNc.SetEventObject(this);
    HandleWindowEvent(eventNc);

    wxPaintEvent eventPaint(GetId());
    eventPaint.SetEventObject(this);
    HandleWindowEvent(eventPaint);

    wxSizeEvent eventSize(GetSize(), GetId());
    eventSize.SetEventObject(this);
    HandleWindowEvent(eventSize);
*/

    return true;
}

// Destructor
wxWindowWASM::~wxWindowWASM()
{
    TRACE("wxWindowWASM::~wxWindowWASM\n");

    MAIN_THREAD_EM_ASM_INT({
        var id = UTF8ToString($0);
        var cid = UTF8ToString($1);
        var div = document.getElementById(id);
        window.wx.bitmaps.delete(cid);
        if ( div ) {
            console.log("wxWindowWASM::~wxWindowWASM removed", id, cid);
            div.parentNode.removeChild(div);
        }
        else {
            console.log("wxWindowWASM::~wxWindowWASM can't find", id, cid);
        }
    }, m_jid, m_cid);

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
    TRACE("wxWindowWASM::Show %s %s %d\n", m_jid, m_cid, show);
    if ( !wxWindowBase::Show(show) )
        return false;

    const char * visibility;
    if ( show )
    {
        visibility = "visible";
    }
    else
    {
        visibility = "hidden";
    }
    TRACE("wxWindowWASM::Show %s %s %s\n", m_jid, m_cid, visibility);
    MAIN_THREAD_EM_ASM_INT({
        var div = document.getElementById(UTF8ToString($0));
        div.style.visibility = UTF8ToString($1);
	//div.style.display = 'block';
        console.log("Update Div: " + UTF8ToString($0), div);
    }, m_jid, visibility);

    if ( show )
    {
        Update();
        //wxNcPaintEvent eventNc(GetId());
        //eventNc.SetEventObject(this);
        //HandleWindowEvent(eventNc);
    }
/*
    wxPaintEvent eventPaint(GetId());
    eventPaint.SetEventObject(this);
    HandleWindowEvent(eventPaint);

    wxSizeEvent event(GetSize(), GetId());
    event.SetEventObject(this);
    HandleWindowEvent(event);
*/
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
	TRACE("wxWindowWASM::SetScrollbar\n");
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
//
// Get total size
void wxWindowWASM::DoGetSize(int *x, int *y) const
{
    *x = MAIN_THREAD_EM_ASM_INT({
        var div = document.getElementById(UTF8ToString($0));
        var width = parseInt(window.getComputedStyle(div).width);
        //console.log("wxWindowWASM::DoGetSize Width Div: (" + div.offsetWidth + ")");
        if ( div.offsetWidth ) {
            return div.offsetWidth;
        }
        else {
            return -1;
        }
    }, m_jid);
    *y = MAIN_THREAD_EM_ASM_INT({
        var div = document.getElementById(UTF8ToString($0));
        //console.log("wxWindowWASM::DoGetSize Height Div: " + UTF8ToString($0), div);
        if ( div.offsetHeight ) {
            return div.offsetHeight;
        }
        else {
            return -1;
        }
    }, m_jid);

    TRACE("wxWindowWASM::DoGetSize: %s %s %dx%d\n", m_jid, m_cid, *x, *y);
}

void wxWindowWASM::DoGetPosition(int *x, int *y) const
{
    TRACE("wxWindowWASM::DoGetPosition: %s %s\n", m_jid, m_cid);

    *x = MAIN_THREAD_EM_ASM_INT({
        var div = document.getElementById(UTF8ToString($0));
        //console.log("Get Width Div: " + UTF8ToString($0), div);
	return div.offsetLeft;
    }, m_jid);
    *y = MAIN_THREAD_EM_ASM_INT({
        var div = document.getElementById(UTF8ToString($0));
        //console.log("Get Height Div: " + UTF8ToString($0), div);
	return div.offsetTop;
    }, m_jid);

    TRACE("wxWindowWASM::DoGetPosition: %dx%d\n", *x, *y);
}

void wxWindowWASM::DoScreenToClient(int *x, int *y) const
{
    TRACE("wxWindowWASM::DoScreenToClient:\n");
}

void wxWindowWASM::DoClientToScreen(int *x, int *y) const
{
    TRACE("wxWindowWASM::DoClientToScreen:\n");
}

// Get size *available for subwindows* i.e. excluding menu bar etc.
void wxWindowWASM::DoGetClientSize(int *x, int *y) const
{
    *x = MAIN_THREAD_EM_ASM_INT({
        var div = document.getElementById(UTF8ToString($0));
        if ( div.clientWidth) {
            return div.clientWidth;
        }
        else {
            return -1;
        }
    }, m_jid);
    *y = MAIN_THREAD_EM_ASM_INT({
        var div = document.getElementById(UTF8ToString($0));
        if ( div.clientHeight) {
            return div.clientHeight;
        }
        else {
            return -1;
        }
    }, m_jid);

    TRACE("wxWindowWASM::DoGetClientSize: %s %s %dx%d\n", m_jid, m_cid, *x, *y);
}

void wxWindowWASM::DoSetSize(int x, int y, int width, int height, int sizeFlags)
{
    TRACE("wxWindowWASM::DoSetSize: %s %s %dx%d %dx%d\n", m_jid, m_cid, x, y, width, height);



    // get the current size and position...
    int currentX, currentY;
    int currentW, currentH;

    GetPosition(&currentX, &currentY);
    GetSize(&currentW, &currentH);

    if ( x == wxDefaultCoord && !(sizeFlags & wxSIZE_ALLOW_MINUS_ONE) )
        x = currentX;
    if ( y == wxDefaultCoord && !(sizeFlags & wxSIZE_ALLOW_MINUS_ONE) )
        y = currentY;

    wxSize size = wxDefaultSize;
    if ( width == wxDefaultCoord )
    {
        if ( sizeFlags & wxSIZE_AUTO_WIDTH )
        {
            size = GetBestSize();
            width = size.x;
        }
        else
        {
            // just take the current one
            width = currentW;
        }
    }

    if ( height == wxDefaultCoord )
    {
        if ( sizeFlags & wxSIZE_AUTO_HEIGHT )
        {
            if ( size.x == wxDefaultCoord )
            {
                size = GetBestSize();
            }
            //else: already called GetBestSize() above

            height = size.y;
        }
        else
        {
            // just take the current one
            height = currentH;
        }
    }

    // ... and don't do anything (avoiding flicker) if it's already ok unless
    // we're forced to resize the window
    if ( !(sizeFlags & wxSIZE_FORCE) )
    {
        if ( width == currentW && height == currentH )
        {
            // We need to send wxSizeEvent ourselves because Windows won't do
            // it if the size doesn't change.
            if ( sizeFlags & wxSIZE_FORCE_EVENT )
            {
                wxSizeEvent event( wxSize(width,height), GetId() );
                event.SetEventObject( this );
                HandleWindowEvent( event );
            }

            // Still call DoMoveWindow() below if we need to change the
            // position, otherwise we're done.
            if ( x == currentX && y == currentY )
                return;
        }
    }

    AdjustForParentClientOrigin(x, y, sizeFlags);

    DoMoveWindow(x, y, width, height);


/*
    wxSize newSize(width, height);
    wxSizeEvent event(newSize, GetId());
    event.SetEventObject(this);
    HandleWindowEvent(event);
*/
}

void wxWindowWASM::DoSetClientSize(int width, int height)
{
    TRACE("wxWindowWASM::DoSetClientSize: %s %s %dx%d\n", m_jid, m_cid, width, height);
//    SetSize(width, height);
    MAIN_THREAD_EM_ASM_INT({
        var id = UTF8ToString($0);
        var cid = UTF8ToString($1);
        var width = $2;
        var height = $3;
        var div = document.getElementById(id);
	div.style.width = width + "px";
	div.style.height = height + "px";
        console.log("Update Div: " + UTF8ToString($0), div);
        var canvas = window.wx.bitmaps.get(cid);
        if ( canvas ) {
            canvas.width = width;
            canvas.height = height;
            console.log("Update Canvas: " + UTF8ToString($0), div);
        }
    }, m_jid, m_cid, width, height);
}

void wxWindowWASM::DoMoveWindow(int x, int y, int width, int height)
{
    TRACE("wxWindowWASM::DoMoveWindow: %s %s %dx%d %dx%d\n", m_jid, m_cid, x, y, width, height);

/*
    if (width < 0)
        width = 0;
    if (height < 0)
        height = 0;
*/
    wxRect oldpos = GetRect();
    wxRect newpos(x, y, width, height);
    MAIN_THREAD_EM_ASM_INT({
        console.log("wxWindowWASM::DoMoveWindow");
        var id = UTF8ToString($0);
        var cid = UTF8ToString($1);
        var div = document.getElementById(id);
        var x = $2;
        var y = $3;
        var width  = $4;
        var height = $5;

        div.style.left = x + "px";
	div.style.top = y + "px";
	div.style.width = width + "px";
	div.style.height = height + "px";
        var canvas = window.wx.bitmaps.get(cid);
        if ( canvas ) {
            canvas.width = width;
            canvas.height = height;
        }
        //console.log("wxWindowWASM::DoMoveWindow: ", id, cid, x, y, width, height, div);
        console.log("wxWindowWASM::DoMoveWindow: ", id, cid, x, y, width, height, div, canvas);
    }, m_jid, m_cid, x, y, width, height);

    if ( !IsShown() )
    {
        printf("wxWindowWASM::DoMoveWindow SKipping repaint\n");
        return;
    }
    wxNcPaintEvent eventNc(GetId());
    eventNc.SetEventObject(this);
    HandleWindowEvent(eventNc);

    wxPoint point(x,y);
    wxMoveEvent eventMove(point, m_windowId);
    eventMove.SetEventObject(this);
    HandleWindowEvent(eventMove);

    wxSizeEvent eventSize(GetSize(), GetId());
    eventSize.SetEventObject(this);
    HandleWindowEvent(eventSize);

    wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
    while ( node )
    {
	    wxWindow *win = node->GetData();
	    win->HandleWindowEvent( eventSize );

	    node = node->GetNext();
    }

    Refresh();

/*
*/
    // Kind Of Works Refresh(true, NULL);
    //Refresh();//true, NULL);
    //GetParent()->Refresh(true, NULL);
    //GetParent()->RefreshRect(oldpos);
    //GetParent()->RefreshRect(newpos);
}

// ---------------------------------------------------------------------------
// text metrics
// ---------------------------------------------------------------------------

int wxWindowWASM::GetCharHeight() const
{
    TRACE("wxWindowWASM::GetCharHeight\n");
    return 10;
}

int wxWindowWASM::GetCharWidth() const
{
    TRACE("wxWindowWASM::GetCharWidth\n");
    return 10;
}

void wxWindowWASM::DoGetTextExtent(const wxString& string,
                                  int *x, int *y,
                                  int *descent,
                                  int *externalLeading,
                                  const wxFont *theFont) const
{
    TRACE("wxWindowWASM::DoGetTextExtent DO IT FOR REAL\n");
    const char * text = string.mb_str();
    *x = string.Length() * 10;
    if ( *x == 0 )
    {
        *x = 10;
    }
    *y = 10;
    if ( descent )
    {
        *descent = 0;
    }
    if ( externalLeading )
    {
        *externalLeading = 0;
    }
}

// ----------------------------------------------------------------------------
// painting
// ----------------------------------------------------------------------------

void wxWindowWASM::Refresh(bool eraseBack, const wxRect *rect)
{
    TRACE("wxWindowWASM::Refresh: %s\n", m_cid);
/*
    int height,width;
    GetSize( &width, &height );
    m_updateRegion.Clear();
    m_updateRegion.Union( 0, 0, width, height );


    SendNcPaintEvents();
*/

    if (eraseBack)
    {
        TRACE("wxWindowWASM::Refresh Back: %s\n", m_cid);
        if (rect)
        {
            TRACE("wxWindowWASM::Refresh Back Rect: %s\n", m_cid);
            // Schedule for later Updating in ::Update() or ::OnInternalIdle().
            m_clearRegion.Union( rect->x, rect->y, rect->width, rect->height );
        }
        else
        {
            TRACE("wxWindowWASM::Refresh Back no Rect: %s\n", m_cid);
            int height,width;
            GetSize( &width, &height );

            // Schedule for later Updating in ::Update() or ::OnInternalIdle().
            m_clearRegion.Clear();
            m_clearRegion.Union( 0, 0, width, height );
        }
    }

    if (rect)
    {
         TRACE("wxWindowWASM::Refresh 2 Rect: %s\n", m_cid);
         // Schedule for later Updating in ::Update() or ::OnInternalIdle().
         m_updateRegion.Union( rect->x, rect->y, rect->width, rect->height );
    }
    else
    {
         TRACE("wxWindowWASM::Refresh 2 No Rect: %s\n", m_cid);
        int height,width;
        GetSize( &width, &height );

        // Schedule for later Updating in ::Update() or ::OnInternalIdle().
        m_updateRegion.Clear();
        m_updateRegion.Union( 0, 0, width, height );
    }

    wxPaintEvent paint_event( GetId() );
    paint_event.SetEventObject( this );
    HandleWindowEvent( paint_event );

    wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
    while ( node )
    {
        wxWindow *win = node->GetData();
        win->HandleWindowEvent( paint_event );

        node = node->GetNext();
    }


}

void wxWindowWASM::Update()
{
    TRACE("wxWindowWASM::Update: %s\n", m_cid);

    if (m_updateNcArea)
    {
        SendNcPaintEvents();
    }

    if (!m_updateRegion.IsEmpty())
    {
        SendEraseEvents();
        SendPaintEvents();
    }

}


void wxWindowWASM::SendPaintEvents()
{
    printf("wxWindowWASM::SendPaintEvents: %s\n", m_cid);
    m_clipPaintRegion = true;

    wxPaintEvent paint_event( GetId() );
    paint_event.SetEventObject( this );
    HandleWindowEvent( paint_event );

/// Draw all subwindows ?
    wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
    while ( node )
    {
        wxWindow *win = node->GetData();
        win->HandleWindowEvent( paint_event );

        node = node->GetNext();
    }


    m_updateRegion.Clear();

    m_clipPaintRegion = false;
}

void wxWindowWASM::SendEraseEvents()
{
    printf("wxWindowWASM::SendEraseEvents: %s\n", m_cid);
    if (m_clearRegion.IsEmpty()) return;

    wxClientDC dc( (wxWindow*)this );
    dc.SetDeviceClippingRegion( m_clearRegion );

    wxEraseEvent erase_event( GetId(), &dc );
    erase_event.SetEventObject( this );

    HandleWindowEvent(erase_event);

    m_clearRegion.Clear();
}

void wxWindowWASM::SendNcPaintEvents()
{
    wxNcPaintEvent nc_paint_event( GetId() );
    nc_paint_event.SetEventObject( this );
    HandleWindowEvent( nc_paint_event );

    m_updateNcArea = false;
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

// ----------------------------------------------------------------------------
// Colour stuff
// ----------------------------------------------------------------------------

bool wxWindowWASM::SetBackgroundColour(const wxColour& col)
{
    wxWindowBase::SetBackgroundColour(col);

    wxString css = col.GetAsString(wxC2S_CSS_SYNTAX);
    const char * css_val = css.mb_str();
    TRACE("!!!!!!!!!!!!!wxWindowWASM::SetBackgroundColour: %s\n", m_jid);
    MAIN_THREAD_EM_ASM_INT({
        var div = document.getElementById(UTF8ToString($0));
        div.style.backgroundColor = UTF8ToString($1);
        console.log("Update Div: " + UTF8ToString($0), div);
    }, m_jid, css_val);
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

