/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/dc.cpp
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/icon.h"
    #include "wx/log.h"
#endif // WX_PRECOMP

#include "wx/dc.h"
#include "wx/wasm/dc.h"

wxIMPLEMENT_CLASS(wxWASMDCImpl,wxDCImpl);

wxWASMDCImpl::wxWASMDCImpl( wxDC *owner )
    : wxDCImpl( owner )
{
    m_ok = true;
}

wxWASMDCImpl::~wxWASMDCImpl()
{
}


bool wxWASMDCImpl::CanDrawBitmap() const
{
    return true;
}

bool wxWASMDCImpl::CanGetTextExtent() const
{
    return true;
}

void wxWASMDCImpl::DoGetSize(int *width, int *height) const
{
}

void wxWASMDCImpl::DoGetSizeMM(int* width, int* height) const
{
}

int wxWASMDCImpl::GetDepth() const
{
    return 0;
}

wxSize wxWASMDCImpl::GetPPI() const
{
    return wxSize(300, 1);
}

void wxWASMDCImpl::SetFont(const wxFont& font)
{
}

void wxWASMDCImpl::SetPen(const wxPen& pen)
{
}

void wxWASMDCImpl::SetBrush(const wxBrush& brush)
{
}

void wxWASMDCImpl::SetBackground(const wxBrush& brush)
{
}

void wxWASMDCImpl::SetBackgroundMode(int mode)
{
}

#if wxUSE_PALETTE
void wxWASMDCImpl::SetPalette(const wxPalette& WXUNUSED(palette))
{
    wxMISSING_IMPLEMENTATION(__FUNCTION__);
}
#endif // wxUSE_PALETTE

void wxWASMDCImpl::SetLogicalFunction(wxRasterOperationMode function)
{
}

wxCoord wxWASMDCImpl::GetCharHeight() const
{
}

wxCoord wxWASMDCImpl::GetCharWidth() const
{
}

void wxWASMDCImpl::DoGetTextExtent(const wxString& string,
                             wxCoord *x, wxCoord *y,
                             wxCoord *descent,
                             wxCoord *externalLeading,
                             const wxFont *theFont ) const
{
}

void wxWASMDCImpl::Clear()
{
}

void wxWASMDCImpl::DoSetClippingRegion(wxCoord x, wxCoord y,
                                 wxCoord width, wxCoord height)
{
}

void wxWASMDCImpl::DoSetDeviceClippingRegion(const wxRegion& region)
{
}

void wxWASMDCImpl::DestroyClippingRegion()
{
}

bool wxWASMDCImpl::DoFloodFill(wxCoord x, wxCoord y, const wxColour& col,
                         wxFloodFillStyle style )
{
#if wxUSE_IMAGE
    extern bool wxDoFloodFill(wxDC *dc, wxCoord x, wxCoord y,
                              const wxColour & col, wxFloodFillStyle style);

    return wxDoFloodFill( GetOwner(), x, y, col, style);
#else
    wxUnusedVar(x);
    wxUnusedVar(y);
    wxUnusedVar(col);
    wxUnusedVar(style);

    return false;
#endif
}

bool wxWASMDCImpl::DoGetPixel(wxCoord x, wxCoord y, wxColour *col) const
{
    return false;
}

void wxWASMDCImpl::DoDrawPoint(wxCoord x, wxCoord y)
{
}

void wxWASMDCImpl::DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2)
{
}


void wxWASMDCImpl::DoDrawArc(wxCoord x1, wxCoord y1,
                       wxCoord x2, wxCoord y2,
                       wxCoord xc, wxCoord yc)
{
}

void wxWASMDCImpl::DoDrawEllipticArc(wxCoord x, wxCoord y, wxCoord w, wxCoord h,
                               double sa, double ea)
{
}

void wxWASMDCImpl::DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
}

void wxWASMDCImpl::DoDrawRoundedRectangle(wxCoord x, wxCoord y,
                                    wxCoord width, wxCoord height,
                                    double radius)
{
}

void wxWASMDCImpl::DoDrawEllipse(wxCoord x, wxCoord y,
                           wxCoord width, wxCoord height)
{
}

void wxWASMDCImpl::DoCrossHair(wxCoord x, wxCoord y)
{
}

void wxWASMDCImpl::DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y)
{
}

void wxWASMDCImpl::DoDrawBitmap(const wxBitmap &bmp, wxCoord x, wxCoord y,
                          bool useMask )
{
}

void wxWASMDCImpl::DoDrawText(const wxString& text, wxCoord x, wxCoord y)
{
}

void wxWASMDCImpl::DoDrawRotatedText(const wxString& text,
                               wxCoord x, wxCoord y, double angle)
{
}

bool wxWASMDCImpl::DoBlit(wxCoord xdest, wxCoord ydest,
                    wxCoord width, wxCoord height,
                    wxDC *source,
                    wxCoord xsrc, wxCoord ysrc,
                    wxRasterOperationMode rop,
                    bool useMask,
                    wxCoord WXUNUSED(xsrcMask),
                    wxCoord WXUNUSED(ysrcMask) )
{
    return false;
}

void wxWASMDCImpl::DoDrawLines(int n, const wxPoint points[],
                         wxCoord xoffset, wxCoord yoffset )
{
}

void wxWASMDCImpl::DoDrawPolygon(int n, const wxPoint points[],
                       wxCoord xoffset, wxCoord yoffset,
                       wxPolygonFillMode fillStyle )
{
}

