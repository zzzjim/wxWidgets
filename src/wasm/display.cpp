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

// This implementation is always available, whether wxUSE_DISPLAY is 1 or not,
// as we fall back to it in case of error.
class wxDisplayImplSingleWASM : public wxDisplayImplSingle
{
public:
    virtual wxRect GetGeometry() const wxOVERRIDE
    {
        return wxRect(0, 0, 800, 600);
    }

    virtual wxRect GetClientArea() const wxOVERRIDE
    {
        return wxRect(0, 0, 800, 600);
    }

    virtual int GetDepth() const wxOVERRIDE
    {
        return 0;
    }

    virtual wxSize GetSizeMM() const wxOVERRIDE
    {
        return wxSize(800, 600);
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
