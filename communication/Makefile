
#CC = CC
#CFLAGS = -mips3 -g
CC = g++
CFLAGS = -fpic -g

LIBRARY_NAME = communication

LIB = lib$(LIBRARY_NAME)
LIB_DIR = .
LIB_OBJS = atIPCInterface.o \
           atNetworkInterface.o \
           atUDPNetworkInterface.o atTCPNetworkInterface.o \
           atSharedMemoryInterface.o atSharedQueue.o \
           atMessageQueueInterface.o \
           atThreadInterface.o atThreadQueue.o \
           atSerialInterface.o \
           atHLAInterface.o atRTIInterface.o atRTIInterfaceAmbassador.o \
           atNameValuePair.o atKeyedBufferHandler.o


OS_DIR = ../os
FOUNDATION_DIR = ../foundation
CONTAINER_DIR = ../container
UTIL_DIR = ../util

RTI_DIR = /irl/tools/libs/rtis-1.3_D18A


.SUFFIXES: .c++


all: $(LIB).a


$(LIB).a: $(LIB_OBJS)
	ar -ruc $@ $?

.c++.o:
	$(CC) $(CFLAGS) -c -DATLAS_SYM=EXPORT $< -I$(OS_DIR) -I$(FOUNDATION_DIR) -I$(CONTAINER_DIR) -I$(UTIL_DIR) -I$(RTI_DIR)/include/1.3


clean:
	/bin/rm -f *.o

allclean:
	/bin/rm -f *.o $(LIB_DIR)/*.a $(LIB_DIR)/*.so so_locations

