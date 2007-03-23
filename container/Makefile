
#CC = CC
#CFLAGS = -mips3 -g
CC = g++
CFLAGS = -fpic -g

LIBRARY_NAME = container

LIB = lib$(LIBRARY_NAME)
LIB_DIR = .
LIB_OBJS = atArray.o \
           atList.o \
           atMap.o \
           atPriorityQueue.o


FOUNDATION_DIR = ../foundation
UTIL_DIR = ../util


.SUFFIXES: .c++


all: $(LIB).a


$(LIB).a: $(LIB_OBJS)
	ar -ruc $@ $?

.c++.o:
	$(CC) $(CFLAGS) -c $< -I$(FOUNDATION_DIR) -I$(UTIL_DIR)


clean:
	/bin/rm -f *.o

allclean:
	/bin/rm -f *.o $(LIB_DIR)/*.a $(LIB_DIR)/*.so so_locations

