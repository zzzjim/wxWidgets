/////////////////////////////////////////////////////////////////////////////
// Name:        wx/wasm/dcmemory.h
// Purpose:     wxMemoryDC class
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCMEMORY_H_
#define _WX_DCMEMORY_H_

#include "wx/dc.h"
#include "wx/dcmemory.h"
#include "wx/wasm/dcclient.h"

class WXDLLIMPEXP_CORE wxMemoryDCImpl : public wxWindowDCImpl
{
public:
    wxMemoryDCImpl( wxMemoryDC *owner );
    wxMemoryDCImpl( wxMemoryDC *owner, wxBitmap& bitmap);
    wxMemoryDCImpl( wxMemoryDC *owner, wxDC *dc );
    virtual ~wxMemoryDCImpl();

    virtual const wxBitmap& GetSelectedBitmap() const;
    virtual wxBitmap& GetSelectedBitmap();

    // implementation
    wxBitmap  m_selected;

protected:
    virtual void DoGetSize( int *width, int *height ) const;
    virtual void DoSelect(const wxBitmap& bitmap);

private:
    void Init();

private:
    wxBitmap m_bmp;
    wxDECLARE_CLASS(wxMemoryDCImpl);
};

#endif
// _WX_DCMEMORY_H_
