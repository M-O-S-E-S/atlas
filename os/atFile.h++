
#ifndef AT_FILE_HPP
#define AT_FILE_HPP


#include "atSymbols.h++"


// Microsoft has a different name for the access function.
// The flags for read access, write access, and so on also need to be defined.
#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <io.h>

   #define access   _access
   #define F_OK   0x00
   #define W_OK   0x02
   #define R_OK   0x04

   #define DIRECTORY_SEPARATOR   '\\'
   #define PATH_SEPARATOR        ';'

   // FUNCTIONS
   #ifdef __cplusplus
      extern "C"
      {
         ATLAS_SYM int    chdir(const char * path);
      }
   #else
      ATLAS_SYM int    chdir(const char * path);
   #endif
#else
   #define DIRECTORY_SEPARATOR   '/'
   #define PATH_SEPARATOR        ':'
#endif


#ifdef __cplusplus
   extern "C"
   {
      ATLAS_SYM bool   createDirectory(char * path);
      ATLAS_SYM int    listFiles(char * path, char ** results, int count);
   }
#else
   ATLAS_SYM bool   createDirectory(char * path);
   ATLAS_SYM int    listFiles(char * path, char ** results, int count);
#endif


#endif

