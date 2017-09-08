# Microsoft Developer Studio Project File - Name="Phone" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Phone - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Phone.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Phone.mak" CFG="Phone - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Phone - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Phone - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ATAPI Library/SAMPLE/Phone", HCBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Phone - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GR /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 tapi32.lib atapi.lib /nologo /subsystem:windows /map /machine:I386 /libpath:"..\..\lib"

!ELSEIF  "$(CFG)" == "Phone - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W4 /Gm /GR /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 tapi32.lib atapid.lib /nologo /subsystem:windows /map /machine:I386 /pdbtype:sept /libpath:"..\..\lib"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "Phone - Win32 Release"
# Name "Phone - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AgentStateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CallDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CompleteCall.cpp
# End Source File
# Begin Source File

SOURCE=.\CompleteTransferDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DialDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ForwardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\hexedit.cpp
# End Source File
# Begin Source File

SOURCE=.\Hlinkctl.cpp
# End Source File
# Begin Source File

SOURCE=.\ISDNInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Phone.rc
# End Source File
# Begin Source File

SOURCE=.\PhoneCapsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PhoneDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PredDialDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\QosInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SetupConfDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TransferDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UUIDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AgentStateDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\INCLUDE\Atapi.h
# End Source File
# Begin Source File

SOURCE=.\CallDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\CompleteCall.h
# End Source File
# Begin Source File

SOURCE=.\CompleteTransferDlg.h
# End Source File
# Begin Source File

SOURCE=.\ConfListDlg.h
# End Source File
# Begin Source File

SOURCE=.\DialDlg.h
# End Source File
# Begin Source File

SOURCE=.\ForwardDlg.h
# End Source File
# Begin Source File

SOURCE=.\hexedit.h
# End Source File
# Begin Source File

SOURCE=.\Hlinkctl.h
# End Source File
# Begin Source File

SOURCE=.\ISDNInfo.h
# End Source File
# Begin Source File

SOURCE=.\Phone.h
# End Source File
# Begin Source File

SOURCE=.\PhoneCapsDlg.h
# End Source File
# Begin Source File

SOURCE=.\PhoneDlg.h
# End Source File
# Begin Source File

SOURCE=.\PredDialDlg.h
# End Source File
# Begin Source File

SOURCE=.\QosInfo.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\resource2.h
# End Source File
# Begin Source File

SOURCE=.\SetupConfDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE="C:\Program Files\Microsoft Visual Studio\Vc98\Include\Tapi.h"
# End Source File
# Begin Source File

SOURCE=.\TransferDlg.h
# End Source File
# Begin Source File

SOURCE=.\UUIDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\Phone.ico
# End Source File
# Begin Source File

SOURCE=.\res\Phone.rc2
# End Source File
# End Group
# End Target
# End Project
