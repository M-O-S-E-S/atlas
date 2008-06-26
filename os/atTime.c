
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
          atTimeBaseTime = (ft.dwHighDateTime << 32) | (ft.swLowDateTime);
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
       currentTime = atTimeBaseTime + (nextTime - atTimeStartTime) / 1000.0;

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
          // Set the timezone struct fields
          tz->tz_minuteswest = _timezone / 60;
          tz->tz_dsttime = _daylight;
       }
    }
#endif

