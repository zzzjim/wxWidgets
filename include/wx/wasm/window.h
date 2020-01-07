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

    // SetLabel(), which does nothing in wxWindow
    virtual void SetLabel(const wxString& label) wxOVERRIDE { m_Label = label; }
    virtual wxString GetLabel() const wxOVERRIDE            { return m_Label; }

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

    // accessors
    // ---------

    // Get main WASM window
    //virtual WXWindow WASMGetMainWindow() const;

    // Get WASM window representing the client area
    //virtual WXWindow GetClientAreaWindow() const;

    void SetLastClick(int button, long timestamp)
        { m_lastButton = button; m_lastTS = timestamp; }

    int GetLastClickedButton() const { return m_lastButton; }
    long GetLastClickTime() const { return m_lastTS; }

    // Gives window a chance to do something in response to a size message, e.g.
    // arrange status bar, toolbar etc.
    virtual bool PreResize();

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

    void NeedUpdateNcAreaInIdle( bool update = true ) { m_updateNcArea = update; }

    // Inserting into main window instead of client
    // window. This is mostly for a wxWindow's own
    // scrollbars.
    void SetInsertIntoMain( bool insert = true ) { m_insertIntoMain = insert; }
    bool GetInsertIntoMain() { return m_insertIntoMain; }

    // sets the fore/background colour for the given widget
    static void DoChangeForegroundColour(WXWindow widget, wxColour& foregroundColour);
    static void DoChangeBackgroundColour(WXWindow widget, wxColour& backgroundColour, bool changeArmColour = false);

    virtual void OnInternalIdle();

protected:
    // Responds to colour changes: passes event on to children.
    void OnSysColourChanged(wxSysColourChangedEvent& event);

    // For double-click detection
    long   m_lastTS;         // last timestamp
    int    m_lastButton;     // last pressed button

protected:
    WXWindow              m_mainWindow;
    WXWindow              m_clientWindow;
    bool                  m_insertIntoMain;

    bool                  m_winCaptured;
    wxRegion              m_clearRegion;
    bool                  m_clipPaintRegion;
    bool                  m_updateNcArea;
    bool                  m_needsInputFocus; // Input focus set in OnIdle

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
    virtual void DoSetSizeHints(int minW, int minH,
        int maxW, int maxH,
        int incW, int incH);
    virtual void DoCaptureMouse();
    virtual void DoReleaseMouse();
    virtual void KillFocus();

#if wxUSE_TOOLTIPS
    virtual void DoSetToolTip( wxToolTip *tip );
#endif // wxUSE_TOOLTIPS

private:
    // common part of all ctors
    void Init();

    wxString m_Label;

    wxDECLARE_DYNAMIC_CLASS(wxWindowWASM);
    wxDECLARE_NO_COPY_CLASS(wxWindowWASM);
    wxDECLARE_EVENT_TABLE();
};

#endif //_WX_WINDOW_H_
