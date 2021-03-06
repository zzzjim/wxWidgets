/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/font.cpp
// Purpose:     wxFont class
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/font.h"

#ifndef WX_PRECOMP
    #include "wx/string.h"
    #include "wx/utils.h"       // for wxGetDisplay()
    #include "wx/settings.h"
    #include "wx/gdicmn.h"
    #include "wx/wxcrtvararg.h" // for wxSscanf
#endif

#include "wx/fontutil.h"    // for wxNativeFontInfo
#include "wx/tokenzr.h"
#include "wx/fontenum.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// the default size (in points) for the fonts
static const int wxDEFAULT_FONT_SIZE = 12;

#if wxUSE_UNICODE
#else
// ----------------------------------------------------------------------------
// wxXFont
// ----------------------------------------------------------------------------

// For every wxFont, there must be a font for each display and scale requested.
// So these objects are stored in wxFontRefData::m_fonts
class wxXFont : public wxObject
{
public:
    wxXFont();
    virtual ~wxXFont();

    WXFontStructPtr     m_fontStruct;   // XFontStruct
    WXDisplay*          m_display;      // XDisplay
    int                 m_scale;        // Scale * 100
};

wxXFont::wxXFont()
{
    m_fontStruct = (WXFontStructPtr) 0;
    m_display = (WXDisplay*) 0;
    m_scale = 100;
}

wxXFont::~wxXFont()
{
    // Freeing the font used to produce a segv, but
    // appears to be OK now (bug fix in X11?)
    XFontStruct* fontStruct = (XFontStruct*) m_fontStruct;
    XFreeFont((Display*) m_display, fontStruct);
}
#endif

// ----------------------------------------------------------------------------
// wxFontRefData
// ----------------------------------------------------------------------------

class wxFontRefData: public wxGDIRefData
{
friend class wxFont;

public:
    wxFontRefData(int size = wxDEFAULT,
                  wxFontFamily family = wxFONTFAMILY_DEFAULT,
                  wxFontStyle style = wxFONTSTYLE_NORMAL,
                  wxFontWeight weight = wxFONTWEIGHT_NORMAL,
                  bool underlined = false,
                  bool strikethrough = false,
                  const wxString& faceName = wxEmptyString,
                  wxFontEncoding encoding = wxFONTENCODING_DEFAULT);

    // copy cstr
    wxFontRefData(const wxFontRefData& data);

    // from XFLD
    wxFontRefData(const wxString& fontname);

    // dstr
    virtual ~wxFontRefData();

    // setters: all of them also take care to modify m_nativeFontInfo if we
    // have it so as to not lose the information not carried by our fields
    void SetFractionalPointSize(float pointSize);
    void SetFamily(wxFontFamily family);
    void SetStyle(wxFontStyle style);
    void SetNumericWeight(int weight);
    void SetUnderlined(bool underlined);
    void SetStrikethrough(bool strikethrough);
    bool SetFaceName(const wxString& facename);
    void SetEncoding(wxFontEncoding encoding);

    // and this one also modifies all the other font data fields
    void SetNativeFontInfo(const wxNativeFontInfo& info);

protected:
    // common part of all ctors
    void Init(int size,
              wxFontFamily family,
              wxFontStyle style,
              wxFontWeight weight,
              bool underlined,
              bool strikethrough,
              const wxString& faceName,
              wxFontEncoding encoding);

    // set all fields from (already initialized and valid) m_nativeFontInfo
    void InitFromNative();

    // font attributes
    float         m_pointSize;
    wxFontFamily  m_family;
    wxFontStyle   m_style;
    int           m_weight;
    bool          m_underlined;
    bool          m_strikethrough;
    wxString      m_faceName;
    wxFontEncoding m_encoding;   // Unused in Unicode mode

    wxNativeFontInfo m_nativeFontInfo;


#if wxUSE_UNICODE
#else
    // A list of wxXFonts
    wxList        m_fonts;
#endif
};

#define M_FONTDATA ((wxFontRefData*)m_refData)

// ----------------------------------------------------------------------------
// wxFontRefData
// ----------------------------------------------------------------------------

void wxFontRefData::Init(int pointSize,
                         wxFontFamily family,
                         wxFontStyle style,
                         wxFontWeight weight,
                         bool underlined,
                         bool strikethrough,
                         const wxString& faceName,
                         wxFontEncoding encoding)
{
    m_family = family == wxFONTFAMILY_DEFAULT ? wxFONTFAMILY_SWISS : family;

    m_faceName = faceName;

    // we accept both wxDEFAULT and wxNORMAL here - should we?
    m_style = static_cast<int>(style) == wxDEFAULT ? wxFONTSTYLE_NORMAL : style;
    m_weight = static_cast<int>(weight) == wxDEFAULT ? wxFONTWEIGHT_NORMAL : weight;

    m_underlined = underlined;
    m_strikethrough = strikethrough;
    m_encoding = encoding;

#if wxUSE_UNICODE
    // just the family
    if ( faceName.empty() || !wxFontEnumerator::IsValidFacename(faceName) )
    {
        // TODO: scan system for valid fonts matching the given family instead
        //       of hardcoding them here
        switch ( m_family )
        {
            case wxFONTFAMILY_TELETYPE:
                m_faceName = wxT("monospace");
                break;

            case wxFONTFAMILY_ROMAN:
                m_faceName = wxT("serif");
                break;

            default:
                m_faceName = wxT("sans");
        }
    }
    else // specified face name is available, use it
    {
        m_faceName = faceName;
    }

    m_nativeFontInfo.SetFaceName(m_faceName);
    m_nativeFontInfo.SetNumericWeight(m_weight);
    m_nativeFontInfo.SetStyle((wxFontStyle)m_style);
    m_nativeFontInfo.SetUnderlined(underlined);
#endif // wxUSE_UNICODE

    SetFractionalPointSize(static_cast<float>(pointSize));
}

void wxFontRefData::InitFromNative()
{
#if wxUSE_UNICODE
#else // X11
#endif // Pango/X11
}

wxFontRefData::wxFontRefData( const wxFontRefData& data )
             : wxGDIRefData()
{
    m_pointSize = data.m_pointSize;
    m_family = data.m_family;
    m_style = data.m_style;
    m_weight = data.m_weight;

    m_underlined = data.m_underlined;

    m_faceName = data.m_faceName;
    m_encoding = data.m_encoding;

    m_nativeFontInfo = data.m_nativeFontInfo;
}

wxFontRefData::wxFontRefData(int size, wxFontFamily family, wxFontStyle style,
                             wxFontWeight weight, bool underlined, bool strikethrough,
                             const wxString& faceName,
                             wxFontEncoding encoding)
{
    Init(size, family, style, weight, underlined, strikethrough, faceName, encoding);
}

wxFontRefData::wxFontRefData(const wxString& fontname)
{
    // VZ: FromString() should really work in both cases, doesn't it?
#if wxUSE_UNICODE
    m_nativeFontInfo.FromString( fontname );
#else
    m_nativeFontInfo.SetXFontName(fontname);
#endif

    InitFromNative();
}

wxFontRefData::~wxFontRefData()
{
}

// ----------------------------------------------------------------------------
// wxFontRefData SetXXX()
// ----------------------------------------------------------------------------

void wxFontRefData::SetFractionalPointSize(float pointSize)
{
    // NB: Pango doesn't support point sizes less than 1
    m_pointSize = pointSize == wxDEFAULT || pointSize < 1 ? wxDEFAULT_FONT_SIZE
                                                          : pointSize;

#if wxUSE_UNICODE
    m_nativeFontInfo.SetFractionalPointSize(m_pointSize);
#endif
}

void wxFontRefData::SetFamily(wxFontFamily family)
{
    m_family = family;

    // TODO: what are we supposed to do with m_nativeFontInfo here?
}

void wxFontRefData::SetStyle(wxFontStyle style)
{
    m_style = style;

#if wxUSE_UNICODE
#endif
}

void wxFontRefData::SetNumericWeight(int weight)
{
    m_weight = weight;
}

void wxFontRefData::SetUnderlined(bool underlined)
{
    m_nativeFontInfo.SetUnderlined(underlined);
    // the XLFD doesn't have "underlined" field anyhow
}

void wxFontRefData::SetStrikethrough(bool strikethrough)
{
    m_nativeFontInfo.SetStrikethrough(strikethrough);
}

bool wxFontRefData::SetFaceName(const wxString& facename)
{
    m_faceName = facename;
    return true;
}

void wxFontRefData::SetEncoding(wxFontEncoding encoding)
{
    m_encoding = encoding;
}

void wxFontRefData::SetNativeFontInfo(const wxNativeFontInfo& info)
{
    // previously cached fonts shouldn't be used

    m_nativeFontInfo = info;

    m_family = info.GetFamily();

    // set all the other font parameters from the native font info
    InitFromNative();
}

// ----------------------------------------------------------------------------
// wxFont
// ----------------------------------------------------------------------------

wxFont::wxFont(const wxNativeFontInfo& info)
{
#if wxUSE_UNICODE
    Create( info.GetPointSize(),
            info.GetFamily(),
            info.GetStyle(),
            info.GetWeight(),
            info.GetUnderlined(),
            info.GetFaceName(),
            info.GetEncoding() );

    if ( info.GetStrikethrough() )
        SetStrikethrough(true);
#else
    (void) Create(info.GetXFontName());
#endif
}

bool wxFont::Create(int pointSize,
                    wxFontFamily family,
                    wxFontStyle style,
                    wxFontWeight weight,
                    bool underlined,
                    const wxString& faceName,
                    wxFontEncoding encoding)
{
    UnRef();

    m_refData = new wxFontRefData(pointSize, family, style, weight,
                                  underlined, false, faceName, encoding);

    return true;
}

bool wxFont::Create(const wxString& fontname, wxFontEncoding enc)
{
#if wxUSE_UNICODE
    wxUnusedVar(enc);
#endif

    *this = wxSystemSettings::GetFont( wxSYS_DEFAULT_GUI_FONT);
    return true;
}

wxFont::~wxFont()
{
}

wxGDIRefData *wxFont::CreateGDIRefData() const
{
    return new wxFontRefData;
}

wxGDIRefData *wxFont::CloneGDIRefData(const wxGDIRefData *data) const
{
    return new wxFontRefData(*static_cast<const wxFontRefData *>(data));
}

// ----------------------------------------------------------------------------
// change the font attributes
// ----------------------------------------------------------------------------

void wxFont::Unshare()
{
    // Don't change shared data
    if (!m_refData)
    {
        m_refData = new wxFontRefData();
    }
    else
    {
        wxFontRefData* ref = new wxFontRefData(*(wxFontRefData*)m_refData);
        UnRef();
        m_refData = ref;
    }
}

// ----------------------------------------------------------------------------
// accessors
// ----------------------------------------------------------------------------

float wxFont::GetFractionalPointSize() const
{
    wxCHECK_MSG( IsOk(), 0, wxT("invalid font") );

    return M_FONTDATA->m_nativeFontInfo.GetFractionalPointSize();
}

wxString wxFont::GetFaceName() const
{
    wxCHECK_MSG( IsOk(), wxEmptyString, wxT("invalid font") );

    return M_FONTDATA->m_nativeFontInfo.GetFaceName();
}

wxFontFamily wxFont::DoGetFamily() const
{
    return M_FONTDATA->m_family;
}

wxFontStyle wxFont::GetStyle() const
{
    wxCHECK_MSG( IsOk(), wxFONTSTYLE_MAX, wxT("invalid font") );

    return M_FONTDATA->m_style;
}

int wxFont::GetNumericWeight() const
{
    wxCHECK_MSG( IsOk(), wxFONTWEIGHT_MAX, wxT("invalid font") );

    return M_FONTDATA->m_weight;
}

bool wxFont::GetUnderlined() const
{
    wxCHECK_MSG( IsOk(), false, wxT("invalid font") );

    return M_FONTDATA->m_nativeFontInfo.GetUnderlined();
}

bool wxFont::GetStrikethrough() const
{
    wxCHECK_MSG( IsOk(), false, wxT("invalid font") );

    return M_FONTDATA->m_nativeFontInfo.GetStrikethrough();
}

#if defined( __WXX11__ ) && ( wxUSE_PANGO == 0 )
bool wxNativeFontInfo::GetStrikethrough() const
{
   return false;
}
#endif

wxFontEncoding wxFont::GetEncoding() const
{
    wxCHECK_MSG( IsOk(), wxFONTENCODING_DEFAULT, wxT("invalid font") );

#if wxUSE_UNICODE
    // unicode didn't use font encoding
    return wxFONTENCODING_DEFAULT;
#else
    return M_FONTDATA->m_encoding;
#endif

}

const wxNativeFontInfo *wxFont::GetNativeFontInfo() const
{
    wxCHECK_MSG( IsOk(), NULL, wxT("invalid font") );

#if wxUSE_UNICODE
#else
    if ( M_FONTDATA->m_nativeFontInfo.GetXFontName().empty() )
        GetInternalFont();
#endif

    return &(M_FONTDATA->m_nativeFontInfo);
}

bool wxFont::IsFixedWidth() const
{
    wxCHECK_MSG( IsOk(), false, wxT("invalid font") );

#if wxUSE_UNICODE
   return wxFontBase::IsFixedWidth();
#else
    // Robert, is this right? HasNativeFont doesn't exist.
    if ( true )
    //    if ( M_FONTDATA->HasNativeFont() )
    {
        // the monospace fonts are supposed to have "M" in the spacing field
        wxString spacing = M_FONTDATA->
                            m_nativeFontInfo.GetXFontComponent(wxXLFD_SPACING);

        return spacing.Upper() == wxT('M');
    }
   // Unreaceable code for now
   // return wxFontBase::IsFixedWidth();
#endif

}

// ----------------------------------------------------------------------------
// change font attributes
// ----------------------------------------------------------------------------

void wxFont::SetFractionalPointSize(float pointSize)
{
    Unshare();

    M_FONTDATA->SetFractionalPointSize(pointSize);
}

void wxFont::SetFamily(wxFontFamily family)
{
    Unshare();

    M_FONTDATA->SetFamily(family);
}

void wxFont::SetStyle(wxFontStyle style)
{
    Unshare();

    M_FONTDATA->SetStyle(style);
}

void wxFont::SetNumericWeight(int weight)
{
    Unshare();

    M_FONTDATA->SetNumericWeight(weight);
}

bool wxFont::SetFaceName(const wxString& faceName)
{
    Unshare();

    return M_FONTDATA->SetFaceName(faceName) &&
        wxFontBase::SetFaceName(faceName);
}

void wxFont::SetUnderlined(bool underlined)
{
    Unshare();

    M_FONTDATA->SetUnderlined(underlined);
}

void wxFont::SetStrikethrough(bool strikethrough)
{
    Unshare();

    M_FONTDATA->SetStrikethrough(strikethrough);
}

void wxFont::SetEncoding(wxFontEncoding encoding)
{
    Unshare();

    M_FONTDATA->SetEncoding(encoding);
}

void wxFont::DoSetNativeFontInfo( const wxNativeFontInfo& info )
{
    Unshare();

    M_FONTDATA->SetNativeFontInfo( info );
}

#if wxUSE_PANGO
// Although we don't use this function yet, but we must create it here.
// first, for the prepare the unicode drawing support in wxUniv/x11 port.
// If we use pango to draw the text, then we must set some attributes
// for pango layout, such as "strikethrough" and "underline".
bool wxFont::SetPangoAttrs(PangoLayout* layout) const
{
    if ( !IsOk() || !(GetUnderlined() || GetStrikethrough()) )
        return false;

    PangoAttrList* attrs = pango_attr_list_new();
    PangoAttribute* a;

    if ( GetUnderlined() )
    {
        a = pango_attr_underline_new(PANGO_UNDERLINE_SINGLE);
        pango_attr_list_insert(attrs, a);
    }
    if ( GetStrikethrough() )
    {
        a = pango_attr_strikethrough_new(true);
        pango_attr_list_insert(attrs, a);
    }

    pango_layout_set_attributes(layout, attrs);
    pango_attr_list_unref(attrs);

    return true;
}
#endif

#if !wxUSE_UNICODE

// ----------------------------------------------------------------------------
// X11 implementation
// ----------------------------------------------------------------------------

// Find an existing, or create a new, XFontStruct
// based on this wxFont and the given scale. Append the
// font to list in the private data for future reference.
wxXFont* wxFont::GetInternalFont(double scale, WXDisplay* display) const
{
    if ( !IsOk() )
        return NULL;

    long intScale = long(scale * 100.0 + 0.5); // key for wxXFont
    int pointSize = (M_FONTDATA->m_pointSize * 10 * intScale) / 100;

    // search existing fonts first
    wxList::compatibility_iterator node = M_FONTDATA->m_fonts.GetFirst();
    while (node)
    {
        wxXFont* f = (wxXFont*) node->GetData();
        if ((!display || (f->m_display == display)) && (f->m_scale == intScale))
            return f;
        node = node->GetNext();
    }

    wxString xFontName = M_FONTDATA->m_nativeFontInfo.GetXFontName();
    if (xFontName == "-*-*-*-*-*--*-*-*-*-*-*-*-*")
      // wxFont constructor not called with native font info parameter => take M_FONTDATA values
      xFontName.Clear();

    // not found, create a new one
    XFontStruct *font = (XFontStruct *)
                        wxLoadQueryNearestFont(pointSize,
                                               M_FONTDATA->m_family,
                                               M_FONTDATA->m_style,
                                               M_FONTDATA->m_weight,
                                               M_FONTDATA->m_underlined,
                                               wxT(""),
                                               M_FONTDATA->m_encoding,
                                               & xFontName);

    if ( !font )
    {
        wxFAIL_MSG( wxT("Could not allocate even a default font -- something is wrong.") );

        return NULL;
    }

    wxXFont* f = new wxXFont;
    f->m_fontStruct = (WXFontStructPtr)font;
    f->m_display = ( display ? display : wxGetDisplay() );
    f->m_scale = intScale;
    M_FONTDATA->m_fonts.Append(f);

    return f;
}

WXFontStructPtr wxFont::GetFontStruct(double scale, WXDisplay* display) const
{
    wxXFont* f = GetInternalFont(scale, display);

    return (f ? f->m_fontStruct : (WXFontStructPtr) 0);
}

#endif // !wxUSE_UNICODE
