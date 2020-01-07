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
}

wxScreenDCImpl::~wxScreenDCImpl( )
{
}

void wxScreenDCImpl::DoGetSize(int *width, int *height) const
{
    wxDisplaySize(width, height);
}

