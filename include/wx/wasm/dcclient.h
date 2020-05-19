/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dfb/dcclient.h
// Purpose:     wxWindowDCImpl, wxClientDCImpl and wxPaintDCImpl
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WASM_DCCLIENT_H_
#define _WX_WASM_DCCLIENT_H_

#include "wx/wasm/dc.h"

class WXDLLIMPEXP_FWD_CORE wxWindow;

//-----------------------------------------------------------------------------
// wxWindowDCImpl
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxWindowDCImpl : public wxWASMDCImpl
{
public:
    wxWindowDCImpl(wxDC *owner) : wxWASMDCImpl(owner), m_shouldFlip(false) { }
    wxWindowDCImpl(wxDC *owner, wxWindow *win);
    virtual ~wxWindowDCImpl();

protected:
    // initializes the DC for painting on given window; if rect!=NULL, then
    // for painting only on the given region of the window
    void InitForWin(wxWindow *win, const wxRect *rect);

private:
    wxRect    m_winRect; // rectangle of the window being painted

    bool m_shouldFlip; // flip the surface when done?

    friend class wxOverlayImpl; // for m_shouldFlip;

    wxDECLARE_DYNAMIC_CLASS(wxWindowDCImpl);
    wxDECLARE_NO_COPY_CLASS(wxWindowDCImpl);
};

//-----------------------------------------------------------------------------
// wxClientDCImpl
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxClientDCImpl : public wxWindowDCImpl
{
public:
    wxClientDCImpl(wxDC *owner) : wxWindowDCImpl(owner) { }
    wxClientDCImpl(wxDC *owner, wxWindow *win);
    ~wxClientDCImpl();

    wxDECLARE_DYNAMIC_CLASS(wxClientDCImpl);
    wxDECLARE_NO_COPY_CLASS(wxClientDCImpl);
};


//-----------------------------------------------------------------------------
// wxPaintDCImpl
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxPaintDCImpl : public wxClientDCImpl
{
public:
    wxPaintDCImpl(wxDC *owner);
    wxPaintDCImpl(wxDC *owner, wxWindow *win);

    wxDECLARE_DYNAMIC_CLASS(wxPaintDCImpl);
    wxDECLARE_NO_COPY_CLASS(wxPaintDCImpl);
};

#endif // _WX_WASM_DCCLIENT_H_
