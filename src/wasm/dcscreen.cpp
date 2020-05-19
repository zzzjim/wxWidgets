/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/dcscreen.cpp
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/dcscreen.h"
#include "wx/wasm/dcscreen.h"

wxIMPLEMENT_ABSTRACT_CLASS(wxScreenDCImpl, wxWASMDCImpl);

wxScreenDCImpl::wxScreenDCImpl( wxScreenDC *owner )
    : wxWindowDCImpl( owner )
{
    m_js_cid = wxString::Format("wxDCScreen-%d", (int)this);
    m_cid = m_js_cid.mb_str();
    printf("wxScreenDCImpl::wxScreenDCImpl %s\n", m_cid);
}

wxScreenDCImpl::~wxScreenDCImpl( )
{
    printf("wxScreenDCImpl::~wxScreenDCImpl: %s\n", m_cid);
}

void wxScreenDCImpl::DoGetSize(int *width, int *height) const
{
    printf("wxScreenDCImpl::DoGetSize %s\n", m_cid);
    wxDisplaySize(width, height);
}

