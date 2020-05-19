/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/app.cpp
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#define wxUNIV_DEFAULT_THEME win32
#include "wx/wxprec.h"

#include "wx/app.h"
#include "wx/apptrait.h"
#include "wx/debug.h"
#include "wx/image.h"

#include <emscripten.h>

wxIMPLEMENT_DYNAMIC_CLASS(wxApp, wxEvtHandler);

void WASMAssertHandler(const wxString& file,
                                  int line,
                                  const wxString& func,
                                  const wxString& cond,
                                  const wxString& msg)
{
	const char * ff = file.mb_str();
	const char * f = func.mb_str();
	const char * c = cond.mb_str();
	const char * m = msg.mb_str();
	fprintf(stderr, "WASMAPPASSERT: %s:%d %s %s %s\n", ff, line, f, c, m);

        EM_ASM(
             console.log(stackTrace());
        );
}


wxApp::wxApp()
{
    //wxInitAllImageHandlers();
    WX_USE_THEME(gtk);
    WX_USE_THEME(win32);
    WX_USE_THEME(Metal);

    //wxTheme * theme = wxTheme::Create("win32");
    //wxTheme * theme = wxTheme::Create("gtk");
    wxTheme * theme = wxTheme::Create("metal");
    wxTheme::Set(theme);
}


wxApp::~wxApp()
{
}

bool wxApp::Initialize( int &argc, wxChar **argv )
{
    wxSetAssertHandler(WASMAssertHandler);

    printf("wxApp::Initialize!\n");
    MAIN_THREAD_EM_ASM_INT({
        window.wx = {};
        window.wx.bitmaps = new Map();
    });

    if ( !wxAppBase::Initialize( argc, argv ))
        return false;

    return true;
}

void wxApp::CleanUp()
{
    wxAppBase::CleanUp();
}

