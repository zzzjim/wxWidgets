/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/bitmap.cpp
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/icon.h"
    #include "wx/image.h"
#endif // WX_PRECOMP

#include "wx/bitmap.h"
#include "wx/cursor.h"
#include "wx/rawbmp.h"

//-----------------------------------------------------------------------------
// wxBitmapRefData
//-----------------------------------------------------------------------------

class wxBitmapRefData: public wxGDIRefData
{
    public:
        wxBitmapRefData() { m_mask = NULL; }

        wxBitmapRefData( int width, int height, int depth )
        {
            m_mask = NULL;
        }

        virtual ~wxBitmapRefData() { delete m_mask; }

        wxMask *m_mask;

};

//-----------------------------------------------------------------------------
// wxBitmap
//-----------------------------------------------------------------------------

wxIMPLEMENT_DYNAMIC_CLASS(wxBitmap, wxObject);

void wxBitmap::InitStandardHandlers()
{
}

wxBitmap::wxBitmap()
{
}

wxBitmap::wxBitmap(const wxBitmap& bmp)
{
    Ref(bmp);
}
wxBitmap::wxBitmap(const char bits[], int width, int height, int depth )
{
}

wxBitmap::wxBitmap(int width, int height, int depth)
{
    Create(width, height, depth);
}

wxBitmap::wxBitmap(const wxSize& sz, int depth )
{
    Create(sz, depth);
}

// Create a wxBitmap from xpm data
wxBitmap::wxBitmap(const char* const* bits)
{
}

wxBitmap::wxBitmap(const wxString &filename, wxBitmapType type )
{
    LoadFile(filename, type);
}

wxBitmap::wxBitmap(const wxImage& image, int depth, double WXUNUSED(scale) )
{
}

wxBitmap::wxBitmap(const wxCursor& cursor)
{
}

bool wxBitmap::Create(int width, int height, int depth )
{
    UnRef();
    return true;
}

bool wxBitmap::Create(const wxSize& sz, int depth )
{
    return Create(sz.GetWidth(), sz.GetHeight(), depth);
}

bool wxBitmap::Create(int width, int height, const wxDC& WXUNUSED(dc))
{
    return Create(width, height);
}

int wxBitmap::GetHeight() const
{
    return 100;
}

int wxBitmap::GetWidth() const
{
    return 100;
}

int wxBitmap::GetDepth() const
{
    return 3;
}


#if wxUSE_IMAGE
wxImage wxBitmap::ConvertToImage() const
{
    return wxNullImage;
}

#endif // wxUSE_IMAGE

wxMask *wxBitmap::GetMask() const
{
    return NULL;
}

void wxBitmap::SetMask(wxMask *mask)
{
    AllocExclusive();
}

wxBitmap wxBitmap::GetSubBitmap(const wxRect& rect) const
{
    return wxNullBitmap;
}


bool wxBitmap::SaveFile(const wxString &name, wxBitmapType type,
              const wxPalette *WXUNUSED(palette) ) const
{
    #if wxUSE_IMAGE
    //Try to save using wx
    wxImage image = ConvertToImage();
    if (image.IsOk() && image.SaveFile(name, type))
        return true;
    #endif
    //save
    return false;
}

bool wxBitmap::LoadFile(const wxString &name, wxBitmapType type)
{
#if wxUSE_IMAGE
    //Try to load using wx
    wxImage image;
    if (image.LoadFile(name, type) && image.IsOk())
    {
        *this = wxBitmap(image);
        return true;
    }
    else
#endif
    {
        return false;
    }
}


#if wxUSE_PALETTE
wxPalette *wxBitmap::GetPalette() const
{
    return 0;
}

void wxBitmap::SetPalette(const wxPalette& WXUNUSED(palette))
{
}

#endif // wxUSE_PALETTE

// copies the contents and mask of the given (colour) icon to the bitmap
bool wxBitmap::CopyFromIcon(const wxIcon& icon)
{
    *this = icon;
    return IsOk();
}

#if WXWIN_COMPATIBILITY_3_0
void wxBitmap::SetHeight(int height)
{
}

void wxBitmap::SetWidth(int width)
{
}

void wxBitmap::SetDepth(int depth)
{
}
#endif

void *wxBitmap::GetRawData(wxPixelDataBase& data, int bpp)
{
    return NULL;
}

void wxBitmap::UngetRawData(wxPixelDataBase& WXUNUSED(data))
{
}

wxGDIRefData *wxBitmap::CreateGDIRefData() const
{
    return new wxBitmapRefData;
}

wxGDIRefData *wxBitmap::CloneGDIRefData(const wxGDIRefData *data) const
{
}

bool wxBitmap::HasAlpha() const
{
}

//-----------------------------------------------------------------------------
// wxMask
//-----------------------------------------------------------------------------

wxIMPLEMENT_DYNAMIC_CLASS(wxMask, wxObject);

wxMask::wxMask()
{
}

wxMask::wxMask(const wxMask &mask)
{
}

wxMask& wxMask::operator=(const wxMask &mask)
{
}

wxMask::wxMask(const wxBitmap& bitmap, const wxColour& colour)
{
    Create(bitmap, colour);
}

wxMask::wxMask(const wxBitmap& bitmap, int paletteIndex)
{
    Create(bitmap, paletteIndex);
}

wxMask::wxMask(const wxBitmap& bitmap)
{
    Create(bitmap);
}

wxMask::~wxMask()
{
}

// this function is called from Create() to free the existing mask data
void wxMask::FreeData()
{
}

bool wxMask::InitFromColour(const wxBitmap& bitmap, const wxColour& colour)
{
    if (!bitmap.IsOk())
        return false;

}

bool wxMask::InitFromMonoBitmap(const wxBitmap& bitmap)
{
    return false;
}


