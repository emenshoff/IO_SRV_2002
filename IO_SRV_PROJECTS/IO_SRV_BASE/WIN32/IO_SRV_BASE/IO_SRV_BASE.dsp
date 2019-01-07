# Microsoft Developer Studio Project File - Name="IO_SRV_BASE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=IO_SRV_BASE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IO_SRV_BASE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IO_SRV_BASE.mak" CFG="IO_SRV_BASE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IO_SRV_BASE - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "IO_SRV_BASE - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IO_SRV_BASE - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "IO_SRV_BASE - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /w /W0 /Gm /GX /ZI /Od /I "../../sources/include/" /I "../../sources/xdr/" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "IO_SRV_BASE - Win32 Release"
# Name "IO_SRV_BASE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\SOURCES\SRC\crypt.c
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\io_srv.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\io_srv_dump.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\io_srv_event_supervisor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\srv_sock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\srv_tracer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\stm_cln.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\stm_srv.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\stm_srv_cfg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\stm_srv_cfg_control.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\win2koi_koi2win.c
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\xdr_utils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\io_srv.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\io_srv_drv_module.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\io_srv_dump.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\io_srv_event.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\io_srv_event_processor.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\io_srv_event_supervisor.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\srv_sock.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\srv_tracer.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\stm_cln.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\stm_srv.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\stm_srv_cfg.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\stm_srv_cfg_control.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\stm_srv_const.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\stm_types.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\time_utils.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\val.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\win2koi_koi2win.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\XDR\Xdr.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\xdr_utils.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
