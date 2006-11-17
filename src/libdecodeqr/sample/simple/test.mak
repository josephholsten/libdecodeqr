# Microsoft Developer Studio Generated NMAKE File, Based on test.dsp
!IF "$(CFG)" == ""
CFG=test - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの test - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "test - Win32 Release" && "$(CFG)" != "test - Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "test.mak" CFG="test - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "test - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "test - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "test - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\test.exe"

!ELSE 

ALL : "libdecodeqr - Win32 Release" "$(OUTDIR)\test.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"libdecodeqr - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\simpletest.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\test.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\test.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\test.pdb" /machine:I386 /out:"$(OUTDIR)\test.exe" 
LINK32_OBJS= \
	"$(INTDIR)\simpletest.obj" \
	"..\..\Release\libdecodeqr.lib"

"$(OUTDIR)\test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\test.exe"

!ELSE 

ALL : "libdecodeqr - Win32 Debug" "$(OUTDIR)\test.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"libdecodeqr - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\simpletest.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\test.exe"
	-@erase "$(OUTDIR)\test.ilk"
	-@erase "$(OUTDIR)\test.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\test.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\test.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cv.lib cxcore.lib highgui.lib ws2_32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\test.pdb" /debug /machine:I386 /out:"$(OUTDIR)\test.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\simpletest.obj" \
	"..\..\Debug\libdecodeqr.lib"

"$(OUTDIR)\test.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("test.dep")
!INCLUDE "test.dep"
!ELSE 
!MESSAGE Warning: cannot find "test.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "test - Win32 Release" || "$(CFG)" == "test - Win32 Debug"
SOURCE=.\simpletest.cpp

"$(INTDIR)\simpletest.obj" : $(SOURCE) "$(INTDIR)"


!IF  "$(CFG)" == "test - Win32 Release"

"libdecodeqr - Win32 Release" : 
   cd "\Documents and Settings\zophos\My Documents\project\06-001h-cona001\src\libdecodeqr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libdecodeqr.mak" CFG="libdecodeqr - Win32 Release" 
   cd ".\sample\simple"

"libdecodeqr - Win32 ReleaseCLEAN" : 
   cd "\Documents and Settings\zophos\My Documents\project\06-001h-cona001\src\libdecodeqr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libdecodeqr.mak" CFG="libdecodeqr - Win32 Release" RECURSE=1 CLEAN 
   cd ".\sample\simple"

!ELSEIF  "$(CFG)" == "test - Win32 Debug"

"libdecodeqr - Win32 Debug" : 
   cd "\Documents and Settings\zophos\My Documents\project\06-001h-cona001\src\libdecodeqr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libdecodeqr.mak" CFG="libdecodeqr - Win32 Debug" 
   cd ".\sample\simple"

"libdecodeqr - Win32 DebugCLEAN" : 
   cd "\Documents and Settings\zophos\My Documents\project\06-001h-cona001\src\libdecodeqr"
   $(MAKE) /$(MAKEFLAGS) /F ".\libdecodeqr.mak" CFG="libdecodeqr - Win32 Debug" RECURSE=1 CLEAN 
   cd ".\sample\simple"

!ENDIF 


!ENDIF 

