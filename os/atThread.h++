
#ifndef AT_THREAD_H
#define AT_THREAD_H


#include "atSymbols.h++"


#ifdef __ANDROID__
   #include <pthread.h>

   #define BARRIER_VALID   0xdbcafe

   #define BARRIER_INITIALIZER(cnt) \
       {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, \
       BARRIER_VALID, cnt, cnt, 0}

   typedef struct pthread_barrier_tag 
   {
       pthread_mutex_t   mutex;          /* Control access to barrier */
       pthread_cond_t    cv;             /* wait for barrier */
       int               valid;          /* set when valid */
       int               threshold;      /* number of threads required */
       int               counter;        /* current number of threads */
       int               cycle;          /* alternate wait cycles (0 or 1) */
   } pthread_barrier_t;


   #define __SIZEOF_PTHREAD_BARRIERATTR_T 4
   typedef union
   {
      char __size[__SIZEOF_PTHREAD_BARRIERATTR_T];
      int __align;
   } pthread_barrierattr_t;
#endif


#ifdef __ANDROID__
#ifdef __cplusplus
   extern "C"
   {
      ATLAS_SYM int   pthread_barrier_init(pthread_barrier_t * barrier, 
                         const pthread_barrierattr_t * attr,
                         int count);
      ATLAS_SYM int   pthread_barrier_destroy(pthread_barrier_t * barrier);
      ATLAS_SYM int   pthread_barrier_wait(pthread_barrier_t * barrier);
   }
#else
   ATLAS_SYM int   pthread_barrier_init(pthread_barrier_t * barrier, 
                      const pthread_barrierattr_t *restrict  attr,
                      int count);
   ATLAS_SYM int   pthread_barrier_destroy(pthread_barrier_t * barrier);
   ATLAS_SYM int   pthread_barrier_wait(pthread_barrier_t * barrier);
#endif
#endif


#endif

