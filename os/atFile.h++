
#ifndef AT_FILE_HPP
#define AT_FILE_HPP


#include "atSymbols.h++"


// Microsoft has a different name for the access function.
// The flags for read access, write access, and so on also need to be defined.
#ifdef _MSC_VER
   #include <direct.h>
   #include <io.h>

   #define chdir    _chdir
   #define access   _access
   #define F_OK   0x00
   #define W_OK   0x02
   #define R_OK   0x04

   #define DIRECTORY_SEPARATOR   '\\'
   #define PATH_SEPARATOR        ';'
   #define TMP_DIR               getenv("TEMP")
#elif __ANDROID__
   #include <unistd.h>

   #define DIRECTORY_SEPARATOR   '/'
   #define PATH_SEPARATOR        ':'
   #define TMP_DIR               "/sdcard"
#else
   #include <unistd.h>

   #define DIRECTORY_SEPARATOR   '/'
   #define PATH_SEPARATOR        ':'
   #define TMP_DIR               "/tmp"
#endif


enum atPersonalPathKey
{
   AT_PATH_HOME,
   AT_PATH_DESKTOP,
   AT_PATH_DOCUMENTS,
   AT_PATH_DOWNLOADS,
   AT_PATH_MUSIC,
   AT_PATH_PICTURES,
   AT_PATH_PUBLIC,
   AT_PATH_TEMPLATES,
   AT_PATH_VIDEOS
};


#ifdef __cplusplus
   extern "C"
   {
      ATLAS_SYM bool     isDirectory(char * path);
      ATLAS_SYM bool     createDirectory(char * path);
      ATLAS_SYM int      listFiles(char * path, char ** results, int count);
      ATLAS_SYM char *   getPersonalPath(atPersonalPathKey key);
   }
#else
   ATLAS_SYM bool     isDirectory(char * path);
   ATLAS_SYM bool     createDirectory(char * path);
   ATLAS_SYM int      listFiles(char * path, char ** results, int count);
   ATLAS_SYM char *   getPersonalPath(atPersonalPathKey key);
#endif


#endif

