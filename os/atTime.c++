
// Under Windows, define the gettimeofday() function with corresponding types
#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <mmsystem.h>
   #include "atTime.h++"

   // CONSTANTS
   #define DELTA_EPOCH_IN_MICROSECS   11644473600000000Ui64


   // GLOBAL VARIABLES (needed to store state between calls)
   __int64         atTimeBaseTime;
   bool            atTimeInitialized = false;
   bool            atUseMMTimer;
   DWORD           atTimeStartTimeMM;
   LARGE_INTEGER   atTimeStartTimePerf;
   LARGE_INTEGER   atTimerFrequency;


   // FUNCTIONS
   int gettimeofday(struct timeval * tv, struct timezone * tz)
   {
      FILETIME        ft;
      DWORD           nextTimeMM;
      LARGE_INTEGER   nextTimePerf;
      __int64         currentTime;
      __int64         timeDiff;
      long            secondsUTCLocal;
      int             hoursUTCLocal;

      // Check to see if we have run a first time
      if (atTimeInitialized == false)
      {
         // We haven't initialized so do that now

         // First, figure out which timer API to use.  The multimedia timer
         // is reliable, but is only millisecond resolution.  The performance
         // counter is much higher resolution, but on older machines, it can
         // use an unreliable timing mechanism.  We need to figure out which
         // one to use.  To do this, we query the frequency of the hi-res
         // timer.  If the frequency is very high (above 1 GHz) it must be
         // using the CPU time stamp counter (TSC), which is not reliable.
         // If it's less, it's probably using a High Precision Event Timer
         // (HPET), which is reliable and should have at least microsecond
         // granularity and nanosecond accuracy.
         if (QueryPerformanceFrequency(&atTimerFrequency))
         {
            // Check the timer frequency
            if (atTimerFrequency.QuadPart > 1000000000)
            {
               // Must be using the TSC, so use the multimedia timer instead
               atUseMMTimer = true;
            }
            else
            {
               // An HPET seems to be available, so we can use the performance
               // timer
               atUseMMTimer = false;
            }
         }
         else
         {
            // The performance timer query failed for some reason, so fall
            // back to the multimedia timer
            atUseMMTimer = true;
         }

         // Next, get the current time in two formats (microsecs since
         // epoch and the time since reboot); the time since reboot 
         // is higher resolution so we need to use it to replicate
         // the microsecond resolution of gettimeofday()
         GetSystemTimeAsFileTime(&ft);
         if (atUseMMTimer)
         {
            // Use the multimedia timer for the high-resolution time
            atTimeStartTimeMM = timeGetTime();

            // Set the current high-resolution time offset
            nextTimeMM = atTimeStartTimeMM;
         }
         else
         {
            // Use the performance timer for the high-resolution time
            QueryPerformanceCounter(&atTimeStartTimePerf);

            // Set the current high-resolution time offset
            nextTimePerf = atTimeStartTimePerf;
         }

         // Hi-res timer is initialized now
         atTimeInitialized = true;

         // Compute the base time in microseconds
         atTimeBaseTime = ((__int64 ) ft.dwHighDateTime << 32) |
                          (ft.dwLowDateTime);
         atTimeBaseTime = (atTimeBaseTime / 10) - DELTA_EPOCH_IN_MICROSECS;

         // Initialize the timezone
         _tzset();
      }
      else
      {
         // We have initialized already

         // Set the current high-resolution time offset
         if (atUseMMTimer)
         {
            nextTimeMM = timeGetTime();
         }
         else
         {
            QueryPerformanceCounter(&nextTimePerf);
         }
      }

      // Compute real world time (in microseconds)
      if (atUseMMTimer)
      {
         // Using the multimedia timer
         currentTime = (__int64 ) (atTimeBaseTime + 
                                   (nextTimeMM - atTimeStartTimeMM) * 1000.0);
      }
      else
      {
         // Using the performance timer
         timeDiff = nextTimePerf.QuadPart - atTimeStartTimePerf.QuadPart;
         currentTime = (__int64 ) (atTimeBaseTime + 
            ((double) timeDiff / (double) atTimerFrequency.QuadPart) * 1.0E6);
      }

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
         // Get the difference of seconds between UTC and local time and
         // get the difference of hours between UTC and local time
         _get_timezone(&secondsUTCLocal);
         _get_daylight(&hoursUTCLocal);
      
         // Set the timezone struct fields
         tz->tz_minuteswest = secondsUTCLocal / 60;
         tz->tz_dsttime = hoursUTCLocal;
      }

      // Returning 0 for a success (I never fail!)
      return 0;
   }
#endif

