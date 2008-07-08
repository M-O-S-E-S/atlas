
#ifndef AT_ERRNO_H
#define AT_ERRNO_H


#ifdef __cplusplus
   extern "C"
   {
      int   getLastError();
   }
#else
   int   getLastError();
#endif


#endif

