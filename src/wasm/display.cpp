/////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/display.cpp
// Purpose:     wxDisplay class
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/private/display.h"
#include "wx/dynlib.h"

#include <emscripten.h>

// This implementation is always available, whether wxUSE_DISPLAY is 1 or not,
// as we fall back to it in case of error.
class wxDisplayImplSingleWASM : public wxDisplayImplSingle
{
public:
    virtual wxRect GetGeometry() const wxOVERRIDE
    {
        int32_t width, height;
        width = MAIN_THREAD_EM_ASM_INT({
            return window.screen.width;
        });
        height = MAIN_THREAD_EM_ASM_INT({
            return window.screen.height;
        });

        printf("wxDisplayImplSingleWASM::GetGeometry: %dx%d\n", width, height);
        return wxRect(0, 0, width, height);
    }

    virtual wxRect GetClientArea() const wxOVERRIDE
    {
        printf("wxDisplayImplSingleWASM::GetClientArea\n");
        int32_t width, height;
        width = MAIN_THREAD_EM_ASM_INT({
            return window.screen.availWidth;
        });
        height = MAIN_THREAD_EM_ASM_INT({
            return window.screen.availHeight;
        });

        printf("wxDisplayImplSingleWASM::GetClientArea: %dx%d\n", width, height);

        return wxRect(0, 0, width, height);
    }

    virtual int GetDepth() const wxOVERRIDE
    {
        int32_t depth;
        depth = MAIN_THREAD_EM_ASM_INT({
            return window.screen.pixelDepth;
        });
        printf("wxDisplayImplSingleWASM::GetDepth depth=%d\n", depth);
        return depth;
    }

    virtual wxSize GetSizeMM() const wxOVERRIDE
    {
        double devicePixelRatio = 1;
	/*
	double devicePixelRatio = MAIN_THREAD_EM_ASM_INT({
            console.log("Window: ", window);
            return window.devicePixelRatio;
        });
	*/
        printf("wxDisplayImplSingleWASM::GetSizeMM This isn't right need a better way\n");
        printf("wxDisplayImplSingleWASM::GetSizeMM window.devicePixelRatio: %f\n", devicePixelRatio);
        wxRect geo = GetGeometry();
	int width = geo.width * devicePixelRatio * (25.4/ 96);
	int height = geo.height * devicePixelRatio * (25.4/ 96);
        printf("wxDisplayImplSingleWASM::GetSizeMM: %dx%d\n", width, height);
        return wxSize(width, height);
    }
};

class wxDisplayFactorySingleWASM : public wxDisplayFactorySingle
{
protected:
    virtual wxDisplayImpl *CreateSingleDisplay() wxOVERRIDE
    {
        return new wxDisplayImplSingleWASM;
    }
};

wxDisplayFactory* wxDisplay::CreateFactory()
{
    return new wxDisplayFactorySingleWASM;
}
