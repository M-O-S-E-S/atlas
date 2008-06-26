
#ifndef AT_TIME_H
#define AT_TIME_H


// Under Windows, define the gettimeofday() function with corresponding types
#ifdef _MSC_VER
    #include <time.h>


    // TYPES
    struct timezone
    {
       int   tz_minuteswest;
       int   tz_dsttime;
    };


    // FUNCTIONS
    int gettimeofday(struct timeval * tv, struct timezone * tz);
#endif


#endif

