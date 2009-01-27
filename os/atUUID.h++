
#ifndef AT_UUID_H
#define AT_UUID_H


#include "atSymbols.h++"


#ifdef __cplusplus
   extern "C"
   {
      ATLAS_SYM char *   getUUID();
   }
#else
   ATLAS_SYM char *   getUUID();
#endif


#endif

