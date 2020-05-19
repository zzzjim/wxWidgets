/////////////////////////////////////////////////////////////////////////////
// Name:        wx/wasm/window.h
// Purpose:     wxWindow class
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WINDOW_H_
#define _WX_WINDOW_H_

#include "wx/region.h"

class WXDLLIMPEXP_CORE wxWindowWASM : public wxWindowBase
{
    friend class WXDLLIMPEXP_FWD_CORE wxDC;
    friend class WXDLLIMPEXP_FWD_CORE wxWindowDC;
    friend class WXDLLIMPEXP_FWD_CORE wxClientDCImpl;
    friend class WXDLLIMPEXP_FWD_CORE wxPaintDCImpl;
    friend class WXDLLIMPEXP_FWD_CORE wxMemoryDCImpl;
    friend class WXDLLIMPEXP_FWD_CORE wxTopLevelWindowWASM;
    friend class WXDLLIMPEXP_FWD_CORE wxWindowDCImpl;
    friend class WXDLLIMPEXP_FWD_CORE wxWASMDCImpl;

public:
    wxWindowWASM() { Init(); }

    wxWindowWASM(wxWindow *parent,
        wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxPanelNameStr)
    {
        Init();
        Create(parent, id, pos, size, style, name);
    }

    virtual ~wxWindowWASM();

    bool Create(wxWindow *parent,
        wxWindowID id,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxPanelNameStr);

    virtual void Raise();
    virtual void Lower();

    virtual void SetLabel( const wxString &WXUNUSED(label) ) {}
    virtual wxString GetLabel() const { return wxEmptyString; }

    virtual bool Show( bool show = true );
    virtual bool Enable( bool enable = true );

    virtual void SetFocus();

    virtual void WarpPointer(int x, int y);

    virtual void Refresh( bool eraseBackground = true,
                          const wxRect *rect = (const wxRect *) NULL );
    virtual void Update();

    virtual bool SetBackgroundColour( const wxColour &colour );
    virtual bool SetForegroundColour( const wxColour &colour );

    virtual bool SetCursor( const wxCursor &cursor );
    virtual bool SetFont( const wxFont &font );

    virtual int GetCharHeight() const;
    virtual int GetCharWidth() const;

    virtual void ScrollWindow( int dx, int dy, const wxRect* rect = NULL );
    virtual void SetScrollbar( int orient, int pos, int thumbVisible,
                               int range, bool refresh = true ) wxOVERRIDE;

#if wxUSE_DRAG_AND_DROP
    virtual void SetDropTarget( wxDropTarget *dropTarget );
#endif // wxUSE_DRAG_AND_DROP

    // Accept files for dragging
    virtual void DragAcceptFiles(bool accept);

    // Get the unique identifier of a window
    virtual WXWindow GetHandle() const { return NULL; }//WASMGetMainWindow(); }

    // implementation from now on
    // --------------------------
protected:
    // Responds to colour changes: passes event on to children.
    void OnSysColourChanged(wxSysColourChangedEvent& event);

protected:
    // implement the base class pure virtuals
    virtual void DoGetTextExtent(const wxString& string,
                                 int *x, int *y,
                                 int *descent = NULL,
                                 int *externalLeading = NULL,
                                 const wxFont *font = NULL) const;
    virtual void DoClientToScreen( int *x, int *y ) const;
    virtual void DoScreenToClient( int *x, int *y ) const;
    virtual void DoGetPosition( int *x, int *y ) const;
    virtual void DoGetSize( int *width, int *height ) const;
    virtual void DoGetClientSize( int *width, int *height ) const;
    virtual void DoSetSize(int x, int y,
        int width, int height,
        int sizeFlags = wxSIZE_AUTO);
    virtual void DoSetClientSize(int width, int height);
    virtual void DoMoveWindow(int x, int y, int width, int height);
    virtual void DoCaptureMouse();
    virtual void DoReleaseMouse();
    virtual void KillFocus();

#if wxUSE_TOOLTIPS
    virtual void DoSetToolTip( wxToolTip *tip );
#endif // wxUSE_TOOLTIPS


    // Generates paint events from m_updateRegion
    void SendPaintEvents();

    // Generates paint events from flag
    void SendNcPaintEvents();

    // Generates erase events from m_clearRegion
    void SendEraseEvents();

    // Clip to paint region?
    bool GetClipPaintRegion() { return m_clipPaintRegion; }

    // Return clear region
    wxRegion &GetClearRegion() { return m_clearRegion; }
protected:
    wxString m_js_id;
    const char * m_jid;
    wxString m_js_cid;
    const char * m_cid;

    wxRegion              m_clearRegion;
    bool                  m_clipPaintRegion;
    bool                  m_updateNcArea;
private:
    // common part of all ctors
    void Init();

    wxDECLARE_DYNAMIC_CLASS(wxWindowWASM);
    wxDECLARE_NO_COPY_CLASS(wxWindowWASM);
    wxDECLARE_EVENT_TABLE();
};

#endif //_WX_WINDOW_H_
