/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/pen.cpp
// Purpose:     wxPen
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/pen.h"

#ifndef WX_PRECOMP
    #include "wx/utils.h"
    #include "wx/bitmap.h"
    #include "wx/colour.h"
#endif

//-----------------------------------------------------------------------------
// wxPen
//-----------------------------------------------------------------------------

class wxPenRefData : public wxGDIRefData
{
public:
    wxPenRefData()
    {
    }

    wxPenRefData( const wxPenRefData& data )
    {
    }

    wxPenRefData( const wxPenInfo& info )
    {
    }

    bool operator == (const wxPenRefData& data) const
    {
	    return false;
    }

};

//-----------------------------------------------------------------------------

#define M_PENDATA ((wxPenRefData *)m_refData)

wxIMPLEMENT_DYNAMIC_CLASS(wxPen, wxGDIObject);

wxPen::wxPen( const wxColour &colour, int width, wxPenStyle style )
{
    m_refData = new wxPenRefData(wxPenInfo(colour, width).Style(style));
}

wxPen::wxPen(const wxColour& colour, int width, int style)
{
    m_refData = new wxPenRefData
                    (
                        wxPenInfo(colour, width).Style((wxPenStyle)style)
                    );
}

wxPen::wxPen(const wxPenInfo& info)
{
    m_refData = new wxPenRefData(info);
}

wxPen::~wxPen()
{
}

wxGDIRefData *wxPen::CreateGDIRefData() const
{
    return new wxPenRefData;
}

wxGDIRefData *wxPen::CloneGDIRefData(const wxGDIRefData *data) const
{
    return new wxPenRefData(*(wxPenRefData *)data);
}

bool wxPen::operator == ( const wxPen& pen ) const
{
    if (m_refData == pen.m_refData) return true;

    if (!m_refData || !pen.m_refData) return false;

    return ( *(wxPenRefData*)m_refData == *(wxPenRefData*)pen.m_refData );
}

void wxPen::SetColour( const wxColour &colour )
{
    AllocExclusive();
}

void wxPen::SetDashes( int number_of_dashes, const wxDash *dash )
{
    AllocExclusive();
}

void wxPen::SetColour( unsigned char red, unsigned char green, unsigned char blue )
{
    AllocExclusive();
}

void wxPen::SetCap( wxPenCap capStyle )
{
    AllocExclusive();
}

void wxPen::SetJoin( wxPenJoin joinStyle )
{
    AllocExclusive();
}

void wxPen::SetStipple( const wxBitmap& stipple )
{
    AllocExclusive();
}

void wxPen::SetStyle( wxPenStyle style )
{
    AllocExclusive();
}

void wxPen::SetWidth( int width )
{
    AllocExclusive();
}

int wxPen::GetDashes( wxDash **ptr ) const
{
    wxCHECK_MSG( IsOk(), -1, wxT("invalid pen") );
    return 0;
}

int wxPen::GetDashCount() const
{
}

wxDash* wxPen::GetDash() const
{
}

wxPenCap wxPen::GetCap() const
{
}

wxPenJoin wxPen::GetJoin() const
{
    wxCHECK_MSG( IsOk(), wxJOIN_INVALID, wxT("invalid pen") );
}

wxPenStyle wxPen::GetStyle() const
{
    wxCHECK_MSG( IsOk(), wxPENSTYLE_INVALID, wxT("invalid pen") );
}

int wxPen::GetWidth() const
{
    wxCHECK_MSG( IsOk(), -1, wxT("invalid pen") );
}

wxColour wxPen::GetColour() const
{
    wxCHECK_MSG( IsOk(), wxNullColour, wxT("invalid pen") );
}

wxBitmap *wxPen::GetStipple() const
{
    wxCHECK_MSG( IsOk(), &wxNullBitmap, wxT("invalid pen") );
}
