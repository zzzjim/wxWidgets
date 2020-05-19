/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/dc.cpp
// Purpose:     wxFont class
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
    #include "wx/dcmemory.h"
    #include "wx/log.h"
#endif

#include "wx/wasm/dc.h"
#include "wx/window.h"

#include <emscripten.h>

// these values are used to initialize newly created DC
#define DEFAULT_FONT      (*wxNORMAL_FONT)
#define DEFAULT_PEN       (*wxBLACK_PEN)
#define DEFAULT_BRUSH     (*wxWHITE_BRUSH)

//#define DELAY() sleep(1)
#define DELAY() 

#define TRACE(msg...)
//#define TRACE(msg...) printf(msg)
// ===========================================================================
// implementation
// ===========================================================================

//-----------------------------------------------------------------------------
// wxWASMDCImpl
//-----------------------------------------------------------------------------

wxIMPLEMENT_ABSTRACT_CLASS(wxWASMDCImpl, wxDCImpl);

wxWASMDCImpl::wxWASMDCImpl(wxDC *owner) : wxDCImpl(owner)
{
    m_js_cid = wxString::Format("wxDC-%d", (int)this);
    m_cid = m_js_cid.mb_str();

    TRACE("wxWASMDCImpl::wxWASMDCImpl %s owner=%p\n", m_cid, owner);

    Init();
}
void wxWASMDCImpl::Init()
{
    m_pen = *wxBLACK_PEN;
    m_font = *wxNORMAL_FONT;
    m_brush = *wxWHITE_BRUSH;

    m_backgroundMode = wxBRUSHSTYLE_TRANSPARENT;

    m_textForegroundColour = *wxBLACK;
    m_textBackgroundColour = *wxWHITE;
}
// ---------------------------------------------------------------------------
// clipping
// ---------------------------------------------------------------------------

void wxWASMDCImpl::DoSetClippingRegion(wxCoord cx, wxCoord cy, wxCoord cw, wxCoord ch)
{
    TRACE("wxWASMDCImpl::DoSetClippingRegion: %s %d,%d %d,%d\n", m_cid, cx, cy, cw, ch);
    wxCHECK_RET( IsOk(), wxT("invalid dc") );
    m_clipping = true;
    m_clipX1 = cx;
    m_clipY1 = cy;
    m_clipX2 = cx + cw;
    m_clipY2 = cy + ch;
}

void wxWASMDCImpl::DoSetDeviceClippingRegion(const wxRegion& region)
{
    TRACE("wxWASMDCImpl::DoSetDeviceClippingRegion\n");
}

// ---------------------------------------------------------------------------
// query capabilities
// ---------------------------------------------------------------------------

int wxWASMDCImpl::GetDepth() const
{
    TRACE("wxWASMDCImpl::GetDepth\n");
    return 24;
}

// ---------------------------------------------------------------------------
// drawing
// ---------------------------------------------------------------------------

void wxWASMDCImpl::Clear()
{
    TRACE("wxWASMDCImpl::Clear: %s\n", m_cid);
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    wxColour col;
    wxString css;

    //wxCHECK_RET( m_backgroundBrush.IsOk(), wxT("invalid brush") );

    bool valid_brush = true;
    if ( !m_backgroundBrush.IsOk() )
    {
        TRACE("wxWASMDCImpl::Clear SKipping No Background Brush Set: %s\n", m_cid);
        valid_brush = false;
        return;
    }
    else if ( m_backgroundBrush.GetStyle() == wxBRUSHSTYLE_TRANSPARENT )
    {
        TRACE("wxWASMDCImpl::Clear SKipping: %s\n", m_cid);
        valid_brush = false;
        return;
    }

    const char * colour = "white";
    if ( valid_brush ) {
        TRACE("wxWASMDCImpl::Clear found brush: %s\n", m_cid);
        col = m_backgroundBrush.GetColour();
        css = col.GetAsString(wxC2S_CSS_SYNTAX);
	colour = css.mb_str();
    }

    MAIN_THREAD_EM_ASM_INT({
        var id = UTF8ToString($0);
        var colour = UTF8ToString($1);
        var canvas = window.wx.bitmaps.get(id);
        if ( canvas ) {
            const ctx = canvas.getContext('2d');
            var oldFill = ctx.fillStyle;
            ctx.fillStyle = colour;
            ctx.fillRect(0, 0, canvas.width, canvas.height);
            ctx.fillStyle = oldFill;
            //console.log("wxWASMDCImpl JS clear: " + colour, canvas);
        }
        else {
            //console.log("wxWASMDCImpl JS clear No canvas found");
        }
    }, m_cid, colour);
    DELAY();
}

extern bool wxDoFloodFill(wxDC *dc, wxCoord x, wxCoord y,
                          const wxColour & col, wxFloodFillStyle style);

bool wxWASMDCImpl::DoFloodFill(wxCoord x, wxCoord y,
                       const wxColour& col, wxFloodFillStyle style)
{
    TRACE("wxWASMDCImpl::DoFloodFill\n");
    return wxDoFloodFill(GetOwner(), x, y, col, style);
}

bool wxWASMDCImpl::DoGetPixel(wxCoord x, wxCoord y, wxColour *col) const
{
    TRACE("wxWASMDCImpl::DoGetPixel\n");
    wxCHECK_MSG( col, false, "NULL colour parameter in wxWASMDCImpl::GetPixel");

    wxFAIL_MSG( "GetPixel not implemented" );

    wxUnusedVar(x);
    wxUnusedVar(y);

    return false;
}

void wxWASMDCImpl::DoCrossHair(wxCoord x, wxCoord y)
{
    TRACE("wxWASMDCImpl::DoCrossHair\n");
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    wxFAIL_MSG( "CrossHair not implemented" );

    wxUnusedVar(x);
    wxUnusedVar(y);
}

void wxWASMDCImpl::DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2)
{
    wxCHECK_RET( IsOk(), wxT("invalid dc") );
    wxCHECK_RET( m_pen.IsOk(), wxT("invalid pen") );
    TRACE("wxWASMDCImpl::DoDrawLine %s %dx%d -> %dx%d\n", m_cid, x1, y1, x2, y2);

    wxString col = m_pen.GetColour().GetAsString(wxC2S_CSS_SYNTAX);
    const char * fgcolour = col.mb_str();
    int width = m_pen.GetWidth();

    MAIN_THREAD_EM_ASM_INT({
        var id = UTF8ToString($0);
        var x1 = $1;
        var y1 = $2;
        var x2 = $3;
        var y2 = $4;
        var width = $5;
        var fgcolour = UTF8ToString($6);
        var canvas = window.wx.bitmaps.get(id);
        if ( canvas ) {
            const ctx = canvas.getContext('2d');
            ctx.save();

            ctx.strokeStyle = fgcolour;
            ctx.lineWidth = width;

            ctx.beginPath();
            ctx.moveTo(x1, y1);
            ctx.lineTo(x2, y2);
            ctx.stroke();

            ctx.restore();
            //console.log("wxWASMDCImpl JS Line ", x1, y1, x2, y2, fgcolour, canvas)
        }
        else {
            //console.log("wxWASMDCImpl JS clear No canvas found");
        }
    }, m_cid, x1, y1, x2, y2, width, fgcolour);

    CalcBoundingBox(x1, y1);
    CalcBoundingBox(x2, y2);

    DELAY();
}

// Draws an arc of a circle, centred on (xc, yc), with starting point (x1, y1)
// and ending at (x2, y2)
void wxWASMDCImpl::DoDrawArc(wxCoord WXUNUSED(x1), wxCoord WXUNUSED(y1),
                            wxCoord WXUNUSED(x2), wxCoord WXUNUSED(y2),
                            wxCoord WXUNUSED(xc), wxCoord WXUNUSED(yc))
{
    TRACE("wxWASMDCImpl::DoDrawArc\n");
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    wxFAIL_MSG( "DrawArc not implemented" );
}

void wxWASMDCImpl::DoDrawPoint(wxCoord x, wxCoord y)
{
    TRACE("wxWASMDCImpl::DoDrawPoint\n");
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    DoDrawLine(x, y, x, y);
}

void wxWASMDCImpl::DoDrawPolygon(int WXUNUSED(n), const wxPoint WXUNUSED(points)[],
                                wxCoord WXUNUSED(xoffset), wxCoord WXUNUSED(yoffset),
                                wxPolygonFillMode WXUNUSED(fillStyle))
{
    TRACE("wxWASMDCImpl::DoDrawPolygon\n");
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    wxFAIL_MSG( "DrawPolygon not implemented" );
}

void wxWASMDCImpl::DoDrawLines(int WXUNUSED(n), const wxPoint WXUNUSED(points)[],
                              wxCoord WXUNUSED(xoffset), wxCoord WXUNUSED(yoffset))
{
    TRACE("wxWASMDCImpl::DoDrawLines\n");
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    wxFAIL_MSG( "DrawLines not implemented" );
}

void wxWASMDCImpl::DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
    TRACE("wxWASMDCImpl::DoDrawRectangle %s %dx%d %dx%d\n", m_cid, x, y, width, height);
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    wxCHECK_RET( m_brush.IsOk(), wxT("invalid brush") );
    wxCHECK_RET( m_pen.IsOk(), wxT("invalid pen") );

    wxString brushCol = m_brush.GetColour().GetAsString(wxC2S_CSS_SYNTAX);
    const char * bgcolour = brushCol.mb_str();

    wxString penCol = m_pen.GetColour().GetAsString(wxC2S_CSS_SYNTAX);
    const char * fgcolour = penCol.mb_str();

    if ( m_pen.IsTransparent() )
    {
        MAIN_THREAD_EM_ASM_INT({
            var id = UTF8ToString($0);
            var x = $1;
            var y = $2;
            var width = $3;
            var height = $4;
            var bgcolour = UTF8ToString($5);;
            var fgcolour = UTF8ToString($6);;
            var canvas = window.wx.bitmaps.get(id);
            if ( canvas ) {
                const ctx = canvas.getContext('2d');
                ctx.save();
                ctx.fillStyle = bgcolour;
                ctx.fillRect(x, y, width, height);
                ctx.restore();
                //console.log("wxWASMDCImpl JS Fill Rect", bgcolour, canvas)
            }
            else {
                //console.log("wxWASMDCImpl JS Rect No canvas found");
            }
        }, m_cid, x, y, width, height, bgcolour, fgcolour);

    }
    else
    {
        MAIN_THREAD_EM_ASM_INT({
            var id = UTF8ToString($0);
            var x = $1;
            var y = $2;
            var width = $3;
            var height = $4;
            var bgcolour = UTF8ToString($5);;
            var fgcolour = UTF8ToString($6);;
            var canvas = window.wx.bitmaps.get(id);
            if ( canvas ) {
                const ctx = canvas.getContext('2d');
                ctx.save();

                ctx.fillStyle = bgcolour;
                ctx.fillRect(x, y, width, height);
    
                ctx.strokeStyle = fgcolour;
                ctx.strokeRect(x, y, width, height);

                ctx.restore();
                //console.log("wxWASMDCImpl JS Rect With Border", fgcolour, bgcolour, canvas)
            }
            else {
                //console.log("wxWASMDCImpl JS Rect No canvas found");
            }
        }, m_cid, x, y, width, height, bgcolour, fgcolour);
    }

    CalcBoundingBox(x, y);
    CalcBoundingBox(x + width, y + height);
}

void wxWASMDCImpl::DoDrawRoundedRectangle(wxCoord WXUNUSED(x),
                                         wxCoord WXUNUSED(y),
                                         wxCoord WXUNUSED(width),
                                         wxCoord WXUNUSED(height),
                                         double WXUNUSED(radius))
{
    TRACE("wxWASMDCImpl::DoDrawRoundedRectangle\n");
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    wxFAIL_MSG( "DrawRoundedRectangle not implemented" );
}

void wxWASMDCImpl::DoDrawEllipse(wxCoord WXUNUSED(x),
                                wxCoord WXUNUSED(y),
                                wxCoord WXUNUSED(width),
                                wxCoord WXUNUSED(height))
{
    TRACE("wxWASMDCImpl::DoDrawEllipse\n");
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    wxFAIL_MSG( "DrawElipse not implemented" );
}

void wxWASMDCImpl::DoDrawEllipticArc(wxCoord WXUNUSED(x),
                                    wxCoord WXUNUSED(y),
                                    wxCoord WXUNUSED(w),
                                    wxCoord WXUNUSED(h),
                                    double WXUNUSED(sa),
                                    double WXUNUSED(ea))
{
    TRACE("wxWASMDCImpl::DoDrawEllipticArc\n");
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    wxFAIL_MSG( "DrawElipticArc not implemented" );
}

void wxWASMDCImpl::DoDrawText(const wxString& text, wxCoord x, wxCoord y)
{
    const char * tdraw = text.mb_str();
    TRACE("wxWASMDCImpl::DoDrawText: %s %s %d,%d\n", m_cid, tdraw, x, y);
    wxCHECK_RET( IsOk(), wxT("invalid dc") );
    wxCHECK_RET( m_font.IsOk(), wxT("invalid font") );
    wxCHECK_RET( m_brush.IsOk(), wxT("invalid brush") );
    //wxCHECK_RET( m_pen.IsOk(), wxT("invalid pen") );

    x = XLOG2DEV(x);
    y = YLOG2DEV(y);
    TRACE("wxWASMDCImpl::DoDrawText LOG2DEV: %s %s %d,%d\n", m_cid, tdraw, x, y);

    wxString fg = m_textForegroundColour.GetAsString(wxC2S_CSS_SYNTAX);
    wxString bg = m_textBackgroundColour.GetAsString(wxC2S_CSS_SYNTAX);

    const char * fgcolour = fg.mb_str();
    const char * bgcolour = bg.mb_str();

    // update the bounding box
    wxCoord w, h;
    CalcBoundingBox(x, y);
    DoGetTextExtent(text, &w, &h);
    CalcBoundingBox(x + w, y + h);

    // fill in background box
    if ( m_textBackgroundColour.IsOk () && (m_backgroundMode != wxBRUSHSTYLE_TRANSPARENT) )
    {
        MAIN_THREAD_EM_ASM_INT({
            var id = UTF8ToString($0);
            var x = $1;
            var y = $2;
            var width = $3;
            var height = $4;
            var bgcolour = UTF8ToString($5);;
            var canvas = window.wx.bitmaps.get(id);
            if ( canvas ) {
                const ctx = canvas.getContext('2d');
                ctx.save();
                ctx.fillStyle = bgcolour;
                ctx.fillRect(x, y, width, height);
                ctx.restore();
                //console.log("wxWASMDCImpl JS Fill Rect", bgcolour, canvas)
            }
            else {
                //console.log("wxWASMDCImpl JS Rect No canvas found");
            }
        }, m_cid, x, y, w, h, bgcolour);
    }


    MAIN_THREAD_EM_ASM_INT({
        var id = UTF8ToString($0);
        var text = UTF8ToString($1);
        var x = $2;
        var y = $3;
        var fgcolour = UTF8ToString($4);
        var bgcolour = UTF8ToString($5);
        var canvas = window.wx.bitmaps.get(id);
        if ( canvas ) {
            const ctx = canvas.getContext('2d');
            ctx.save();

            ctx.font = '12px serif';
            ctx.textBaseline = 'top';

            ctx.fillStyle = fgcolour;
            ctx.fillText(text, x, y);

            //ctx.strokeStyle = fgcolour;
            //ctx.strokeText(text, x, y);

            ctx.restore();
            //console.log("wxWASMDCImpl::DoDrawText JS Draw TExt", x, y, text, fgcolour);
        }
        else {
            //console.log("wxWASMDCImpl::DoDrawText JS Draw Text No canvas found");
        }
    }, m_cid, tdraw, x, y, fgcolour, bgcolour);


}

void wxWASMDCImpl::DoDrawRotatedText(const wxString& WXUNUSED(text),
                                    wxCoord WXUNUSED(x), wxCoord WXUNUSED(y),
                                    double WXUNUSED(angle))
{
    TRACE("wxWASMDCImpl::DoDrawRotatedText\n");
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    wxFAIL_MSG( "DrawRotatedText not implemented" );
}

// ---------------------------------------------------------------------------
// set GDI objects
// ---------------------------------------------------------------------------

void wxWASMDCImpl::SetPen(const wxPen& pen)
{
    TRACE("wxWASMDCImpl::SetPen\n");

    m_pen = pen.IsOk() ? pen : DEFAULT_PEN;

    SelectColour(m_pen.GetColour());

    int width = pen.GetWidth();

    MAIN_THREAD_EM_ASM_INT({
        var id = UTF8ToString($0);
        var width = $1;
        var canvas = window.wx.bitmaps.get(id);
        if ( canvas ) {
            const ctx = canvas.getContext('2d');
            ctx.lineWidth = width;
            //console.log("wxWASMDCImpl JS SetPen: width=", width, canvas);
        }
        else {
            //console.log("wxWASMDCImpl JS clear No canvas found");
        }
    }, m_cid, width);
}

void wxWASMDCImpl::SetBrush(const wxBrush& brush)
{
    m_brush = brush.IsOk() ? brush : DEFAULT_BRUSH;

    wxString col = brush.GetColour().GetAsString(wxC2S_CSS_SYNTAX);
    const char * colour = col.mb_str();

    TRACE("wxWASMDCImpl::SetBrush: %s\n", colour);
    MAIN_THREAD_EM_ASM_INT({
        var id = UTF8ToString($0);
        var canvas = window.wx.bitmaps.get(id);
        if ( canvas ) {
            const ctx = canvas.getContext('2d');
            ctx.fillStyle = UTF8ToString($1);
            //console.log("wxWASMDCImpl JS SetBrush : ", canvas);
        }
        else {
            //console.log("wxWASMDCImpl JS clear No canvas found");
        }
    }, m_cid, colour);
}

void wxWASMDCImpl::SelectColour(const wxColour& clr)
{
    wxString css = clr.GetAsString();
    const char * colour = css.mb_str();
    TRACE("wxWASMDCImpl::SetColour: %s\n", colour);
    MAIN_THREAD_EM_ASM_INT({
        var id = UTF8ToString($0);
        var colour = UTF8ToString($1);
        var canvas = window.wx.bitmaps.get(id);
        if ( canvas ) {
            const ctx = canvas.getContext('2d');
            ctx.strokeStyle = colour;
            //console.log("wxWASMDCImpl JS SetColour: ", colour, canvas);
        }
        else {
            //console.log("wxWASMDCImpl JS clear No canvas found");
        }
    }, m_cid, colour);

}

#if wxUSE_PALETTE
void wxWASMDCImpl::SetPalette(const wxPalette& WXUNUSED(palette))
{
    TRACE("wxWASMDCImpl::SetPalette\n");
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    wxFAIL_MSG( "SetPalette not implemented" );
}
#endif // wxUSE_PALETTE

void wxWASMDCImpl::SetFont(const wxFont& font)
{
    TRACE("wxWASMDCImpl::SetFont\n");
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    m_font = font;
}

void wxWASMDCImpl::SetBackground(const wxBrush& brush)
{
    wxCHECK_RET( IsOk(), wxT("invalid dc") );
    wxCHECK_RET( brush.IsOk(), wxT("invalid brush") );
    TRACE("wxWASMDCImpl::SetBackground\n");
    m_backgroundBrush = brush;
}

void wxWASMDCImpl::SetBackgroundMode(int mode)
{
    TRACE("wxWASMDCImpl::SetBackgroundMode\n");
    m_backgroundMode = mode;
}

void wxWASMDCImpl::SetLogicalFunction(wxRasterOperationMode function)
{
    TRACE("wxWASMDCImpl::SetLogicalFunction\n");
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    // NB: we could also support XOR, but for blitting only (via DSBLIT_XOR);
    //     and possibly others via SetSrc/DstBlendFunction()
    wxASSERT_MSG( function == wxCOPY,
                  "only wxCOPY logical function supported" );

    //m_logicalFunction = function;
}

bool wxWASMDCImpl::StartDoc(const wxString& WXUNUSED(message))
{
    TRACE("wxWASMDCImpl::StartDoc\n");
    // We might be previewing, so return true to let it continue.
    return true;
}

void wxWASMDCImpl::EndDoc()
{
    TRACE("wxWASMDCImpl::EndDoc\n");
}

void wxWASMDCImpl::StartPage()
{
    TRACE("wxWASMDCImpl::StartPage\n");
}

void wxWASMDCImpl::EndPage()
{
    TRACE("wxWASMDCImpl::EndPage\n");
}

// ---------------------------------------------------------------------------
// text metrics
// ---------------------------------------------------------------------------

wxCoord wxWASMDCImpl::GetCharHeight() const
{
    TRACE("wxWASMDCImpl::GetCharHeight fix it\n");
    wxCHECK_MSG( IsOk(), -1, wxT("invalid dc") );
    wxCHECK_MSG( m_font.IsOk(), -1, wxT("no font selected") );
    return 10;
/*
    int h = -1;
    //GetCurrentFont()->GetHeight(&h);
    return YDEV2LOGREL(h);
*/
}

wxCoord wxWASMDCImpl::GetCharWidth() const
{
    TRACE("wxWASMDCImpl::GetCharWidth do it for real\n");
    wxCHECK_MSG( IsOk(), -1, wxT("invalid dc") );
    wxCHECK_MSG( m_font.IsOk(), -1, wxT("no font selected") );
    return 10;
/*
    int w = -1;
    GetCurrentFont()->GetStringWidth("H", 1, &w);
    // VS: YDEV is corrent, it should *not* be XDEV, because font's are only
    //     scaled according to m_scaleY
    return YDEV2LOGREL(w);
*/
}

void wxWASMDCImpl::DoGetTextExtent(const wxString& string, wxCoord *x, wxCoord *y,
                           wxCoord *descent, wxCoord *externalLeading,
                           const wxFont *theFont) const
{
    wxCHECK_RET( IsOk(), wxT("invalid dc") );
    //wxCHECK_RET( !theFont || theFont->IsOk(), wxT("invalid font") );
    //wxCHECK_RET( m_font.IsOk(), wxT("no font selected") );

    const char * text = string.mb_str();
    *x = string.Length() * 10;
    if ( *x == 0 )
    {
        *x = 10;
    }
    if ( y )
    {
        *y = 10;
    }
    if ( descent )
    {
        *descent = 0;
    }
    if ( externalLeading )
    {
        *externalLeading = 0;
    }
    TRACE("wxWASMDCImpl::DoGetTextExtent: %s do it for real '%s' %dx%d\n", m_cid, text, *x, *y);
    return;

}


// ---------------------------------------------------------------------------
// mapping modes
// ---------------------------------------------------------------------------

void wxWASMDCImpl::DoGetSize(int *w, int *h) const
{
    TRACE("wxWASMDCImpl::DoGetSize: %s\n", m_cid);
    wxCHECK_RET( IsOk(), wxT("invalid dc") );

    *w = MAIN_THREAD_EM_ASM_INT({
        var id = UTF8ToString($0);
        var canvas = window.wx.bitmaps.get(id);
        if ( canvas ) {
            return canvas.width;
        }
        else {
            return -1;
        }
    }, m_cid);
    *h = MAIN_THREAD_EM_ASM_INT({
        var id = UTF8ToString($0);
        var canvas = window.wx.bitmaps.get(id);
        if ( canvas ) {
            return canvas.height;
        }
        else {
            return -1;
        }
    }, m_cid);
    TRACE("wxWASMDCImpl::DoGetSize: %s %dx%d\n", m_cid, *w, *h);

}

void wxWASMDCImpl::DoGetSizeMM(int *width, int *height) const
{
    TRACE("wxWASMDCImpl::DoGetSizeMM\n");

    int w = 0;
    int h = 0;
    GetSize(&w, &h);
    if ( width ) *width = w*(25.4/ 96);
    if ( height ) *height = h*(25.4/ 96);
}

wxSize wxWASMDCImpl::GetPPI() const
{
    TRACE("wxWASMDCImpl::DoPPI\n");
    return wxSize(int(double(GetMMToPXx()) * inches2mm),
                  int(double(GetMMToPXy()) * inches2mm));
}


// ---------------------------------------------------------------------------
// Blitting
// ---------------------------------------------------------------------------

bool wxWASMDCImpl::DoBlit(wxCoord xdest, wxCoord ydest,
                         wxCoord width, wxCoord height,
                         wxDC *source, wxCoord xsrc, wxCoord ysrc,
                         wxRasterOperationMode rop, bool useMask,
                         wxCoord xsrcMask, wxCoord ysrcMask)
{
    TRACE("wxWASMDCImpl::DoBlit\n");
    wxCHECK_MSG( IsOk(), false, "invalid dc" );
    wxCHECK_MSG( source, false, "invalid source dc" );


    wxWASMDCImpl * sdc = static_cast<wxWASMDCImpl *>(source->GetImpl());

    TRACE("wxWASMDCImpl::DoBlit %s -> %s %dx%d %dx%d %dx%d\n", sdc->m_cid, m_cid, xdest, ydest, xsrc, ysrc, width, height);

    MAIN_THREAD_EM_ASM_INT({
        var id_to = UTF8ToString($0);
        var id_from = UTF8ToString($1);
        var xdest = $2;
        var ydest = $3;
        var xsrc = $4;
        var ysrc = $5;
        var width = $6;
        var height = $7;
        var canvas_to = window.wx.bitmaps.get(id_to);
        var canvas_from = window.wx.bitmaps.get(id_from);
        if ( canvas_to && canvas_from ) {
            const ctx_to = canvas_to.getContext('2d');
            const ctx_from = canvas_from.getContext('2d');

            ctx_to.drawImage(canvas_from, xsrc, ysrc, width, height, xdest, ydest, width, height);

            /* transparenty...
            var img = ctx_from.getImageData(xsrc, ysrc, width, height);

            ctx_to.drawImage(img, xdest, ydest);
            */
            //console.log("wxWASMDCImpl JS Blit: ", canvas_to, canvas_from);
        }
        else {
            //console.log("wxWASMDCImpl JS Blit No canvas found");
        }
    }, m_cid, sdc->m_cid, xdest, ydest, xsrc, ysrc, width, height);

    DELAY();

    return true;
}

void wxWASMDCImpl::DoDrawBitmap(const wxBitmap &bmp, wxCoord x, wxCoord y, bool useMask)
{
    TRACE("wxWASMDCImpl::DoDrawBitmap\n");
    wxCHECK_RET( IsOk(), wxT("invalid dc") );
    wxCHECK_RET( bmp.IsOk(), wxT("invalid bitmap") );

    wxString src = bmp.GetCID();
    const char * src_cid = src.mb_str();

    MAIN_THREAD_EM_ASM_INT({
        var id_to = UTF8ToString($0);
        var id_src = UTF8ToString($1);
        var x = $2;
        var y = $3;

        var canvas_to = window.wx.bitmaps.get(id_to);
        var canvas_from = window.wx.bitmaps.get(id_src);
        //console.log("wxWASMDCImpl::DoDrawBitmap: ", canvas_to, canvas_from);
        if ( canvas_to && canvas_from ) {
            var ctx = canvas_to.getContext('2d');
            ctx.drawImage(canvas_from, x, y);
        }
        else {
            //console.log("wxWASMDCImpl::DoDrawBitmap canvas not found");
        }
    }, m_cid, src_cid, x, y);

}

void wxWASMDCImpl::DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y)
{
    TRACE("wxWASMDCImpl::DoDrawIcon\n");
    // VZ: egcs 1.0.3 refuses to compile this without cast, no idea why
    DoDrawBitmap((const wxBitmap&)icon, x, y, true);
}

