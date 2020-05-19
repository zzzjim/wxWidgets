/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/dcmemory.cpp
// Purpose:     wxMemoryDC class
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/dcmemory.h"

#ifndef WX_PRECOMP
    #include "wx/utils.h"
    #include "wx/settings.h"
#endif

#include "wx/wasm/dcmemory.h"
#include "wx/window.h"
#include "wx/wasm/window.h"

wxIMPLEMENT_ABSTRACT_CLASS(wxMemoryDCImpl, wxWindowDCImpl);

wxMemoryDCImpl::wxMemoryDCImpl( wxMemoryDC *owner )
  : wxWindowDCImpl( owner )
{
    m_js_cid = wxString::Format("wxDCMemory-%d", (int)this);
    m_cid = m_js_cid.mb_str();
    printf("wxMemoryDCImpl::wxMemoryDCImpl %s\n", m_cid);
    //printf("wxMemoryDCImpl::wxMemoryDCImpl %s owner=%d ownerwin=%d\n", m_cid, (int)owner, (int)owner->GetWindow());

    m_ok = true;
    Init();
}

wxMemoryDCImpl::wxMemoryDCImpl( wxMemoryDC *owner, wxBitmap& bitmap )
  : wxWindowDCImpl( owner )
{
    printf("wxMemoryDCImpl::wxMemoryDCImpl bitmap %s\n", m_cid);

    /*
    if ( owner )
    {
        wxWindowWASM * win = (wxWindowWASM*)owner->GetWindow();
        if ( win )
        {
            printf("wxMemoryDCImpl::wxMemoryDCImpl use windDC %s %s\n", win->m_cid, (const char*)win->GetName().mb_str());
        }
    }
    */
    Init();
    DoSelect(bitmap);
}

wxMemoryDCImpl::wxMemoryDCImpl( wxMemoryDC *owner, wxDC *WXUNUSED(dc) )
  : wxWindowDCImpl( owner )
{
    printf("wxMemoryDCImpl::wxMemoryDCImpl DC %s\n", m_cid);
    Init();
}

void wxMemoryDCImpl::Init()
{
    printf("wxMemoryDCImpl::Init %s\n", m_cid);
}

wxMemoryDCImpl::~wxMemoryDCImpl()
{
    printf("~wxMemoryDCImpl::wxMemoryDCImpl DC\n");
}

void wxMemoryDCImpl::DoSelect( const wxBitmap& bitmap )
{
    printf("wxMemoryDCImpl::DoSelectDC %s bitmap\n", m_cid);
    m_bmp = bitmap;

    if ( !bitmap.IsOk() )
    {
        //m_bmp = NULL; // why this trigger the xpm constructor
    }
    m_selected = bitmap;

    m_js_cid = bitmap.GetCID();
    m_cid = m_js_cid.mb_str();
    printf("wxMemoryDCImpl::DoSelectDC %s bitmap\n", m_cid);
}

void wxMemoryDCImpl::DoGetSize( int *width, int *height ) const
{
    printf("wxMemoryDCImpl::DoGetSize\n");
    wxWASMDCImpl::DoGetSize(width, height);
}

const wxBitmap& wxMemoryDCImpl::GetSelectedBitmap() const
{
    printf("wxMemoryDCImpl::GetSelectedBitmap const %s", m_cid);
    return m_selected;
}

wxBitmap& wxMemoryDCImpl::GetSelectedBitmap()
{
    printf("wxMemoryDCImpl::GetSelectedBitmap non const %s", m_cid);
    return m_selected;
}
