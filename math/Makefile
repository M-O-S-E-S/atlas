
#CC = CC
#CFLAGS = -mips3 -g
CC = g++
CFLAGS = -fpic -g

LIBRARY_NAME = math

LIB = lib$(LIBRARY_NAME)
LIB_DIR = .
LIB_OBJS = atVector.o atMatrix.o atQuat.o


FOUNDATION_DIR = ../foundation
CONTAINER_DIR = ../container
UTIL_DIR = ../util


.SUFFIXES: .c++


all: $(LIB).a


$(LIB).a: $(LIB_OBJS)
	ar -ruc $@ $?

.c++.o:
	$(CC) $(CFLAGS) -c $< -I$(FOUNDATION_DIR) -I$(CONTAINER_DIR) -I$(UTIL_DIR)


clean:
	/bin/rm -f *.o

allclean:
	/bin/rm -f *.o $(LIB_DIR)/*.a $(LIB_DIR)/*.so so_locations

