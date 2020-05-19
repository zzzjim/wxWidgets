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

#include <emscripten.h>
// ----------------------------------------------------------------------------
// wxTopLevelWindowWASM creation
// ----------------------------------------------------------------------------

#define TRACE(msg...)
//#define TRACE(msg...) printf(msg)

void wxTopLevelWindowWASM::Init()
{
    m_isShown = false;
}

wxTopLevelWindowWASM::wxTopLevelWindowWASM()
{
    Init();
}

wxTopLevelWindowWASM::wxTopLevelWindowWASM(wxWindow *parent,
               wxWindowID id,
               const wxString& title,
               const wxPoint& pos,
               const wxSize& size,
               long style,
               const wxString& name)
{
    Create(parent, id, title, pos, size, style, name);
}

bool wxTopLevelWindowWASM::Create(wxWindow *parent,
                wxWindowID id,
                const wxString& title,
                const wxPoint& posOrig,
                const wxSize& sizeOrig,
                long style,
                const wxString& name)
{
    TRACE("wxTopLevelWindowWASM::Create:(%s) Parent(%p) Title(%s)\n", (const char*)name.mb_str(), parent, (const char*)title.mb_str());
    Init();

    SetName(name);
    SetTitle(title);


    wxSize size(sizeOrig);
    if ( !size.IsFullySpecified() )
        size.SetDefaults( GetDefaultSize() );

    wxPoint pos(posOrig);
    if ( pos.x == wxDefaultCoord )
        pos.x = 20;
    if ( pos.y == wxDefaultCoord )
        pos.y = 20;


    m_js_id = wxString::Format("wx-%s-%d", name, (int)this);//m_windowId.GetValue());
    m_jid = m_js_id.mb_str();

    m_js_cid = wxString::Format("wxWindowDC-%s-%d", name, (int)this);//m_windowId.GetValue());
    m_cid = m_js_cid.mb_str();;

    TRACE("wxTopLevelWindowWASM::Create(%s) %d %d %d %d\n", m_jid, pos.x, pos.y, size.GetWidth(), size.GetHeight());

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
        console.log("wxTopLevelWindowWASM::Create:", id, pid, cid, x, y, width, height);

        var div = document.createElement('div');
        div.setAttribute('id', id);
	div.style.position = 'absolute';
	div.style.display = 'block';
	//div.style.display = 'none';
        div.style.visibility = "hidden";
        //div.style.visibility = "visible";
	//div.style.backgroundColor = bgcolor;
	div.style.left = x + "px";
	div.style.top = y + "px";
	div.style.width = width + "px";
	div.style.height = height + "px";

        parent.appendChild(div);
        console.log("wxTopLevelWindowWASM::Create Adding window div: ", div);

        var canvas = document.createElement('canvas');
        canvas.setAttribute('id', cid);
	canvas.width = width;
	canvas.height = height;
        console.log("wxTopLevelWindowWASM::Create Adding window DC canvas: ", canvas.width, canvas.height, canvas);
        div.appendChild(canvas);
        window.wx.bitmaps.set(cid, canvas);

    }, m_jid, m_cid, "",  pos.x, pos.y, size.GetWidth(), size.GetHeight());


    wxTopLevelWindows.Append( this );

    if (parent)
    {
        parent->AddChild( this );
    }

    if (!CreateBase( parent, id, pos, size, style, wxDefaultValidator, name ))
    {
        wxFAIL_MSG( wxT("wxTopLevelWindowNative creation failed") );
        return false;
    }

    DoMoveWindow(pos.x, pos.y, size.GetWidth(), size.GetHeight());
    return true;
}

void wxTopLevelWindowWASM::DoSetClientSize(int width, int height)
{
    TRACE("wxTopLevelWindowWASM::DoSetClientSize: %s %s %dx%d\n", m_jid, m_cid, width, height);
    wxASSERT_MSG( width != 0, "who did it! fuck");
 //   SetSize(width, height);
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

void wxTopLevelWindowWASM::DoMoveWindow(int x, int y, int width, int height)
{
    TRACE("wxTopLevelWindowWASM::DoMoveWindow: %s %s %dx%d %dx%d\n", m_jid, m_cid, x, y, width, height);
    wxWindowWASM::DoMoveWindow(x, y, width, height);
/*
    MAIN_THREAD_EM_ASM_INT({
        console.log("wxWindowWASM::DoMoveWindow");
        var id = UTF8ToString($0);
        var cid = UTF8ToString($1);
        var div = document.getElementById(id);
        var canvas = window.wx.bitmaps.get(cid);
        var x = $2;
        var y = $3;
        var width  = $4;
        var height = $5;

        div.style.left = x + "px";
	div.style.top = y + "px";
	div.style.width = width + "px";
	div.style.height = height + "px";
        if ( canvas ) {
            canvas.width = width;
            canvas.height = height;
        }
        console.log("wxWindowWASM::DoMoveWindow: ", id, cid, x, y, width, height, div, canvas);
    }, m_jid, m_cid, x, y, width, height);

    wxPoint point(x,y);
    wxMoveEvent event(point, m_windowId);
    event.SetEventObject(this);
    HandleWindowEvent(event);

    wxSize newSize(width, height);
    wxSizeEvent eventSize(newSize, GetId());
    eventSize.SetEventObject(this);
    HandleWindowEvent(eventSize);
   
    wxNcPaintEvent eventNc(GetId());
    eventNc.SetEventObject(this);
    HandleWindowEvent(eventNc);

    Refresh();
*/

}


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
    return false;
}

void wxTopLevelWindowWASM::Restore()
{
}

void wxTopLevelWindowWASM::SetTitle(const wxString& title)
{
    m_title = title;

}

wxString wxTopLevelWindowWASM::GetTitle() const
{
    return m_title;
}

bool wxTopLevelWindowWASM::IsFullScreen() const
{
    return false;
}

bool wxTopLevelWindowWASM::Show(bool show)
{
    TRACE("wxTopLevelWindowWASM::Show %s %s %d\n", m_jid, m_cid, show);
    if ( !wxWindowBase::Show(show) )
    {
        TRACE("wxTopLevelWindowWASM::Show Early Exit %s %s %d\n", m_jid, m_cid, show);
        return false;
    }

    const char * visibility;
    if ( show )
    {
        visibility = "visible";
        wxSizeEvent event(GetSize(), GetId());
        event.SetEventObject(this);
        HandleWindowEvent(event);

	wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
	while ( node )
	{
		wxWindow *win = node->GetData();
		win->HandleWindowEvent( event );

		node = node->GetNext();
	}

    }
    else
    {
        visibility = "hidden";
    }
    TRACE("wxTopLevelWindowWASM::Show %s %s %s\n", m_jid, m_cid, visibility);
    MAIN_THREAD_EM_ASM_INT({
        var div = document.getElementById(UTF8ToString($0));
        div.style.visibility = UTF8ToString($1);
	//div.style.display = 'block';
        console.log("Update Div: " + UTF8ToString($0), div);
    }, m_jid, visibility);

    if ( show )
    {
	/* This works paints the background */
        wxNcPaintEvent eventNc(GetId());
        eventNc.SetEventObject(this);
        HandleWindowEvent(eventNc);

        Refresh();
/*
        wxPaintEvent paint_event( GetId() );
        paint_event.SetEventObject( this );
        HandleWindowEvent( paint_event );

        wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
        while ( node )
        {
            wxWindow *win = node->GetData();

	    wxPaintEvent paint_event( win->GetId() );
	    paint_event.SetEventObject( win );

            win->HandleWindowEvent( paint_event );

            node = node->GetNext();
        }
*/
        //Refresh();
        //Update();
/*
        //Update();
        wxNcPaintEvent eventNc(GetId());
        eventNc.SetEventObject(this);
        HandleWindowEvent(eventNc);

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
*/

    }

/*
    wxSizeEvent event(GetSize(), GetId());
    event.SetEventObject(this);
    HandleWindowEvent(event);
    if ( show )
        Update();

*/
    return true;
}


// ----------------------------------------------------------------------------
// wxTopLevelWindowX11 fullscreen
// ----------------------------------------------------------------------------

bool wxTopLevelWindowWASM::ShowFullScreen(bool show, long style)
{
    TRACE("WARN: This may be possible with SDL\n");
    return false;
}

