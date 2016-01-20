
// ATLAS: Adaptable Tool Library for Advanced Simulation
//
// Copyright 2015 University of Central Florida
//
//
// This library provides many fundamental capabilities used in creating
// virtual environment simulations.  It includes elements such as vectors,
// matrices, quaternions, containers, communication schemes (UDP, TCP, DIS,
// HLA, Bluetooth), and XML processing.  It also includes some extensions
// to allow similar code to work in Linux and in Windows.  Note that support
// for iOS and Android development is also included.
//
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef AT_THREAD_H
#define AT_THREAD_H


#include "atSymbols.h++"


#if defined(__ANDROID__) || defined(__IOS__)
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


#if defined(__ANDROID__) || defined(__IOS__)
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

