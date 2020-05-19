/////////////////////////////////////////////////////////////////////////////
// Name:        wx/wasm/dcscreen.h
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WASM_DCSCREEN_H_
#define _WX_WASM_DCSCREEN_H_

#include "wx/wasm/dcclient.h"

class WXDLLIMPEXP_CORE wxScreenDCImpl : public wxWindowDCImpl
{
public:
    wxScreenDCImpl( wxScreenDC *owner );

    ~wxScreenDCImpl();

protected:
    virtual void DoGetSize(int *width, int *height) const wxOVERRIDE;

    wxDECLARE_ABSTRACT_CLASS(wxScreenDCImpl);
};

#endif // _WX_WASM_DCSCREEN_H_
