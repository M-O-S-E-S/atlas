
#CC = CC
#CFLAGS = -mips3 -g
CC = g++
CFLAGS = -fpic -g

LIBRARY_NAME = util

LIB = lib$(LIBRARY_NAME)
LIB_DIR = .
LIB_OBJS = atItem.o atConfigFile.o atNotifier.o atBufferHandler.o atString.o


FOUNDATION_DIR = ../foundation


.SUFFIXES: .c++


all: $(LIB).a


$(LIB).a: $(LIB_OBJS)
	ar -ruc $@ $?

.c++.o:
	$(CC) $(CFLAGS) -c $< -I$(FOUNDATION_DIR)


clean:
	/bin/rm -f *.o

allclean:
	/bin/rm -f *.o $(LIB_DIR)/*.a $(LIB_DIR)/*.so so_locations

