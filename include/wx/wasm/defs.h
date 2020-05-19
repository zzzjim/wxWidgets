/////////////////////////////////////////////////////////////////////////////
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WASM_DEFS_H_
#define _WX_WASM_DEFS_H_

#ifdef __cplusplus

typedef void *		WXWidget;
typedef void*           WXWindow;
typedef void*           WXWidget;
typedef void*           WXAppContext;
typedef void*           WXColormap;
typedef void*           WXColor;
typedef void            WXDisplay;
typedef void            WXEvent;
typedef void*           WXCursor;
typedef void*           WXPixmap;
typedef void*           WXFontStructPtr;
typedef void*           WXGC;
typedef void*           WXRegion;
typedef void*           WXFont;
typedef void*           WXImage;
typedef void*           WXFontList;
typedef void*           WXFontSet;
typedef void*           WXRendition;
typedef void*           WXRenderTable;
typedef void*           WXFontType; /* either a XmFontList or XmRenderTable */
typedef void*           WXString;

typedef unsigned long   Atom;  /* this might fail on a few architectures */
typedef long            WXPixel; /* safety catch in src/motif/colour.cpp */

#endif

#endif /* _WX_WASM_DEFS_H_ */
