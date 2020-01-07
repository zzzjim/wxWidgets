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

wxIMPLEMENT_ABSTRACT_CLASS(wxMemoryDCImpl, wxWindowDCImpl);

wxMemoryDCImpl::wxMemoryDCImpl( wxDC *owner )
  : wxWindowDCImpl( owner )
{
    Init();
}

wxMemoryDCImpl::wxMemoryDCImpl(  wxDC *owner, wxBitmap& bitmap )
  : wxWindowDCImpl( owner )
{
    Init();
    DoSelect(bitmap);
}

wxMemoryDCImpl::wxMemoryDCImpl( wxDC* owner, wxDC *WXUNUSED(dc) )
  : wxWindowDCImpl( owner )
{
    Init();
}

void wxMemoryDCImpl::Init()
{
    m_ok = false;
}

wxMemoryDCImpl::~wxMemoryDCImpl()
{
}

void wxMemoryDCImpl::DoSelect( const wxBitmap& bitmap )
{
    Destroy();
}

void wxMemoryDCImpl::DoGetSize( int *width, int *height ) const
{
}

const wxBitmap& wxMemoryDCImpl::GetSelectedBitmap() const
{
    return m_selected;
}

wxBitmap& wxMemoryDCImpl::GetSelectedBitmap()
{
    return m_selected;
}
