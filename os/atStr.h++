
#ifndef AT_STR_H
#define AT_STR_H


// Under Windows some common string functions are not named the same
#ifdef _MSC_VER
   // There is no strcasecmp or strncasecmp in windows, they have 
   // stricmp and strnicmp
   #define strcasecmp    _stricmp
   #define strncasecmp   _strnicmp
#endif


#endif

