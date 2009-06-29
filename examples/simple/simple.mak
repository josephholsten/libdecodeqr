# Microsoft Developer Studio Generated NMAKE File, Based on simple.dsp
!IF "$(CFG)" == ""
CFG=simple - Win32 Debug
!MESSAGE Missing target. Using defualt "simple - Win32 Debug".
!ENDIF 

!IF "$(CFG)" != "simple - Win32 Release" && "$(CFG)" != "simple - Win32 Debug"
!MESSAGE Mode "$(CFG)" is not valid.
!MESSAGE 
!MESSAGE Valid modes are:
!MESSAGE 
!MESSAGE "simple - Win32 Release" (for "Win32 (x86) Static Library")
!MESSAGE "simple - Win32 Debug" (for "Win32 (x86) Static Library")
!MESSAGE 
!MESSAGE eg:
!MESSAGE 
!MESSAGE NMAKE /f "simple.mak" CFG="webcam - Win32 Debug"
!ERROR Invalid target are gaven.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "simple - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\simple.exe"


CLEAN :
	-@erase "$(INTDIR)\simpletest.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\simple.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\simple.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\simple.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\simple.pdb" /machine:I386 /out:"$(OUTDIR)\simple.exe" 
LINK32_OBJS= \
	"$(INTDIR)\simpletest.obj" \
	"..\..\Debug\libdecodeqr.lib"

"$(OUTDIR)\simple.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "simple - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\simple.exe"


CLEAN :
	-@erase "$(INTDIR)\simpletest.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\simple.exe"
	-@erase "$(OUTDIR)\simple.ilk"
	-@erase "$(OUTDIR)\simple.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\simple.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\simple.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib cv.lib cxcore.lib highgui.lib ws2_32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\simple.pdb" /debug /machine:I386 /out:"$(OUTDIR)\simple.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\simpletest.obj" \
	"..\..\Debug\libdecodeqr.lib"

"$(OUTDIR)\simple.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("simple.dep")
!INCLUDE "simple.dep"
!ELSE 
!MESSAGE Warning: cannot find "simple.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "simple - Win32 Release" || "$(CFG)" == "simple - Win32 Debug"
SOURCE=.\simpletest.cpp

"$(INTDIR)\simpletest.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

