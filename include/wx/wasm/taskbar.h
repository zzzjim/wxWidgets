/////////////////////////////////////////////////////////////////////////////
// Name:        wx/wasm/taskbar.h
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WASM_TASKBAR_H_
#define _WX_WASM_TASKBAR_H_

class WXDLLIMPEXP_ADV wxTaskBarIcon : public wxTaskBarIconBase
{
public:
    wxTaskBarIcon(wxTaskBarIconType iconType = wxTBI_DEFAULT_TYPE);
    virtual ~wxTaskBarIcon();

    // Accessors
    bool IsOk() const { return false; }
    bool IsIconInstalled() const { return false; }

    // Operations
    virtual bool SetIcon(const wxIcon& icon,
                         const wxString& tooltip = wxEmptyString) wxOVERRIDE;
    virtual bool RemoveIcon() wxOVERRIDE;
    virtual bool PopupMenu(wxMenu *menu) wxOVERRIDE;

private:
    wxDECLARE_DYNAMIC_CLASS_NO_COPY(wxTaskBarIcon);
};

#endif // _WX_WASM_TASKBAR_H_
