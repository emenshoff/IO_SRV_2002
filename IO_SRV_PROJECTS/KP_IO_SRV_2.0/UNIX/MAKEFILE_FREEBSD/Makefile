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
CFLAGS  =	-c -I ${H_DIR} -I ${H_DIR1} -I ${H_DIR2} -I ${H_DIR3} -fexceptions  ${DEFINES}   
#-fhuge-objects 
#-pg
#-Wall
LDFLAGS	=	-lstdc++ -o 
#-pthread -lstdc++ -o 

PROG	=	${OUT_DIR}KP_IO_SRV

OBJS	=	cond_parser.o kp_abstract_dev.o kp_afb.o kp_cdv.o kp_channel.o kp_config.o \
		kp_control.o kp_dev.o kp_events_cash.o kp_fb_light.o kp_fb_tii.o kp_fb_tit.o \
		kp_tii.o kp_fb_ts.o kp_fb_tu.o kp_io_srv.o kp_light.o  \
		io_srv.o io_srv_event_supervisor.o io_srv_event_processor.o \
		serial_port.o stm_srv.o stm_srv_cfg.o stm_srv_cfg_control.o io_srv_drv_types.o \
		kp_tit.o kp_titi.o kp_ts.o kp_tu.o kp_types.o main.o stm_cln.o crypt.o  \
		srv_sock.o srv_tracer.o xdr_utils.o win2koi_koi2win.o data_block.o io_srv_dump.o \

${PROG}: ${OBJS} 
	${CCC} ${OBJS} ${LDFLAGS} ${PROG}

cond_parser.o :
	${CCC} ${CFLAGS} ${SRC_DIR}cond_parser.cpp
kp_abstract_dev.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_abstract_dev.cpp
kp_afb.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_afb.cpp
kp_cdv.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_cdv.cpp
kp_channel.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_channel.cpp
kp_config.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_config.cpp
kp_control.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_control.cpp
kp_dev.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_dev.cpp
kp_events_cash.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_events_cash.cpp
kp_fb_light.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_fb_light.cpp
kp_fb_tii.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_fb_tii.cpp
kp_fb_tit.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_fb_tit.cpp
kp_fb_ts.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_fb_ts.cpp
kp_fb_tu.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_fb_tu.cpp
kp_tii.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_tii.cpp
kp_tit.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_tit.cpp
kp_titi.o :
	${CCC} ${CFLAGS} ${SRC_DIR}kp_titi.cpp
kp_ts.o :
	${CCC} ${CFLAGS} ${SRC_DIR}kp_ts.cpp
kp_tu.o :
	${CCC} ${CFLAGS} ${SRC_DIR}kp_tu.cpp
kp_types.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_types.cpp
kp_fb_ts.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_fb_ts.cpp
kp_fb_tu.o :
	${CCC} ${CFLAGS} ${SRC_DIR}kp_fb_tu.cpp
kp_io_srv.o :
	${CCC} ${CFLAGS} ${SRC_DIR}kp_io_srv.cpp
kp_light.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_light.cpp
kp_io_srv.o:
	${CCC} ${CFLAGS} ${SRC_DIR}kp_io_srv.cpp
kp_light.o: 
	${CCC} ${CFLAGS} ${SRC_DIR}kp_light.cpp
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
