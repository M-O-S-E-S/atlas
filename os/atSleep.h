
#ifndef AT_SLEEP_H
#define AT_SLEEP_H


// Under Windows, define the sleep() and usleep() functions as macros of
// the Windows Sleep() function
#ifdef _MSC_VER
    #include <windows.h>
    #include <wingdi.h>

    // Sleep() takes milliseconds so multiply x by 1000 for sleep()
    #define sleep(x)  Sleep((x) * 1000)

    // Sleep() takes milliseconds, so divide x by 1000 for usleep()
    // if the result of (x/1000) is zero, the thread will still sleep
    // (give up the processor)
    #define usleep(x) Sleep((x) / 1000)
#endif


#endif

