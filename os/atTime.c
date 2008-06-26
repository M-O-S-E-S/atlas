
// Under Windows, define the gettimeofday() function with corresponding types
#ifdef _MSC_VER
    #include "atTime.h"

    // CONSTANTS
    #define DELTA_EPOCH_IN_MICROSECS   11644473600000000Ui64


    // GLOBAL VARIABLES (needed to store state between calls)
    __int64   atTimeBaseTime;
    DWORD     atTimeStartTime = 0;


    // FUNCTIONS
    int gettimeofday(struct timeval * tv, struct timezone * tz)
    {
       FILETIME   ft;
       DWORD      nextTime;
       __int64    currentTime;
       int        secondsUTCLocal;
       int        hoursUTCLocal;

       // Check to see if we have run a first time
       if (atTimeStartTime == 0)
       {
          // We haven't initialized so do that now

          // First get the current time in two formats (microsecs since
          // epoch and the number of ticks since reboot); the number of
          // ticks is higher resolution so we need to use it to replicate
          // the microsecond resolution of gettimeofday()
          GetSystemTimeAsFileTime(&ft);
          atTimeStartTime = timeGetTime();

          // Compute the base time in microseconds
          // JTS: Casted the ft.dwHighDateTime to be a 64 bit integer
          // instead of 32 this way the 32 bit shift will or the high
          // and low components together properly.
          atTimeBaseTime = ((__int64)ft.dwHighDateTime << 32) |
             (ft.dwLowDateTime);
          atTimeBaseTime = (atTimeBaseTime / 10) - DELTA_EPOCH_IN_MICROSECS;

          // Initialize the timezone
          _tzset();

          // Set the current high-resolution time offset
          nextTime = atTimeStartTime;
       }
       else
       {
          // We have initialized already

          // Set the current high-resolution time offset
          nextTime = timeGetTime();
       }

       // Compute real world time (in microseconds)
       currentTime = (__int64)(atTimeBaseTime + (nextTime - atTimeStartTime) /
          1000.0);

       // Set the time if asked for
       if (tv != NULL)
       {
          // Set the time struct fields
          tv->tv_sec = (long ) (currentTime / 1000000);
          tv->tv_usec = (long ) (currentTime % 1000000);
       }

       // Set the timezone if asked for
       if (tz != NULL)
       {
          // Set the difference of seconds between UTC and local time
          // to be 28800
          // Set the difference of hours between UTC and local time to be 1
          // These are the defaults from msdn
          secondsUTCLocal = 28800;
          hoursUTCLocal = 1;
      
          // JTS: Replacing the _timezone and _daylight to the non depricated 
          //      function equivilent
          // Set the timezone struct fields
          tz->tz_minuteswest = _get_timezone(&secondsUTCLocal) / 60;
          tz->tz_dsttime = _get_daylight(&hoursUTCLocal);
       }

       // Returning 0 for a success. I never fail!
       return 0;
    }
#endif

