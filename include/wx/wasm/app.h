/////////////////////////////////////////////////////////////////////////////
// Name:        wx/wasm/app.h
// Purpose:     wxApp class
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WASM_APP_H_
#define _WX_WASM_APP_H_

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/gdicmn.h"
#include "wx/event.h"

// ----------------------------------------------------------------------------
// the wxApp class for wxWASM - see wxAppBase for more details
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxApp : public wxAppBase
{
public:
    wxApp();
    virtual ~wxApp();

    // Implementation
    virtual bool Initialize(int& argc, wxChar **argv);
    virtual void CleanUp();
    wxDECLARE_DYNAMIC_CLASS(wxApp);
};

#endif // _WX_WASM_APP_H_

