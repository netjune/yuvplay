# Microsoft Developer Studio Project File - Name="yuvplay" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=yuvplay - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "yuvplay.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "yuvplay.mak" CFG="yuvplay - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "yuvplay - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "yuvplay - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "yuvplay - Win32 Release"

# PROP BASE Use_MFC 6
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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Vfw32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib"

!ELSEIF  "$(CFG)" == "yuvplay - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Vfw32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "yuvplay - Win32 Release"
# Name "yuvplay - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CmpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\common.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAString.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFilter_Y8.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFilterGain.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFilters.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFormater.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPixel.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSimpleCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTranscode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgValues2.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgValues2Set.cpp
# End Source File
# Begin Source File

SOURCE=.\FileSource.cpp
# End Source File
# Begin Source File

SOURCE=.\Filter_mirror.cpp
# End Source File
# Begin Source File

SOURCE=.\Filter_test.cpp
# End Source File
# Begin Source File

SOURCE=.\Filter_upset.cpp
# End Source File
# Begin Source File

SOURCE=.\FilterMan.cpp
# End Source File
# Begin Source File

SOURCE=.\Formater_420SP.cpp
# End Source File
# Begin Source File

SOURCE=.\Formater_422SP.cpp
# End Source File
# Begin Source File

SOURCE=.\Formater_BGR24.cpp
# End Source File
# Begin Source File

SOURCE=.\Formater_I420.cpp
# End Source File
# Begin Source File

SOURCE=.\Formater_I422.cpp
# End Source File
# Begin Source File

SOURCE=.\Formater_RGB24.cpp
# End Source File
# Begin Source File

SOURCE=.\Formater_UYVY.cpp
# End Source File
# Begin Source File

SOURCE=.\Formater_Y16.cpp
# End Source File
# Begin Source File

SOURCE=.\Formater_Y8.cpp
# End Source File
# Begin Source File

SOURCE=.\Formater_YUV444.cpp
# End Source File
# Begin Source File

SOURCE=.\Formater_YUYV.cpp
# End Source File
# Begin Source File

SOURCE=.\Formater_YV12.cpp
# End Source File
# Begin Source File

SOURCE=.\Formater_YV16.cpp
# End Source File
# Begin Source File

SOURCE=.\FormaterMan.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Inner_modules.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuList.cpp
# End Source File
# Begin Source File

SOURCE=.\mFile.cpp
# End Source File
# Begin Source File

SOURCE=.\PosWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Render.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ThreadTranscode.cpp
# End Source File
# Begin Source File

SOURCE=.\ValuesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\yuvDoc.cpp

!IF  "$(CFG)" == "yuvplay - Win32 Release"

# ADD CPP /O2

!ELSEIF  "$(CFG)" == "yuvplay - Win32 Debug"

# ADD CPP /ZI /Od

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\yuvplay.cpp
# End Source File
# Begin Source File

SOURCE=.\yuvplay.rc
# End Source File
# Begin Source File

SOURCE=.\yuvplayDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CmpDlg.h
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\DlgAString.h
# End Source File
# Begin Source File

SOURCE=.\DlgFilter.h
# End Source File
# Begin Source File

SOURCE=.\DlgFilter_Y8.h
# End Source File
# Begin Source File

SOURCE=.\DlgFilterGain.h
# End Source File
# Begin Source File

SOURCE=.\DlgFilters.h
# End Source File
# Begin Source File

SOURCE=.\DLGFormater.h
# End Source File
# Begin Source File

SOURCE=.\DlgMsg.h
# End Source File
# Begin Source File

SOURCE=.\DlgPixel.h
# End Source File
# Begin Source File

SOURCE=.\DLGProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgSimpleCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgTranscode.h
# End Source File
# Begin Source File

SOURCE=.\DlgValues2.h
# End Source File
# Begin Source File

SOURCE=.\DlgValues2Set.h
# End Source File
# Begin Source File

SOURCE=.\FileSource.h
# End Source File
# Begin Source File

SOURCE=.\Filter_mirror.h
# End Source File
# Begin Source File

SOURCE=.\Filter_test.h
# End Source File
# Begin Source File

SOURCE=.\Filter_upset.h
# End Source File
# Begin Source File

SOURCE=.\FilterMan.h
# End Source File
# Begin Source File

SOURCE=.\Formater_420SP.h
# End Source File
# Begin Source File

SOURCE=.\Formater_422SP.h
# End Source File
# Begin Source File

SOURCE=.\Formater_BGR24.h
# End Source File
# Begin Source File

SOURCE=.\Formater_I420.h
# End Source File
# Begin Source File

SOURCE=.\Formater_I422.h
# End Source File
# Begin Source File

SOURCE=.\Formater_RGB24.h
# End Source File
# Begin Source File

SOURCE=.\Formater_UYVY.h
# End Source File
# Begin Source File

SOURCE=.\Formater_Y16.h
# End Source File
# Begin Source File

SOURCE=.\Formater_Y8.h
# End Source File
# Begin Source File

SOURCE=.\Formater_YUV444.h
# End Source File
# Begin Source File

SOURCE=.\Formater_YUYV.h
# End Source File
# Begin Source File

SOURCE=.\Formater_YV12.h
# End Source File
# Begin Source File

SOURCE=.\Formater_YV16.h
# End Source File
# Begin Source File

SOURCE=.\FormaterMan.h
# End Source File
# Begin Source File

SOURCE=.\ImageWnd.h
# End Source File
# Begin Source File

SOURCE=.\Inner_modules.h
# End Source File
# Begin Source File

SOURCE=.\MenuList.h
# End Source File
# Begin Source File

SOURCE=.\Message.h
# End Source File
# Begin Source File

SOURCE=.\mFile.h
# End Source File
# Begin Source File

SOURCE=.\ModuleList.h
# End Source File
# Begin Source File

SOURCE=.\Plugin.h
# End Source File
# Begin Source File

SOURCE=.\PosWnd.h
# End Source File
# Begin Source File

SOURCE=.\Render.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ThreadTranscode.h
# End Source File
# Begin Source File

SOURCE=.\ValuesDlg.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# Begin Source File

SOURCE=.\yuvDoc.h
# End Source File
# Begin Source File

SOURCE=.\yuvplay.h
# End Source File
# Begin Source File

SOURCE=.\yuvplayDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\default1.bin
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\yuvplay.ico
# End Source File
# Begin Source File

SOURCE=.\res\yuvplay.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\manifest.xml
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\yuvplay.exe.manifest
# End Source File
# End Target
# End Project
