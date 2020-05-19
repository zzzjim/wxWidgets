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
    wxTopLevelWindowWASM();
    wxTopLevelWindowWASM(wxWindow *parent,
               wxWindowID winid,
               const wxString& title,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxDEFAULT_FRAME_STYLE,
               const wxString& name = wxFrameNameStr);

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                const wxString& name = wxFrameNameStr);

    virtual void DoSetClientSize(int width, int height);
    virtual void DoMoveWindow(int x, int y, int width, int height);

    virtual void Maximize(bool maximize = true) wxOVERRIDE;
    virtual void Restore() wxOVERRIDE;
    virtual void Iconize(bool iconize = true) wxOVERRIDE;
    virtual bool IsMaximized() const wxOVERRIDE;
    virtual bool IsIconized() const wxOVERRIDE;

    virtual bool ShowFullScreen( bool show, long style = wxFULLSCREEN_ALL ) wxOVERRIDE;
    virtual bool IsFullScreen() const wxOVERRIDE;
    virtual void SetTitle(const wxString& title) wxOVERRIDE;
    virtual wxString GetTitle() const wxOVERRIDE;

    virtual bool Show( bool show = true ) wxOVERRIDE;
private:
    void Init();
    wxString m_title;
};

#endif // _WX_WASM_TOPLEVEL_H_
