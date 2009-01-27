
#ifndef AT_UUID_H
#define AT_UUID_H


#include "atSymbols.h++"


#ifdef __cplusplus
   extern "C"
   {
      ATLAS_SYM void   getUUID(char * id);
   }
#else
   ATLAS_SYM void   getUUID(char * id);
#endif


#endif

