
#ifndef AT_FILE_HPP
#define AT_FILE_HPP


// Microsoft has a different name for the access function.
// The flags for read access, write access, and so on also need to be defined.
#ifdef _MSC_VER
   #include <io.h>

   #define access   _access
   #define F_OK   0x00
   #define W_OK   0x02
   #define R_OK   0x04

   #define DIRECTORY_SEPARATOR   '\\'
   #define PATH_SEPARATOR        ';'
#else
   #define DIRECTORY_SEPARATOR   '/'
   #define PATH_SEPARATOR        ':'
#endif


#endif

