
#ifndef AT_STR_H
#define AT_STRCMP_H


// Under Windows some common string functions are not named the same
#ifdef _MSC_VER

    // There is no strcasecmp in windows, they have stricmp
    #define strcasecmp _stricmp

#endif


#endif

