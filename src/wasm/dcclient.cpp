/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/dcclient.cpp
// Purpose:     wxWindowDCImpl, wxClientDCImpl and wxPaintDC
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/window.h"
    #include "wx/nonownedwnd.h"
#endif

#include "wx/window.h"
#include "wx/wasm/window.h"
#include "wx/wasm/dcclient.h"

#include <emscripten.h>

#define TRACE(msg...)
//#define TRACE(msg...) printf(msg)

// ===========================================================================
// implementation
// ===========================================================================

//-----------------------------------------------------------------------------
// wxWindowDCImpl
//-----------------------------------------------------------------------------

wxIMPLEMENT_ABSTRACT_CLASS(wxWindowDCImpl, wxWASMDCImpl);

wxWindowDCImpl::wxWindowDCImpl(wxDC *owner, wxWindow *win)
              : wxWASMDCImpl(owner)
{
    wxWindowWASM * wwindow = (wxWindowWASM*)win;
    TRACE("wxWindowDCImpl::wxWindowDCImpl win=%p %s %s\n", win, wwindow->m_cid, wwindow->m_jid);

    // draw into the window canvas
    m_js_cid = win->m_js_cid;
    m_cid = m_js_cid.mb_str();

    m_textForegroundColour = win->GetForegroundColour();
    m_textBackgroundColour = win->GetBackgroundColour();

    TRACE("wxWindowDCImpl::wxWindowDCImpl win=%p %s\n", win, m_cid);

    SetFont(win->GetFont());
}

wxWindowDCImpl::~wxWindowDCImpl()
{
}

//-----------------------------------------------------------------------------
// wxClientDCImpl
//-----------------------------------------------------------------------------

wxIMPLEMENT_ABSTRACT_CLASS(wxClientDCImpl, wxWindowDCImpl);

wxClientDCImpl::wxClientDCImpl(wxDC *owner, wxWindow *win)
              : wxWindowDCImpl(owner, win)
{
    wxCHECK_RET( win, "invalid window" );
    TRACE("wxClientDCImpl::wxClientDCImpl win=%p %s\n", win, m_cid);

/*
    int x_t;
    int y_t;
    win->GetPosition(&x_t, &y_t);

    MAIN_THREAD_EM_ASM_INT({
        var id = UTF8ToString($0);
        var x = $1;
        var y = $2;
        var canvas = window.wx.bitmaps.get(id);
        if ( canvas ) {
            const ctx = canvas.getContext('2d');
            ctx.save();
            ctx.translate(x, y);
            console.log("wxWASMDCImpl JS Translate", x, y);
        }
        else {
            console.log("wxWASMDCImpl JS Translate No canvas found");
        }
    }, m_cid, x_t, y_t);
*/
}
wxClientDCImpl::~wxClientDCImpl()
{
/*
    MAIN_THREAD_EM_ASM_INT({
        var id = UTF8ToString($0);
        var canvas = window.wx.bitmaps.get(id);
        if ( canvas ) {
            const ctx = canvas.getContext('2d');
            ctx.restore();
            console.log("wxWASMDCImpl JS Restore");
        }
        else {
            console.log("wxWASMDCImpl JS Restore No canvas found");
        }
    }, m_cid);
*/
}

//-----------------------------------------------------------------------------
// wxPaintDC
//-----------------------------------------------------------------------------

wxIMPLEMENT_ABSTRACT_CLASS(wxPaintDCImpl, wxWindowDCImpl);

wxPaintDCImpl::wxPaintDCImpl(wxDC *owner) : wxClientDCImpl(owner)
{
    TRACE("wxPaintDCImpl::wxPaintDCImpl owner\n");
}
wxPaintDCImpl::wxPaintDCImpl(wxDC *owner, wxWindow *win) : wxClientDCImpl(owner, win)
{
    TRACE("wxPaintDCImpl::wxPaintDCImpl win=%p cid=%s\n", win, m_cid);

}

