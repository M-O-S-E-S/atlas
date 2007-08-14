
#CC = CC
#CFLAGS = -mips3 -g
CC = g++
CFLAGS = -fpic -g

LIBRARY_NAME = atlas

COMMUNICATION_DIR = ./communication
CONTAINER_DIR = ./container
FOUNDATION_DIR = ./foundation
MATH_DIR = ./math
UTIL_DIR = ./util
XML_DIR = ./xml

LIB = lib$(LIBRARY_NAME)
LIB_DIR = .
LIB_OBJS = $(COMMUNICATION_DIR)/*.o $(CONTAINER_DIR)/*.o $(FOUNDATION_DIR)/*.o $(MATH_DIR)/*.o $(UTIL_DIR)/*.o $(XML_DIR)/*.o

RTI_LIB_DIR = /irl/tools/libs/rtis-1.3_D18A/lib/linux_g++-4.1
           

.SUFFIXES: .c++


all: $(LIB_DIR)/$(LIB).so


$(LIB_DIR)/$(LIB).so: $(LIB_DIR)/$(LIB).a
	g++ -shared -o $@ -Wl,--whole-archive $(LIB_OBJS) -Wl,--no-whole-archive -lrti13 -L$(RTI_LIB_DIR) -Wl,--no-whole-archive

$(LIB_DIR)/$(LIB).a: $(COMMUNICATION_DIR)/libcommunication.a $(CONTAINER_DIR)/libcontainer.a $(FOUNDATION_DIR)/libfoundation.a $(MATH_DIR)/libmath.a $(UTIL_DIR)/libutil.a $(XML_DIR)/libxml.a
	ar -ruc $@ $(LIB_OBJS)

.c++.o:
	cd $(COMMUNICATION_DIR); $(MAKE)
	cd $(CONTAINER_DIR); $(MAKE)
	cd $(FOUNDATION_DIR); $(MAKE)
	cd $(MATH_DIR); $(MAKE)
	cd $(UTIL_DIR); $(MAKE)
	cd $(XML_DIR); $(MAKE)

$(COMMUNICATION_DIR)/libcommunication.a:
	cd $(COMMUNICATION_DIR) && $(MAKE)

$(CONTAINER_DIR)/libcontainer.a:
	cd $(CONTAINER_DIR) && $(MAKE)

$(FOUNDATION_DIR)/libfoundation.a:
	cd $(FOUNDATION_DIR) && $(MAKE)

$(MATH_DIR)/libmath.a:
	cd $(MATH_DIR) && $(MAKE)

$(UTIL_DIR)/libutil.a:
	cd $(UTIL_DIR) && $(MAKE)

$(XML_DIR)/libxml.a:
	cd $(XML_DIR) && $(MAKE)

clean:
	cd $(COMMUNICATION_DIR); $(MAKE) clean
	cd $(CONTAINER_DIR); $(MAKE) clean
	cd $(FOUNDATION_DIR); $(MAKE) clean
	cd $(MATH_DIR); $(MAKE) clean
	cd $(UTIL_DIR); $(MAKE) clean
	cd $(XML_DIR); $(MAKE) clean
	/bin/rm -f *.o

allclean:
	cd $(COMMUNICATION_DIR); $(MAKE) allclean
	cd $(CONTAINER_DIR); $(MAKE) allclean
	cd $(FOUNDATION_DIR); $(MAKE) allclean
	cd $(MATH_DIR); $(MAKE) allclean
	cd $(UTIL_DIR); $(MAKE) allclean
	cd $(XML_DIR); $(MAKE) allclean
	/bin/rm -f *.o $(LIB_DIR)/*.a $(LIB_DIR)/*.so so_locations

