
#CC = CC
#CFLAGS = -mips3 -g
CC = g++
CFLAGS = -fpic -g

LIBRARY_NAME = util

LIB = lib$(LIBRARY_NAME)
LIB_DIR = .
LIB_OBJS = atConfigFile.o atBufferHandler.o atString.o atStringBuffer.o \
           atStringTokenizer.o atTimer.o atJoint.o


FOUNDATION_DIR = ../foundation
MATH_DIR = ../math
OS_DIR = ../os


.SUFFIXES: .c++


all: $(LIB).a


$(LIB).a: $(LIB_OBJS)
	ar -ruc $@ $?

.c++.o:
	$(CC) $(CFLAGS) -c -DATLAS_SYM=EXPORT $< -I$(OS_DIR) -I$(FOUNDATION_DIR) -I$(MATH_DIR)


clean:
	/bin/rm -f *.o

allclean:
	/bin/rm -f *.o $(LIB_DIR)/*.a $(LIB_DIR)/*.so so_locations

