# Microsoft Developer Studio Generated NMAKE File, Based on libdecodeqr.dsp
!IF "$(CFG)" == ""
CFG=libdecodeqr - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの libdecodeqr - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "libdecodeqr - Win32 Release" && "$(CFG)" != "libdecodeqr - Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "libdecodeqr.mak" CFG="libdecodeqr - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "libdecodeqr - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "libdecodeqr - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "libdecodeqr - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\libdecodeqr.lib"


CLEAN :
	-@erase "$(INTDIR)\bitstream.obj"
	-@erase "$(INTDIR)\codedata.obj"
	-@erase "$(INTDIR)\container.obj"
	-@erase "$(INTDIR)\ecidecoder.obj"
	-@erase "$(INTDIR)\formatinfo.obj"
	-@erase "$(INTDIR)\galois.obj"
	-@erase "$(INTDIR)\imagereader.obj"
	-@erase "$(INTDIR)\libdecodeqr.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\libdecodeqr.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libdecodeqr.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libdecodeqr.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\libdecodeqr.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bitstream.obj" \
	"$(INTDIR)\codedata.obj" \
	"$(INTDIR)\container.obj" \
	"$(INTDIR)\ecidecoder.obj" \
	"$(INTDIR)\formatinfo.obj" \
	"$(INTDIR)\galois.obj" \
	"$(INTDIR)\imagereader.obj" \
	"$(INTDIR)\libdecodeqr.obj"

"$(OUTDIR)\libdecodeqr.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "libdecodeqr - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\libdecodeqr.lib"


CLEAN :
	-@erase "$(INTDIR)\bitstream.obj"
	-@erase "$(INTDIR)\codedata.obj"
	-@erase "$(INTDIR)\container.obj"
	-@erase "$(INTDIR)\ecidecoder.obj"
	-@erase "$(INTDIR)\formatinfo.obj"
	-@erase "$(INTDIR)\galois.obj"
	-@erase "$(INTDIR)\imagereader.obj"
	-@erase "$(INTDIR)\libdecodeqr.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\libdecodeqr.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\libdecodeqr.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libdecodeqr.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\libdecodeqr.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bitstream.obj" \
	"$(INTDIR)\codedata.obj" \
	"$(INTDIR)\container.obj" \
	"$(INTDIR)\ecidecoder.obj" \
	"$(INTDIR)\formatinfo.obj" \
	"$(INTDIR)\galois.obj" \
	"$(INTDIR)\imagereader.obj" \
	"$(INTDIR)\libdecodeqr.obj"

"$(OUTDIR)\libdecodeqr.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("libdecodeqr.dep")
!INCLUDE "libdecodeqr.dep"
!ELSE 
!MESSAGE Warning: cannot find "libdecodeqr.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "libdecodeqr - Win32 Release" || "$(CFG)" == "libdecodeqr - Win32 Debug"
SOURCE=.\bitstream.cpp

"$(INTDIR)\bitstream.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\codedata.cpp

"$(INTDIR)\codedata.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\container.cpp

"$(INTDIR)\container.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ecidecoder.cpp

"$(INTDIR)\ecidecoder.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\formatinfo.cpp

"$(INTDIR)\formatinfo.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\galois.cpp

"$(INTDIR)\galois.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\imagereader.cpp

"$(INTDIR)\imagereader.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\libdecodeqr.cpp

"$(INTDIR)\libdecodeqr.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

