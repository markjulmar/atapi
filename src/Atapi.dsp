# Microsoft Developer Studio Project File - Name="ATapi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ATapi - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Atapi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Atapi.mak" CFG="ATapi - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ATapi - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ATapi - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ATapi - Win32 DLL Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ATapi - Win32 DLL Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ATapi - Win32 Release Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "ATapi - Win32 Debug Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ATAPI Library/SRC", WTAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ATapi - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W4 /WX /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\LIB\Atapi.lib"

!ELSEIF  "$(CFG)" == "ATapi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W4 /WX /GR /GX /Z7 /Od /Gf /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\LIB\atapid.lib"

!ELSEIF  "$(CFG)" == "ATapi - Win32 DLL Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\ATapi___"
# PROP BASE Intermediate_Dir ".\ATapi___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\DebugD"
# PROP Intermediate_Dir ".\DebugD"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /WX /GX /Zi /Od /Gf /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX"stdafx.h" /c
# ADD CPP /nologo /MDd /W4 /WX /GX /Od /Gf /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\LIB\atapid.lib"
# ADD LIB32 /nologo /out:"..\LIB\datapid.lib"

!ELSEIF  "$(CFG)" == "ATapi - Win32 DLL Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\ATapi__0"
# PROP BASE Intermediate_Dir ".\ATapi__0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\ReleaseD"
# PROP Intermediate_Dir ".\ReleaseD"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /c
# ADD CPP /nologo /MD /W4 /WX /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\LIB\dAtapi.lib"

!ELSEIF  "$(CFG)" == "ATapi - Win32 Release Unicode"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ATapi___"
# PROP BASE Intermediate_Dir "ATapi___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\RUnicode"
# PROP Intermediate_Dir ".\RUnicode"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /WX /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W4 /WX /GX /O2 /I "..\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNICODE" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\LIB\Atapi.lib"
# ADD LIB32 /nologo /out:"..\LIB\AtapiU.lib"

!ELSEIF  "$(CFG)" == "ATapi - Win32 Debug Unicode"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ATapi__0"
# PROP BASE Intermediate_Dir "ATapi__0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\DUnicode"
# PROP Intermediate_Dir ".\DUnicode"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /WX /GX /Zi /Od /Gf /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W4 /WX /GX /ZI /Od /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\LIB\atapid.lib"
# ADD LIB32 /nologo /out:"..\LIB\atapidU.lib"

!ENDIF 

# Begin Target

# Name "ATapi - Win32 Release"
# Name "ATapi - Win32 Debug"
# Name "ATapi - Win32 DLL Debug"
# Name "ATapi - Win32 DLL Release"
# Name "ATapi - Win32 Release Unicode"
# Name "ATapi - Win32 Debug Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Tapiaddr.cpp
# End Source File
# Begin Source File

SOURCE=.\Tapicall.cpp
# End Source File
# Begin Source File

SOURCE=.\Tapiconn.cpp
# End Source File
# Begin Source File

SOURCE=.\Tapiline.cpp
# End Source File
# Begin Source File

SOURCE=.\tapiobj.cpp
# End Source File
# Begin Source File

SOURCE=.\Tapiphone.cpp
# End Source File
# Begin Source File

SOURCE=.\tapireq.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\INCLUDE\Atapi.h
# End Source File
# Begin Source File

SOURCE=.\Stdafx.h
# End Source File
# Begin Source File

SOURCE="C:\Program Files\DevStudio\VC\include\TAPI.H"
# End Source File
# Begin Source File

SOURCE=.\Tapistr.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
