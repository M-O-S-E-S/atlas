
#CC = CC
#CFLAGS = -mips3 -g
CC = g++
CFLAGS = -fpic -g

LIBRARY_NAME = atlas

MATH_DIR = ./math
UTIL_DIR = ./util
CONTAINER_DIR = ./container

LIB = lib$(LIBRARY_NAME)
LIB_DIR = .
LIB_OBJS = $(MATH_DIR)/*.o $(UTIL_DIR)/*.o $(CONTAINER_DIR)/*.o
           

.SUFFIXES: .c++


all: $(LIB).so


$(LIB).so: $(LIB).a
	g++ -shared -o $@ -Wl,--whole-archive $(LIB_OBJS) -Wl,--no-whole-archive

$(LIB).a: $(MATH_DIR)/libmath.a $(UTIL_DIR)/libutil.a $(CONTAINER_DIR)/libcontainer.a
	ar -ruc $@ $(LIB_OBJS)

.c++.o:
	cd $(MATH_DIR); make
	cd $(CONTAINER_DIR); make
	cd $(UTIL_DIR); make


clean:
	cd $(MATH_DIR); make clean
	cd $(CONTAINER_DIR); make clean
	cd $(UTIL_DIR); make clean
	/bin/rm -f *.o

allclean:
	cd $(MATH_DIR); make allclean
	cd $(CONTAINER_DIR); make allclean
	cd $(UTIL_DIR); make allclean
	/bin/rm -f *.o $(LIB_DIR)/*.a $(LIB_DIR)/*.so so_locations

