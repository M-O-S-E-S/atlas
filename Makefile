
#CC = CC
#CFLAGS = -mips3 -g
CC = g++
CFLAGS = -fpic -g

LIBRARY_NAME = atlas

MATH_DIR = ./math
UTIL_DIR = ./util
CONTAINER_DIR = ./container
XML_DIR = ./xml

LIB = lib$(LIBRARY_NAME)
LIB_DIR = .
LIB_OBJS = $(MATH_DIR)/*.o $(UTIL_DIR)/*.o $(CONTAINER_DIR)/*.o $(XML_DIR)/*.o
           

.SUFFIXES: .c++


all: $(LIB_DIR)/$(LIB).so


$(LIB_DIR)/$(LIB).so: $(LIB_DIR)/$(LIB).a
	g++ -shared -o $@ -Wl,--whole-archive $(LIB_OBJS) -Wl,--no-whole-archive

$(LIB_DIR)/$(LIB).a: $(MATH_DIR)/libmath.a $(UTIL_DIR)/libutil.a $(CONTAINER_DIR)/libcontainer.a $(XML_DIR)/libxml.a
	ar -ruc $@ $(LIB_OBJS)

.c++.o:
	cd $(MATH_DIR); $(MAKE)
	cd $(CONTAINER_DIR); $(MAKE)
	cd $(UTIL_DIR); $(MAKE)
	cd $(XML_DIR); $(MAKE)

$(UTIL_DIR)/libutil.a:
	cd $(UTIL_DIR) && $(MAKE)

$(CONTAINER_DIR)/libcontainer.a:
	cd $(CONTAINER_DIR) && $(MAKE)

$(MATH_DIR)/libmath.a:
	cd $(MATH_DIR) && $(MAKE)

$(XML_DIR)/libxml.a:
	cd $(XML_DIR) && $(MAKE)

clean:
	cd $(MATH_DIR); $(MAKE) clean
	cd $(CONTAINER_DIR); $(MAKE) clean
	cd $(UTIL_DIR); $(MAKE) clean
	cd $(XML_DIR); $(MAKE) clean
	/bin/rm -f *.o

allclean:
	cd $(MATH_DIR); $(MAKE) allclean
	cd $(CONTAINER_DIR); $(MAKE) allclean
	cd $(UTIL_DIR); $(MAKE) allclean
	cd $(XML_DIR); $(MAKE) allclean
	/bin/rm -f *.o $(LIB_DIR)/*.a $(LIB_DIR)/*.so so_locations

