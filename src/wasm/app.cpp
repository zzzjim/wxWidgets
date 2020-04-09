/////////////////////////////////////////////////////////////////////////////
// Name:        src/wasm/app.cpp
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#include "wx/app.h"
#include "wx/apptrait.h"
#include "wx/debug.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

wxIMPLEMENT_DYNAMIC_CLASS(wxApp, wxEvtHandler);

void WASMAssertHandler(const wxString& file,
                                  int line,
                                  const wxString& func,
                                  const wxString& cond,
                                  const wxString& msg)
{
	const char * file_s = file.mb_str();
	const char * f = func.mb_str();
	const char * c = cond.mb_str();
	const char * m = msg.mb_str();
	printf("WASMAPPASSERT: %s:%d %s %s %s\n", file_s, line, f, c, m);
#ifdef __EMSCRIPTEN__
	EM_ASM(
		console.log(stackTrace());
	);
#endif

}


wxApp::wxApp()
{
}


wxApp::~wxApp()
{
}

bool wxApp::Initialize( int &argc, wxChar **argv )
{
    wxSetAssertHandler(WASMAssertHandler);

    if ( !wxAppBase::Initialize( argc, argv ))
        return false;

    return true;
}

void wxApp::CleanUp()
{
    wxAppBase::CleanUp();
}

