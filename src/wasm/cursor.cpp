/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/cursor.cpp
// Purpose:     wxCursor class
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#include "wx/cursor.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/utils.h"
    #include "wx/icon.h"
    #include "wx/gdicmn.h"
    #include "wx/image.h"
#endif

//-----------------------------------------------------------------------------
// wxCursor
//-----------------------------------------------------------------------------

class wxCursorRefData: public wxGDIRefData
{
public:

    wxCursorRefData();
    virtual ~wxCursorRefData();

private:
    wxDECLARE_NO_COPY_CLASS(wxCursorRefData);
};

wxCursorRefData::wxCursorRefData()
{
}

wxCursorRefData::~wxCursorRefData()
{
}

//-----------------------------------------------------------------------------

#define M_CURSORDATA ((wxCursorRefData *)m_refData)

wxIMPLEMENT_DYNAMIC_CLASS(wxCursor,wxObject);

wxCursor::wxCursor()
{

}

void wxCursor::InitFromStock( wxStockCursor cursorId )
{
    m_refData = new wxCursorRefData();
}

wxCursor::wxCursor(const wxString& WXUNUSED(name),
                    wxBitmapType WXUNUSED(type),
                    int WXUNUSED(hotSpotX), int WXUNUSED(hotSpotY))
{
   wxFAIL_MSG( wxT("wxCursor creation from file not yet implemented") );
}

#if wxUSE_IMAGE
wxCursor::wxCursor( const wxImage & WXUNUSED(image) )
{
   wxFAIL_MSG( wxT("wxCursor creation from wxImage not yet implemented") );
}
#endif

wxCursor::~wxCursor()
{
}

wxGDIRefData *wxCursor::CreateGDIRefData() const
{
    return new wxCursorRefData;
}

wxGDIRefData *
wxCursor::CloneGDIRefData(const wxGDIRefData * WXUNUSED(data)) const
{
    wxFAIL_MSG( wxS("Cloning cursors is not implemented in wxX11.") );

    return new wxCursorRefData;
}

WXCursor wxCursor::GetCursor() const
{
}

//-----------------------------------------------------------------------------
// busy cursor routines
//-----------------------------------------------------------------------------

/* extern */ wxCursor g_globalCursor;

static wxCursor  gs_savedCursor;
static int       gs_busyCount = 0;

const wxCursor &wxBusyCursor::GetStoredCursor()
{
    return gs_savedCursor;
}

const wxCursor wxBusyCursor::GetBusyCursor()
{
    return wxCursor(wxCURSOR_WATCH);
}

void wxEndBusyCursor()
{
    if (--gs_busyCount > 0)
        return;

    wxSetCursor( gs_savedCursor );
    gs_savedCursor = wxNullCursor;

    if (wxTheApp)
        wxTheApp->ProcessIdle();
}

void wxBeginBusyCursor( const wxCursor *WXUNUSED(cursor) )
{
    if (gs_busyCount++ > 0)
        return;

    wxASSERT_MSG( !gs_savedCursor.IsOk(),
                  wxT("forgot to call wxEndBusyCursor, will leak memory") );

    gs_savedCursor = g_globalCursor;

    wxSetCursor( wxCursor(wxCURSOR_WATCH) );

    if (wxTheApp)
        wxTheApp->ProcessIdle();
}

bool wxIsBusy()
{
    return gs_busyCount > 0;
}

void wxSetCursor( const wxCursor& cursor )
{
    g_globalCursor = cursor;
}
