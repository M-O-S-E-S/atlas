
#ifndef AT_SPAWN_H
#define AT_SPAWN_H


#include "atSymbols.h++"


#ifdef __cplusplus
   extern "C"
   {
      ATLAS_SYM bool   spawn(char * execName, char * cmdParameters);
   }
#else
   ATLAS_SYM bool   spawn(char * execName, char * cmdParameters);
#endif


#endif

