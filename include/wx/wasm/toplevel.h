///////////////////////////////////////////////////////////////////////////////
// Name:        wx/wasm/toplevel.h
// Purpose:     wxTopLevelWindowWASM
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WASM_TOPLEVEL_H_
#define _WX_WASM_TOPLEVEL_H_

// ----------------------------------------------------------------------------
// wxTopLevelWindowWASM
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxTopLevelWindowWASM : public wxTopLevelWindowBase
{
public:
    // constructors and such
    wxTopLevelWindowWASM() { Init(); }

    wxTopLevelWindowWASM(wxWindow *parent,
                        wxWindowID id,
                        const wxString& title,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        long style = wxDEFAULT_FRAME_STYLE,
                        const wxString& name = wxFrameNameStr)
    {
        Init();

        (void)Create(parent, id, title, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                const wxString& name = wxFrameNameStr);

    virtual ~wxTopLevelWindowWASM();

    // implement base class pure virtuals
    virtual void Maximize(bool maximize = true);
    virtual bool IsMaximized() const;
    virtual void Iconize(bool iconize = true);
    virtual bool IsIconized() const;
    virtual void SetIcons(const wxIconBundle& icons);
    virtual void Restore();

    virtual bool Show( bool show = true );

    virtual bool ShowFullScreen( bool show, long style = wxFULLSCREEN_ALL );
    virtual bool IsFullScreen() const { return m_fsIsShowing; }

    virtual void SetTitle( const wxString& title);
    virtual wxString GetTitle() const;

    // implementation
    void SetNeedResizeInIdle( bool set = true )
        { m_needResizeInIdle = set; }
    void SetConfigureGeometry( int x, int y, int width, int height )
        { m_x = x; m_y = y; m_width = width; m_height = height; }

    virtual bool SetShape(const wxRegion& region);

    // For implementation purposes - sometimes decorations make the
    // client area smaller
    virtual wxPoint GetClientAreaOrigin() const;

    virtual void OnInternalIdle();

protected:
    // common part of all ctors
    void Init();

    // set the icon for the window
    void DoSetIcon( const wxIcon& icon );

    // For implementation of delayed resize events
    bool m_needResizeInIdle;

    virtual void DoGetClientSize( int *width, int *height ) const;
    virtual void DoGetSize( int *width, int *height ) const;
    virtual void DoSetClientSize(int width, int height);
    virtual void DoSetSize(int x, int y,
        int width, int height,
        int sizeFlags = wxSIZE_AUTO);
    virtual void DoGetPosition( int *x, int *y ) const;

    // Is the frame currently iconized?
    bool m_iconized;

    // Should the frame be maximized when it will be shown? set by Maximize()
    // when it is called while the frame is hidden
    bool m_maximizeOnShow;

    // Data to save/restore when calling ShowFullScreen
    long                  m_fsStyle;   // Passed to ShowFullScreen
    wxRect                m_fsOldSize;
    bool                  m_fsIsMaximized;
    bool                  m_fsIsShowing;
    wxString              m_title;

    // Geometry
    int                   m_x,m_y,m_width,m_height;
};

// list of all frames and modeless dialogs
//extern WXDLLIMPEXP_DATA_CORE(wxWindowList) wxModelessWindows;

#endif // _WX_WASM_TOPLEVEL_H_
