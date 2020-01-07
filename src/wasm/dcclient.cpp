/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/dcclient.cpp
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/dcclient.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/window.h"
    #include "wx/math.h"
    #include "wx/image.h"
    #include "wx/module.h"
#endif

#include "wx/fontutil.h"
#include "wx/vector.h"


#include "wx/wasm/dcclient.h"
#include "wx/wasm/dcmemory.h"
// ----------------------------------------------------------------------------
// wxWindowDC
// ----------------------------------------------------------------------------

wxIMPLEMENT_ABSTRACT_CLASS(wxWindowDCImpl, wxWASMDCImpl);

wxWindowDCImpl::wxWindowDCImpl( wxDC *owner )
  : wxWASMDCImpl( owner )
{
    Init();
}

wxWindowDCImpl::wxWindowDCImpl( wxDC* owner, wxWindow *window )
  : wxWASMDCImpl( owner )
{
    wxASSERT_MSG( window, wxT("DC needs a window") );

    Init();
}

wxWindowDCImpl::~wxWindowDCImpl()
{
    Destroy();
}

void wxWindowDCImpl::Init()
{
    m_ok = true;
}

void wxWindowDCImpl::DoGetSize( int* width, int* height ) const
{
    wxCHECK_RET( m_window, wxT("GetSize() doesn't work without window") );

    m_window->GetSize(width, height);
}

extern bool wxDoFloodFill(wxDC *dc, wxCoord x, wxCoord y,
                          const wxColour & col, wxFloodFillStyle style);

bool wxWindowDCImpl::DoFloodFill(wxCoord x, wxCoord y,
                                 const wxColour& col, wxFloodFillStyle style)
{
    return true;
}

bool wxWindowDCImpl::DoGetPixel( wxCoord x1, wxCoord y1, wxColour *col ) const
{
    return true;
}

void wxWindowDCImpl::DoDrawLine( wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2 )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::DoCrossHair( wxCoord x, wxCoord y )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::DoDrawArc( wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2, wxCoord xc, wxCoord yc )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::DoDrawEllipticArc( wxCoord x, wxCoord y, wxCoord width, wxCoord height, double sa, double ea )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::DoDrawPoint( wxCoord x, wxCoord y )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::DoDrawLines( int n, const wxPoint points[], wxCoord xoffset, wxCoord yoffset )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::DoDrawPolygon( int n, const wxPoint points[],
                                wxCoord xoffset, wxCoord yoffset,
                                wxPolygonFillMode WXUNUSED(fillStyle) )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::DoDrawRectangle( wxCoord x, wxCoord y, wxCoord width, wxCoord height )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::DoDrawRoundedRectangle( wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::DoDrawEllipse( wxCoord x, wxCoord y, wxCoord width, wxCoord height )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::DoDrawIcon( const wxIcon &icon, wxCoord x, wxCoord y)
{
    DoDrawBitmap(icon, x, y, true);
}

void wxWindowDCImpl::DoDrawBitmap( const wxBitmap &bitmap,
                               wxCoord x, wxCoord y,
                               bool useMask )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );

    wxCHECK_RET( bitmap.IsOk(), wxT("invalid bitmap") );
}


bool wxWindowDCImpl::DoBlit( wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
                         wxDC *source, wxCoord xsrc, wxCoord ysrc,
                         wxRasterOperationMode logical_func, bool useMask,
                         wxCoord xsrcMask, wxCoord ysrcMask )
{
    wxCHECK_MSG( IsOk(), false, wxT("invalid window dc") );

    wxCHECK_MSG( source, false, wxT("invalid source dc") );

    return true;
}

void wxWindowDCImpl::DoDrawText( const wxString &text, wxCoord x, wxCoord y )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::DoDrawRotatedText(const wxString& text,
                                   wxCoord x, wxCoord y,
                                   double angle)
{
}

void wxWindowDCImpl::DoGetTextExtent( const wxString &string, wxCoord *width, wxCoord *height,
                                wxCoord *descent, wxCoord *externalLeading,
                                const wxFont *font ) const
{
}

wxCoord wxWindowDCImpl::GetCharWidth() const
{
	return wxCoord(10);
}

wxCoord wxWindowDCImpl::GetCharHeight() const
{
	return wxCoord(10);
}

void wxWindowDCImpl::Clear()
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::SetFont( const wxFont &font )
{
    wxCHECK_RET( IsOk(), wxT("invalid dc") );
}

void wxWindowDCImpl::SetPen( const wxPen &pen )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::SetBrush( const wxBrush &brush )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::SetBackground( const wxBrush &brush )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::SetLogicalFunction( wxRasterOperationMode function )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::SetTextForeground( const wxColour &col )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::SetTextBackground( const wxColour &col )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::SetBackgroundMode( int mode )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::SetPalette( const wxPalette& WXUNUSED(palette) )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::DoSetClippingRegion( wxCoord x, wxCoord y, wxCoord width, wxCoord height )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::DoSetDeviceClippingRegion( const wxRegion& region )
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::DestroyClippingRegion()
{
    wxCHECK_RET( IsOk(), wxT("invalid window dc") );
}

void wxWindowDCImpl::Destroy()
{
}

void wxWindowDCImpl::ComputeScaleAndOrigin()
{
}

wxSize wxWindowDCImpl::GetPPI() const
{
    return wxSize(100, 100);
}

int wxWindowDCImpl::GetDepth() const
{
    wxFAIL_MSG(wxT("not implemented"));

    return -1;
}

//-----------------------------------------------------------------------------
// wxClientDC
//-----------------------------------------------------------------------------

wxIMPLEMENT_ABSTRACT_CLASS(wxClientDCImpl, wxWindowDCImpl);

wxClientDCImpl::wxClientDCImpl( wxDC *owner, wxWindow *window )
          : wxWindowDCImpl( owner, window )
{
    wxCHECK_RET( window, wxT("NULL window in wxClientDC::wxClientDC") );
}

void wxClientDCImpl::DoGetSize(int *width, int *height) const
{
    wxCHECK_RET( m_window, wxT("GetSize() doesn't work without window") );

    m_window->GetClientSize( width, height );
}

// ----------------------------------------------------------------------------
// wxPaintDC
// ----------------------------------------------------------------------------

wxIMPLEMENT_ABSTRACT_CLASS(wxPaintDCImpl, wxClientDCImpl);

wxPaintDCImpl::wxPaintDCImpl(wxDC *owner, wxWindow* window)
  : wxClientDCImpl(owner, window)
{
}

// ----------------------------------------------------------------------------
// wxDCModule
// ----------------------------------------------------------------------------

class wxDCModule : public wxModule
{
public:
    // we must be cleaned up before wxDisplayModule which closes the global
    // display
    wxDCModule()
    {
    }

    bool OnInit() { return true; }
    void OnExit() { }

private:
    wxDECLARE_DYNAMIC_CLASS(wxDCModule);
};

wxIMPLEMENT_DYNAMIC_CLASS(wxDCModule, wxModule);

