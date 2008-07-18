
CC = g++
CFLAGS = -fpic -g

LIBRARY_NAME = os

LIB = lib$(LIBRARY_NAME)
LIB_DIR = .
LIB_OBJS = atDynamic.o \
           atByteSwap.o \
           atErrno.o \
           atNetwork.o \
           atSem.o \
           atShm.o \
           atTime.o \

.SUFFIXES: .c++


all: $(LIB).a


$(LIB).a: $(LIB_OBJS)
	ar -ruc $@ $?

.c++.o:
	$(CC) $(CFLAGS) -c -DATLAS_SYM=EXPORT $<

clean:
	/bin/rm -f *.o

allclean:
	/bin/rm -f *.o $(LIB_DIR)/*.a $(LIB_DIR)/*.so so_locations

