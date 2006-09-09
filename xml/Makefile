
#CC = CC
#CFLAGS = -mips3 -g
CC = g++
CFLAGS = -fpic -g

LIBRARY_NAME = xml

LIB = lib$(LIBRARY_NAME)
LIB_DIR = .
LIB_OBJS = atXMLBuffer.o atXMLDocument.o atXMLReader.o

FOUNDATION_DIR = ../foundation
UTIL_DIR = ../util
CONTAINER_DIR = ../container
XML_LIB_DIR = /usr/include/libxml2


.SUFFIXES: .c++


all: $(LIB).a


$(LIB).a: $(LIB_OBJS)
	ar -ruc $@ $?

.c++.o:
	$(CC) $(CFLAGS) -c $< -I$(XML_LIB_DIR) -I$(FOUNDATION_DIR) -I$(UTIL_DIR) -I$(CONTAINER_DIR)


clean:
	/bin/rm -f *.o

allclean:
	/bin/rm -f *.o $(LIB_DIR)/*.a $(LIB_DIR)/*.so so_locations

