/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/brush.cpp
// Purpose:     wxBrush
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/brush.h"

#ifndef WX_PRECOMP
    #include "wx/utils.h"
    #include "wx/bitmap.h"
    #include "wx/colour.h"
#endif

//-----------------------------------------------------------------------------
// wxBrush
//-----------------------------------------------------------------------------

class wxBrushRefData : public wxGDIRefData
{
public:
    wxBrushRefData()
    {
    }

    wxBrushRefData( const wxBrushRefData& data )
    {
    }

    bool operator == (const wxBrushRefData& data) const
    {
        return false;
    }

};

//-----------------------------------------------------------------------------

#define M_BRUSHDATA ((wxBrushRefData *)m_refData)

wxIMPLEMENT_DYNAMIC_CLASS(wxBrush,wxGDIObject);

wxBrush::wxBrush( const wxColour &colour, wxBrushStyle style )
{
    m_refData = new wxBrushRefData();
}

wxBrush::wxBrush(const wxColour& col, int style)
{
    m_refData = new wxBrushRefData;
}

wxBrush::wxBrush( const wxBitmap &stippleBitmap )
{
    m_refData = new wxBrushRefData();
}

wxBrush::~wxBrush()
{
    // m_refData unrefed in ~wxObject
}

wxGDIRefData *wxBrush::CreateGDIRefData() const
{
    return new wxBrushRefData;
}

wxGDIRefData *wxBrush::CloneGDIRefData(const wxGDIRefData *data) const
{
    return new wxBrushRefData(*(wxBrushRefData *)data);
}

bool wxBrush::operator == ( const wxBrush& brush ) const
{
    if (m_refData == brush.m_refData) return true;

    if (!m_refData || !brush.m_refData) return false;

    return ( *(wxBrushRefData*)m_refData == *(wxBrushRefData*)brush.m_refData );
}

wxBrushStyle wxBrush::GetStyle() const
{
    wxCHECK_MSG( IsOk(), wxBRUSHSTYLE_INVALID, wxT("invalid brush") );
}

wxColour wxBrush::GetColour() const
{
    wxCHECK_MSG( IsOk(), wxNullColour, wxT("invalid brush") );
}

wxBitmap *wxBrush::GetStipple() const
{
    wxCHECK_MSG( IsOk(), NULL, wxT("invalid brush") );
}

void wxBrush::SetColour( const wxColour& col )
{
    AllocExclusive();
}

void wxBrush::SetColour( unsigned char r, unsigned char g, unsigned char b )
{
    AllocExclusive();
}

void wxBrush::SetStyle( wxBrushStyle style )
{
    AllocExclusive();
}

void wxBrush::SetStipple( const wxBitmap& stipple )
{
    AllocExclusive();
}
