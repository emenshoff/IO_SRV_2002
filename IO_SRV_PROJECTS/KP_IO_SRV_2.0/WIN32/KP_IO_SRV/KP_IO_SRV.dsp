# Microsoft Developer Studio Project File - Name="KP_IO_SRV" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=KP_IO_SRV - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KP_IO_SRV.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KP_IO_SRV.mak" CFG="KP_IO_SRV - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KP_IO_SRV - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "KP_IO_SRV - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "KP_IO_SRV - Win32 Release"

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
# ADD CPP /nologo /G5 /W3 /GX /O2 /I "../../sources/include" /I "../../../IO_SRV_BASE/sources/xdr" /I "../../../IO_SRV_BASE/sources/include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "KP_IO_SRV - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /Gm /GX /ZI /Od /I "../../sources/include" /I "../../../IO_SRV_BASE/sources/xdr" /I "../../../IO_SRV_BASE/sources/include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
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

# Name "KP_IO_SRV - Win32 Release"
# Name "KP_IO_SRV - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\SOURCES\SRC\cond_parser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\crypt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\data_block.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\io_srv.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\io_srv_drv_types.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\io_srv_dump.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\io_srv_event_processor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\io_srv_event_supervisor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_abstract_dev.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_afb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_cdv.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_channel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_config.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_control.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_dev.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_events_cash.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_fb_light.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_fb_tii.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_fb_tit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_fb_ts.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_fb_tu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_io_srv.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_light.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_tii.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_tit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_titi.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_ts.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_tu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\kp_types.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\main.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\srv_sock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\srv_tracer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\stm_cln.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\stm_srv.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\stm_srv_cfg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\stm_srv_cfg_control.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\win2koi_koi2win.c
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\XDR\XDR.C
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\XDR\XDR_ARRA.C
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\XDR\XDR_FLOA.C
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\XDR\XDR_MEM.C
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\XDR\XDR_REC.C
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\XDR\XDR_REFE.C
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\XDR\XDR_STDI.C
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\xdr_utils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\bin_data.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\cond_parser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\data_block.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\def_crit_sec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\io_srv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\io_srv_control_object.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\io_srv_drv_module.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\io_srv_drv_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\io_srv_dump.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\io_srv_event.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\io_srv_event_processor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\io_srv_event_supervisor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\io_srv_module.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\io_srv_modules.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_abstract_dev.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_afb.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_cdv.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_channel.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_config.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_control.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_dev.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_events_cash.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_fb_light.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_fb_tii.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_fb_tit.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_fb_ts.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_fb_tu.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_functional.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_header.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_io_srv.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_light.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_packets.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_tii.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_tit.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_titi.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_ts.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_tu.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\kp_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\srv_sock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\srv_tracer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\stm_cln.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\stm_packets.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\stm_srv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\stm_srv_cfg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\stm_srv_cfg_control.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\stm_srv_const.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\stm_types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\time_utils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\types_convert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\val.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\win2koi_koi2win.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\XDR\Xdr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\xdr_utils.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE="..\..\..\..\..\..\Program Files\Microsoft Visual Studio\VC98\Lib\WSOCK32.LIB"
# End Source File
# End Target
# End Project
