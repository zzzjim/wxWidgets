# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.1.20030719 (http://bakefile.sourceforge.net)
#     Do not modify, all changes will be overwritten!
# =========================================================================

!include ../../../build/config.wat

# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

# Speed up compilation a bit:
!ifdef __LOADDLL__
!  loaddll wcc      wccd
!  loaddll wccaxp   wccdaxp
!  loaddll wcc386   wccd386
!  loaddll wpp      wppdi86
!  loaddll wppaxp   wppdaxp
!  loaddll wpp386   wppd386
!  loaddll wlink    wlink
!  loaddll wlib     wlibd
!endif

# We need these variables in some bakefile-made rules:
WATCOM_CWD = $+ $(%cdrive):$(%cwd) $-

### Conditionally set variables: ###

PORTNAME =
!ifeq USE_GUI 0
PORTNAME = base
!endif
!ifeq USE_GUI 1
PORTNAME = msw
!endif
WXDEBUGFLAG =
!ifeq BUILD debug
WXDEBUGFLAG = d
!endif
WXDLLFLAG =
!ifeq SHARED 1
WXDLLFLAG = dll
!endif
WXUNICODEFLAG =
!ifeq UNICODE 1
WXUNICODEFLAG = u
!endif
WXUNIVNAME =
!ifeq WXUNIV 1
WXUNIVNAME = univ
!endif
__DEBUGFLAG_0 =
!ifeq BUILD debug
__DEBUGFLAG_0 = -d2
!endif
!ifeq BUILD release
__DEBUGFLAG_0 = -d0
!endif
__DEBUGFLAG_1 =
!ifeq BUILD debug
__DEBUGFLAG_1 = debug all
!endif
!ifeq BUILD release
__DEBUGFLAG_1 = 
!endif
__DEBUG_DEFINE_p =
!ifeq BUILD debug
__DEBUG_DEFINE_p = -d__WXDEBUG__
!endif
__DLLFLAG_p =
!ifeq SHARED 1
__DLLFLAG_p = -dWXUSINGDLL
!endif
__OPTIMIZEFLAG_2 =
!ifeq BUILD debug
__OPTIMIZEFLAG_2 = -od
!endif
!ifeq BUILD release
__OPTIMIZEFLAG_2 = -ot -ox
!endif
__RUNTIME_LIBS_5 =
!ifeq RUNTIME_LIBS dynamic
__RUNTIME_LIBS_5 = -br
!endif
!ifeq RUNTIME_LIBS static
__RUNTIME_LIBS_5 = 
!endif
__UNICODE_DEFINE_p =
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -dwxUSE_UNICODE=1
!endif
__WXLIB_BASE_p =
!ifeq MONOLITHIC 0
__WXLIB_BASE_p = wxbase25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__WXLIB_CORE_p =
!ifeq MONOLITHIC 0
__WXLIB_CORE_p = wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_core.lib
!endif
__WXLIB_HTML_p =
!ifeq MONOLITHIC 0
__WXLIB_HTML_p = wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_html.lib
!endif
__WXLIB_MONO_p =
!ifeq MONOLITHIC 1
__WXLIB_MONO_p = wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__WXUNIV_DEFINE_p =
!ifeq WXUNIV 1
__WXUNIV_DEFINE_p = -d__WXUNIVERSAL__
!endif

### Variables: ###

OBJS = wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
TEST_CXXFLAGS = $(CPPFLAGS)  $(__DEBUGFLAG_0) $(__OPTIMIZEFLAG_2) -bm $(__RUNTIME_LIBS_5)  -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\..\include -i=.\..\..\..\lib\wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG) -i=.\..\..\..\src\tiff -i=.\..\..\..\src\jpeg -i=.\..\..\..\src\png -i=.\..\..\..\src\zlib  -i=.\..\..\..\src\regex -i=. $(__DLLFLAG_p) $(CXXFLAGS)



all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(OBJS)\test.exe data

$(OBJS)\test_test.obj :  .AUTODEPEND .\test.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_test.res :  .AUTODEPEND .\test.rc
	wrc -q -ad -bt=nt -r -fo=$^@ -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\..\include -i=.\..\..\..\lib\wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG) -i=.\..\..\..\src\tiff -i=.\..\..\..\src\jpeg -i=.\..\..\..\src\png -i=.\..\..\..\src\zlib  -i=.\..\..\..\src\regex -i=. $(__DLLFLAG_p) $<

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\test.exe del $(OBJS)\test.exe

data :  
	if not exist $(OBJS) mkdir $(OBJS)
	for %f in (f.html fft.html imagemap.htm imagemap.png pic.png pic2.bmp tables.htm test.htm i18n.gif 8859_2.htm cp1250.htm regres.htm) do if not exist $(OBJS)\%f copy .\%f $(OBJS)

$(OBJS)\test.exe :  $(OBJS)\test_test.obj $(OBJS)\test_test.res
	@%create $(OBJS)\test.lbc
	@%append $(OBJS)\test.lbc option quiet
	@%append $(OBJS)\test.lbc name $^@
	@%append $(OBJS)\test.lbc option incremental
	@%append $(OBJS)\test.lbc $(LDFLAGS) $(__DEBUGFLAG_1)  libpath .\..\..\..\lib\wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)  system nt_win ref '_WinMain@16'
	@for %i in ( $(OBJS)\test_test.obj) do @%append $(OBJS)\test.lbc file %i
	@for %i in ( $(__WXLIB_HTML_p) $(__WXLIB_CORE_p) $(__WXLIB_BASE_p) $(__WXLIB_MONO_p) wxtiff$(WXDEBUGFLAG).lib wxjpeg$(WXDEBUGFLAG).lib wxpng$(WXDEBUGFLAG).lib wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXDEBUGFLAG).lib  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib ) do @%append $(OBJS)\test.lbc library %i
	@%append $(OBJS)\test.lbc option resource=$(OBJS)\test_test.res
	wlink @$(OBJS)\test.lbc
