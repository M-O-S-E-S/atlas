
#ifndef AT_SYMBOLS_H
#define AT_SYMBOLS_H


// Symbol-specific linkage directives.  Windows requires explicit export
// and import instructions for its shared libraries (DLL's) and GCC can
// provide such capabilities as well (although it's not required).
// The following sections define tokens that specify the linkage and
// are used in the Makefile (or Project Settings for Windows).  
//
// Constants for the appropriate library (or even sub-library) are
// defined to one of these constants in the Makefile.  For example,
//
//   g++ -o test test.c++ -DATLAS_SYM=IMPORT
//
// and a class might be defined such as:
// 
// class ATLAS_SYM Test
// {
// };
//
// Typical use would export symbols when making a library that
// will be used by others, and import symbols when making an
// application that uses the library.

#ifdef _WIN32
   #define IMPORT __declspec(dllimport)
   #define EXPORT __declspec(dllexport)
#elif __ANDROID__
   #define IMPORT __attribute__((visibility("default")))
   #define EXPORT __attribute__((visibility("default")))
#else
   #define IMPORT __attribute__((visibility("default")))
   #define EXPORT __attribute__((visibility("default")))
#endif


#endif

