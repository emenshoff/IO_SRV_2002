CC			=	gcc
CCC			=	g++
H_DIR   =	../../SOURCES/INCLUDE/
H_DIR1	=	../../SOURCES/INCLUDE/I7041
H_DIR2	=	../../SOURCES/INCLUDE/I7042
H_DIR3	=	../../../IO_SRV_BASE/SOURCES/INCLUDE/
#H_DIR4	=	../../../IO_SRV_BASE/XDR/
SRC_DIR =	../../SOURCES/SRC/
SRC_DIR2=	../../../IO_SRV_BASE/SOURCES/SRC/
SRC_DIR3	=	../../SOURCES/SRC/I7041/
SRC_DIR4	=	../../SOURCES/SRC/I7042/
OUT_DIR =	../../BIN/RELEASE/UNIX/
DEFINES	=	-D_DEBUG_LOG_
# -DMULTI_THREADED
CFLAGS  =	-c -I ${H_DIR} -I ${H_DIR1} -I ${H_DIR2} -I ${H_DIR3} -fexceptions  ${DEFINES}   -fhuge-objects -g0 
#-pg
#-Wall
LDFLAGS	=	-lstdc++ -o 
#-pthread -lstdc++ -o 


PROG	=	${OUT_DIR}I7000_IO_SRV

OBJS	=	I7000_cdv.o I7000_config.o io_srv.o io_srv_event_supervisor.o\
		I7000_crc.o I7000_dev.o I7000_dev_digital_input.o I7000_dev_digital_output.o \
		I7000_drv.o I7000_events_cash.o  I7000_io_srv.o  \
		I7000_proto.o I7000_serial.o I7000_types.o io_srv_event_processor.o \
		main.o serial_port.o stm_srv.o stm_srv_cfg.o stm_srv_cfg_control.o io_srv_drv_types.o \
		stm_cln.o crypt.o I7041_dev.o I7041_types.o I7042_dev.o I7042_types.o \
		srv_sock.o srv_tracer.o xdr_utils.o win2koi_koi2win.o data_block.o io_srv_dump.o \

${PROG}: ${OBJS} 
	${CCC} ${OBJS} ${LDFLAGS} ${PROG}
I7041_dev.o:
	${CCC} ${CFLAGS} ${SRC_DIR3}I7041_dev.cpp
I7041_types.o:
	${CCC} ${CFLAGS} ${SRC_DIR3}I7041_types.cpp 
I7042_dev.o:
	${CCC} ${CFLAGS} ${SRC_DIR4}I7042_dev.cpp 
I7042_types.o:
	${CCC} ${CFLAGS} ${SRC_DIR4}I7042_types.cpp
I7000_cdv.o:
	${CCC} ${CFLAGS} ${SRC_DIR}I7000_cdv.cpp
I7000_config.o:
	${CCC} ${CFLAGS} ${SRC_DIR}I7000_config.cpp
I7000_crc.o:
	${CCC} ${CFLAGS} ${SRC_DIR}I7000_crc.cpp
I7000_dev.o:
	${CCC} ${CFLAGS} ${SRC_DIR}I7000_dev.cpp
I7000_dev_digital_input.o:
	${CCC} ${CFLAGS} ${SRC_DIR}I7000_dev_digital_input.cpp
I7000_dev_digital_output.o:
	${CCC} ${CFLAGS} ${SRC_DIR}I7000_dev_digital_output.cpp
I7000_drv.o:
	${CCC} ${CFLAGS} ${SRC_DIR}I7000_drv.cpp
I7000_events_cash.o:
	${CCC} ${CFLAGS} ${SRC_DIR}I7000_events_cash.cpp
I7000_io_srv.o:
	${CCC} ${CFLAGS} ${SRC_DIR}I7000_io_srv.cpp
I7000_proto.o:
	${CCC} ${CFLAGS} ${SRC_DIR}I7000_proto.cpp
I7000_serial.o:
	${CCC} ${CFLAGS} ${SRC_DIR}I7000_serial.cpp
I7000_types.o:
	${CCC} ${CFLAGS} ${SRC_DIR}I7000_types.cpp
main.o :
	${CCC} ${CFLAGS} ${SRC_DIR}main.cpp
data_block.o :
	${CCC} ${CFLAGS} ${SRC_DIR2}data_block.cpp 
serial_port.o :
	${CCC} ${CFLAGS} ${SRC_DIR2}serial_port.cpp
stm_srv.o :
	${CCC} ${CFLAGS} ${SRC_DIR2}stm_srv.cpp
stm_srv_cfg.o :
	${CCC} ${CFLAGS} ${SRC_DIR2}stm_srv_cfg.cpp
stm_srv_cfg_control.o :
	${CCC} ${CFLAGS} ${SRC_DIR2}stm_srv_cfg_control.cpp
stm_cln.o :
	${CCC} ${CFLAGS} ${SRC_DIR2}stm_cln.cpp
srv_sock.o :
	${CCC} ${CFLAGS} ${SRC_DIR2}srv_sock.cpp
srv_tracer.o :
	${CCC} ${CFLAGS} ${SRC_DIR2}srv_tracer.cpp
xdr_utils.o :
	${CCC} ${CFLAGS} ${SRC_DIR2}xdr_utils.cpp
io_srv_dump.o:
	${CCC} ${CFLAGS} ${SRC_DIR2}io_srv_dump.cpp
io_srv_drv_types.o:
	${CCC} ${CFLAGS} ${SRC_DIR2}io_srv_drv_types.cpp
io_srv_event_processor.o:
	${CCC} ${CFLAGS} ${SRC_DIR2}io_srv_event_processor.cpp
io_srv_event_supervisor.o:
	${CCC} ${CFLAGS} ${SRC_DIR2}io_srv_event_supervisor.cpp
io_srv.o:
	${CCC} ${CFLAGS} ${SRC_DIR2}io_srv.cpp
crypt.o :
	${CC} -c ${SRC_DIR2}crypt.c
win2koi_koi2win.o:
	${CC} -c ${SRC_DIR2}win2koi_koi2win.c

clean: clean_obj
	rm -f ${PROG}
clean_obj:
	rm -f *.o 
