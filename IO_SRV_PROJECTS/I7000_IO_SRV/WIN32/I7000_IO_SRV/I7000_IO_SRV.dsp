# Microsoft Developer Studio Project File - Name="I7000_IO_SRV" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=I7000_IO_SRV - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "I7000_IO_SRV.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "I7000_IO_SRV.mak" CFG="I7000_IO_SRV - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "I7000_IO_SRV - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "I7000_IO_SRV - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "I7000_IO_SRV - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /W3 /GX /O2 /I "../../SOURCES/INCLUDE" /I "../../../IO_SRV_BASE/SOURCES/INCLUDE" /I "../../SOURCES/INCLUDE/I7041" /I "../../SOURCES/INCLUDE/I7042" /I "../../../IO_SRV_BASE/SOURCES/XDR" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "I7000_IO_SRV - Win32 Debug"

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
# ADD CPP /nologo /w /W0 /Gm /GX /ZI /Od /I "../../SOURCES/INCLUDE" /I "../../../IO_SRV_BASE/SOURCES/INCLUDE" /I "../../SOURCES/INCLUDE/I7041" /I "../../SOURCES/INCLUDE/I7042" /I "../../../IO_SRV_BASE/SOURCES/XDR" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "I7000_IO_SRV - Win32 Release"
# Name "I7000_IO_SRV - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\crypt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\data_block.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7000_cdv.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7000_config.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7000_crc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7000_dev.cpp

!IF  "$(CFG)" == "I7000_IO_SRV - Win32 Release"

!ELSEIF  "$(CFG)" == "I7000_IO_SRV - Win32 Debug"

# ADD CPP /I "../../SOURCES/SRC/I7041"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7000_dev_digital_input.cpp

!IF  "$(CFG)" == "I7000_IO_SRV - Win32 Release"

!ELSEIF  "$(CFG)" == "I7000_IO_SRV - Win32 Debug"

# ADD CPP /w /W0 /I "../../SOURCES/SRC/I7041"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7000_dev_digital_output.cpp

!IF  "$(CFG)" == "I7000_IO_SRV - Win32 Release"

!ELSEIF  "$(CFG)" == "I7000_IO_SRV - Win32 Debug"

# ADD CPP /I "../../SOURCES/SRC/I7041"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7000_drv.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7000_events_cash.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7000_io_srv.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7000_proto.cpp

!IF  "$(CFG)" == "I7000_IO_SRV - Win32 Release"

!ELSEIF  "$(CFG)" == "I7000_IO_SRV - Win32 Debug"

# ADD CPP /I "../../SOURCES/SRC/I7041"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7000_serial.cpp

!IF  "$(CFG)" == "I7000_IO_SRV - Win32 Release"

!ELSEIF  "$(CFG)" == "I7000_IO_SRV - Win32 Debug"

# ADD CPP /I "../../SOURCES/SRC/I7041"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7000_types.cpp

!IF  "$(CFG)" == "I7000_IO_SRV - Win32 Release"

!ELSEIF  "$(CFG)" == "I7000_IO_SRV - Win32 Debug"

# ADD CPP /I "../../SOURCES/SRC/I7041"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7041\I7041_dev.cpp

!IF  "$(CFG)" == "I7000_IO_SRV - Win32 Release"

!ELSEIF  "$(CFG)" == "I7000_IO_SRV - Win32 Debug"

# ADD CPP /I "../../SOURCES/SRC/I7041"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7041\I7041_types.cpp

!IF  "$(CFG)" == "I7000_IO_SRV - Win32 Release"

!ELSEIF  "$(CFG)" == "I7000_IO_SRV - Win32 Debug"

# ADD CPP /I "../../SOURCES/SRC/I7041"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7042\I7042_dev.cpp

!IF  "$(CFG)" == "I7000_IO_SRV - Win32 Release"

!ELSEIF  "$(CFG)" == "I7000_IO_SRV - Win32 Debug"

# ADD CPP /I "../../SOURCES/SRC/I7041"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\SRC\I7042\I7042_types.cpp

!IF  "$(CFG)" == "I7000_IO_SRV - Win32 Release"

!ELSEIF  "$(CFG)" == "I7000_IO_SRV - Win32 Debug"

# ADD CPP /I "../../SOURCES/SRC/I7041"

!ENDIF 

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

SOURCE=..\..\SOURCES\SRC\main.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\SRC\serial_port.cpp
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

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\data_block.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\def_crit_sec.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_cdv.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_config.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_crc.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_dev.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_dev_digital_input.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_dev_digital_output.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_dev_input.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_dev_item.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_dev_output.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_drv.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_events_cash.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_functional.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_io_srv.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_proto.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_serial.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7000_types.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7041\I7041_dev.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7041\I7041_types.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7042\I7042_dev.h
# End Source File
# Begin Source File

SOURCE=..\..\SOURCES\INCLUDE\I7042\I7042_types.h
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

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\serial_port.h
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

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\win2koi_koi2win.h
# End Source File
# Begin Source File

SOURCE=..\..\..\IO_SRV_BASE\SOURCES\INCLUDE\xdr_utils.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE="..\..\..\..\..\Program Files\Microsoft Visual Studio\VC98\Lib\WSOCK32.LIB"
# End Source File
# End Target
# End Project
