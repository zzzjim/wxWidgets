/////////////////////////////////////////////////////////////////////////////
// Name:        wx/wasm/chkconf.h
// Purpose:     Compiler-specific configuration checking
// Author:      James Goruk
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence

#ifndef _WX_WASM_CHKCONF_H_
#define _WX_WASM_CHKCONF_H_

#undef wxUNIV_DEFAULT_THEME
//#define wxUNIV_DEFAULT_THEME win32

#undef wxUSE_ALL_THEMES
#define wxUSE_ALL_THEMES    1

#undef wxUSE_THEME_GTK
#define wxUSE_THEME_GTK     1

#undef wxUSE_THEME_METAL
#define wxUSE_THEME_METAL   1

#undef wxUSE_THEME_MONO
#define wxUSE_THEME_MONO    1

#undef wxUSE_THEME_WIN32
#define wxUSE_THEME_WIN32   1

#undef __WXUNIVERSAL__
#define __WXUNIVERSAL__ 1

#endif /* _WX_WASM_CHKCONF_H_ */
