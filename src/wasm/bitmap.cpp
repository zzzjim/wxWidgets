/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/bitmap.cpp
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/icon.h"
    #include "wx/image.h"
#endif // WX_PRECOMP

#include "wx/bitmap.h"
#include "wx/cursor.h"
#include "wx/rawbmp.h"

#include <emscripten.h>

//#define TRACE(msg...)
#define TRACE(msg...) printf(msg)

//-----------------------------------------------------------------------------
// wxBitmapRefData
//-----------------------------------------------------------------------------

void DumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		TRACE("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			TRACE(" ");
			if ((i+1) % 16 == 0) {
				TRACE("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					TRACE(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					TRACE("   ");
				}
				TRACE("|  %s \n", ascii);
			}
		}
	}
}

class wxBitmapRefData: public wxGDIRefData
{
    public:
        wxBitmapRefData() {
            m_mask = NULL;
            m_js_id = wxString::Format("wxDCBitmap%d", (int)this);
            m_cid = m_js_id.mb_str();
        }

        wxBitmapRefData( int width, int height, int depth )
            : m_width(width), m_height(height), m_depth(depth)
        {
            m_mask = NULL;
            m_js_id = wxString::Format("wxDCBitmap%d", (int)this);
            m_cid = m_js_id.mb_str();
        }
        void Create()
        {
            TRACE("wxBitmapRefData::Create %s %dx%d %d\n" , m_cid, m_width, m_height, m_depth);
            MAIN_THREAD_EM_ASM_INT({
                var id = UTF8ToString($0);
                var width = $1;
                var height = $2;

/*
                var canvas = document.createElement('canvas');
                canvas.setAttribute('id', id);
	        canvas.width = width;
	        canvas.height = height;
                document.body.appendChild(canvas);
*/
                var canvas = new OffscreenCanvas(width, height);
                console.log("wxBitmapRefData::Create bitmap canvas: ", canvas);
                window.wx.bitmaps.set(id, canvas);
            }, m_cid, m_width, m_height);

        }
        void CopyFromImage(const wxImage& image)
        {
            int width = image.GetWidth();
            int height = image.GetHeight();
            unsigned char * src = image.GetData();
	    //DumpHex(src, width*height*3);
            MAIN_THREAD_EM_ASM_INT({
                var id = UTF8ToString($0);
                var src = $1;
                var width = $2;
                var height = $3;
                var canvas = window.wx.bitmaps.get(id);
                console.log("wxBitmapRefData::CopyFromImage: id", id);
                console.log("wxBitmapRefData::CopyFromImage: ", canvas);
                console.log("wxBitmapRefData::CopyFromImage: ", src);
                var ctx = canvas.getContext('2d');

                const img = ctx.createImageData(width, height);
                var dst = 0;
                for (let i = 0; i < (width*height); i++) {
                    var r = HEAP8[src];
                    var g = HEAP8[src+1];
                    var b = HEAP8[src+2];
                    if ( r == 1 )  r = 255;
                    if ( g == 1 )  g = 255;
                    if ( b == 1 )  b = 255;
                    //console.log("wxBitmapRefData::CopyFromImage: rgb(" + r + "," + g + "," + b + ")");
                    img.data[dst + 0] = r;//HEAP8[src];
                    img.data[dst + 1] = g;//HEAP8[(src+1)];
                    img.data[dst + 2] = b;//HEAP8[(src+2)];
                    img.data[dst + 3] = 255;

                    src += 3;
                    dst += 4;
                }
                ctx.putImageData(img, 0, 0);
            }, m_cid, src, width, height);

        }
        void CopyToImage(const wxImage& image)
        {
            int width = image.GetWidth();
            int height = image.GetHeight();
            unsigned char * dst = image.GetData();
            MAIN_THREAD_EM_ASM_INT({
                var id = UTF8ToString($0);
                var dst = $1;
                var width = $2;
                var height = $3;
                var canvas = window.wx.bitmaps.get(id);
                console.log("wxBitmapRefData::CopyFromImage: ", canvas);
                console.log("wxBitmapRefData::CopyFromImage: ", dst);
                var ctx = canvas.getContext('2d');

                const img = ctx.getImageData(0, 0, width, height);
                var src = 0;
                for (let i = 0; i < (width*height); i++) {
                    var r = img.data[src];
                    var g = img.data[src+1];
                    var b = img.data[src+2];
                    HEAP8[dst] = r;
                    HEAP8[dst+1] = g;
                    HEAP8[dst+2] = b;
                    src += 4;
                    dst += 3;
                }
            }, m_cid, dst, width, height);

        }

        wxBitmapRefData * Clone() const
        {
            TRACE("wxBitmapRefData::Clone\n");
            wxBitmapRefData * ref = new wxBitmapRefData(m_width, m_height, m_depth);
            ref->Create();
            return ref;
        }

        virtual ~wxBitmapRefData()
        {
            TRACE("wxBitmapRefData::~wxBitmapRefData\n");
            MAIN_THREAD_EM_ASM_INT({
                var id = UTF8ToString($0);
                var rc = window.wx.bitmaps.delete(id);
                var canvas = document.getElementById(id);
                if ( canvas ) {
                    canvas.parentNode.removeChild(canvas);
                }
                console.log("wxBitmapRefData::~wxBitmapRefData Removing bitmap canvas: " + rc);
            }, m_cid);
            delete m_mask;
        }

        wxMask *m_mask;

        // js canvas id
        wxString m_js_id;
        const char * m_cid;
        int m_width;
        int m_height;
        int m_depth;
};

#define M_BITMAP ((wxBitmapRefData *)m_refData)

//-----------------------------------------------------------------------------
// wxBitmap
//-----------------------------------------------------------------------------

wxIMPLEMENT_DYNAMIC_CLASS(wxBitmap, wxObject);

void wxBitmap::InitStandardHandlers()
{
}

wxBitmap::wxBitmap()
{
    TRACE("wxBitmap::wxBitmap\n");
}

/*
wxBitmap::wxBitmap(const wxBitmap& bmp)
{
    TRACE("wxBitmap::wxBitmap bmp\n");
    Ref(bmp);
}
*/
wxBitmap::wxBitmap(const char bits[], int width, int height, int depth )
{
    TRACE("wxBitmap::wxBitmap bmp bits %d\n", depth);
    Create(width, height, depth);

    // EM_ASM Copy the bits to a raw canvas
}

wxBitmap::wxBitmap(int width, int height, int depth)
{
    TRACE("wxBitmap::wxBitmap width heigth depth: %d\n", depth);
    Create(width, height, depth);
}

wxBitmap::wxBitmap(const wxSize& sz, int depth )
{
    TRACE("wxBitmap::wxBitmap size\n");
    Create(sz, depth);
}

// Create a wxBitmap from xpm data
wxBitmap::wxBitmap(const char* const* bits)
{
    wxASSERT(bits != NULL);
    
    TRACE("wxBitmap::wxBitmap bits xpm\n");
    if ( bits == NULL ) {
        TRACE("wxBitmap::wxBitmap null xpm NULL bitmap\n");
        *this = wxNullBitmap;
        return;
    }
    wxImage image(bits);
    TRACE("wxBitmap::wxBitmap image isok=%d\n", image.IsOk());
    if ( image.IsOk() == false ) {
        *this = wxNullBitmap;
        return;
    }

    UnRef();
    wxBitmapRefData * ref = new wxBitmapRefData(image.GetWidth(), image.GetHeight(), -1);
    ref->Create();
    ref->CopyFromImage(image);
    m_refData = ref;
}

wxBitmap::wxBitmap(const wxString &filename, wxBitmapType type )
{
    TRACE("wxBitmap::wxBitmap file\n");
    LoadFile(filename, type);
}

wxBitmap::wxBitmap(const wxImage& image, int depth, double WXUNUSED(scale) )
{
    TRACE("wxBitmap::wxBitmap image %d\n", depth);
    wxASSERT(image.IsOk());

    UnRef();
    wxBitmapRefData * ref = new wxBitmapRefData(image.GetWidth(), image.GetHeight(), -1);
    ref->Create();
    ref->CopyFromImage(image);
    m_refData = ref;
}

wxBitmap::wxBitmap(const wxCursor& cursor)
{
    TRACE("wxBitmap::wxBitmap cursor\n");
}

bool wxBitmap::Create(int width, int height, int depth )
{
    TRACE("wxBitmap::Create %dx%d %d\n", width, height, depth);
    UnRef();

    wxBitmapRefData * ref = new wxBitmapRefData(width, height, depth);
    ref->Create();
    m_refData = ref;

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

int wxBitmap::GetWidth() const
{
    //TRACE("wxBitmap::GetWidth\n");
    return M_BITMAP->m_width;
}

int wxBitmap::GetHeight() const
{
    //TRACE("wxBitmap::GetHeight\n");
    return M_BITMAP->m_height;
}

int wxBitmap::GetDepth() const
{
    TRACE("wxBitmap::GetDepth\n");
    return M_BITMAP->m_depth;
}


#if wxUSE_IMAGE
wxImage wxBitmap::ConvertToImage() const
{
    TRACE("wxBitmap::ConvertToImage\n");
    wxCHECK_MSG( IsOk(), wxNullImage, wxT("invalid bitmap") );

    wxImage img(GetWidth(), GetHeight());

    M_BITMAP->CopyToImage(img);;

    return img;
}

#endif // wxUSE_IMAGE

wxMask *wxBitmap::GetMask() const
{
    wxCHECK_MSG( IsOk(), NULL, wxT("invalid bitmap") );
    return M_BITMAP->m_mask;
}

void wxBitmap::SetMask(wxMask *mask)
{
    TRACE("wxBitmap::SetMask\n");
    wxCHECK_RET( IsOk(), wxT("invalid bitmap") );
    AllocExclusive();
    delete M_BITMAP->m_mask;
    M_BITMAP->m_mask = mask;
}

wxBitmap wxBitmap::GetSubBitmap(const wxRect& rect) const
{
    TRACE("wxBitmap::GetSubBitmap\n");
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
    TRACE("wxBitmap::LoadFile: %s\n", (const char*)name.mb_str());
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
    *this = *((wxBitmap*)(&icon));
    return IsOk();
}

#if WXWIN_COMPATIBILITY_3_0
void wxBitmap::SetHeight(int height)
{
    TRACE("wxBitmap::SetHeight\n");
}

void wxBitmap::SetWidth(int width)
{
    TRACE("wxBitmap::SetWidth\n");
}

void wxBitmap::SetDepth(int depth)
{
    TRACE("wxBitmap::SetDepth\n");
}
#endif

void *wxBitmap::GetRawData(wxPixelDataBase& data, int bpp)
{
    TRACE("wxBitmap::GetRawData\n");
    return NULL;
}

void wxBitmap::UngetRawData(wxPixelDataBase& WXUNUSED(data))
{
    TRACE("wxBitmap::UngetRawData\n");
}

wxGDIRefData *wxBitmap::CreateGDIRefData() const
{
    TRACE("wxBitmap::CreateGDIRefData\n");
    return new wxBitmapRefData;
}

wxGDIRefData *wxBitmap::CloneGDIRefData(const wxGDIRefData *data) const
{
    TRACE("wxBitmap::CloneGDIRefData\n");
    return M_BITMAP->Clone();
}

bool wxBitmap::HasAlpha() const
{
    TRACE("wxBitmap::HasAlpha\n");
    return true;
}

const wxString& wxBitmap::GetCID() const
{
    return M_BITMAP->m_js_id;
}


/*
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
    TRACE("wxMask::wxMask bitmap\n");
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

    return false;
}

bool wxMask::InitFromMonoBitmap(const wxBitmap& bitmap)
{
    return false;
}
*/
