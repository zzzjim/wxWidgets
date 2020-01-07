/////////////////////////////////////////////////////////////////////////////
// Name:        wx/wasm/chkconf.h
// Purpose:     Compiler-specific configuration checking
// Author:      James Goruk
// Modified by:
// Created:     2019-12-09
// Copyright:   (c) James Goruk
// Licence:     wxWindows licence

/* THIS IS A C FILE, DON'T USE C++ FEATURES (IN PARTICULAR COMMENTS) IN IT */

#undef wxUNIV_DEFAULT_THEME
#define wxUNIV_DEFAULT_THEME win32

#ifndef _WX_WASM_CHKCONF_H_
#define _WX_WASM_CHKCONF_H_

#undef wxHAS_TEXT_WINDOW_STREAM

#undef __WXUNIVERSAL__
#define __WXUNIVERSAL__ 1

#undef wxUSE_PROPGRID
#define wxUSE_PROPGRID 0

#undef wxUSE_TOOLTIPS
#define wxUSE_TOOLTIPS 0

//#undef wxUSE_BOOKCTRL
//#define wxUSE_BOOKCTRL 0

#undef wxUSE_DIRDLG
#define wxUSE_DIRDLG 0

#undef wxUSE_FILEDLG
#define wxUSE_FILEDLG 0

#undef wxUSE_FILEPICKERCTRL
#define wxUSE_FILEPICKERCTRL 0

#undef wxUSE_DIRPICKERCTRL
#define wxUSE_DIRPICKERCTRL 0

#endif /* _WX_WASM_CHKCONF_H_ */
