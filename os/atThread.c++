
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


#include "atErrno.h++"
#include "atThread.h++"


/*
 Adapted from:
    http://ptgmedia.pearsoncmg.com/images/0201633922/sourcecode/barrier.c

    to provide barrier support to Android's interior pthread library
*/


#if defined(__ANDROID__) || defined(__IOS__)
   int pthread_barrier_init (pthread_barrier_t *barrier, 
                             const pthread_barrierattr_t *attr, int count)
   {
       /* Note attributes not supported! */
       int status;

       barrier->threshold = barrier->counter = count;
       barrier->cycle = 0;
       status = pthread_mutex_init (&barrier->mutex, NULL);
       if (status != 0)
           return status;
       status = pthread_cond_init (&barrier->cv, NULL);
       if (status != 0) 
       {
           pthread_mutex_destroy (&barrier->mutex);
           return status;
       }
       barrier->valid = BARRIER_VALID;
       return 0;
   }


   int pthread_barrier_destroy (pthread_barrier_t *barrier)
   {
       int status, status2;
   
       if (barrier->valid != BARRIER_VALID)
           return EINVAL;
   
       status = pthread_mutex_lock (&barrier->mutex);
       if (status != 0)
           return status;
   
       /*
        * Check whether any threads are known to be waiting; report
        * "BUSY" if so.
        */
       if (barrier->counter != barrier->threshold) 
       {
           pthread_mutex_unlock (&barrier->mutex);
           return EBUSY;
       }
   
       barrier->valid = 0;
       status = pthread_mutex_unlock (&barrier->mutex);
       if (status != 0)
           return status;
   
       /*
        * If unable to destroy either 1003.1c synchronization
        * object, return the error status.
        */
       status = pthread_mutex_destroy (&barrier->mutex);
       status2 = pthread_cond_destroy (&barrier->cv);
       return (status == 0 ? status : status2);
   }
   
   
   int pthread_barrier_wait (pthread_barrier_t *barrier)
   {
       int status, cancel, tmp, cycle;
   
       if (barrier->valid != BARRIER_VALID)
           return EINVAL;
   
       status = pthread_mutex_lock (&barrier->mutex);
       if (status != 0)
           return status;
   
       cycle = barrier->cycle;   /* Remember which cycle we're on */
   
       if (--barrier->counter == 0) 
       {
           barrier->cycle = !barrier->cycle;
           barrier->counter = barrier->threshold;
           status = pthread_cond_broadcast (&barrier->cv);
           /*
            * The last thread into the barrier will return status
            * -1 rather than 0, so that it can be used to perform
            * some special serial code following the barrier.
            */
           if (status == 0)
               status = -1;
       } 
       else 
       {
           /*
            * Wait with cancellation disabled, because barrier_wait
            * should not be a cancellation point.
            * GAM : Not supported under Android
           pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, &cancel);
            */
   
           /*
            * Wait until the barrier's cycle changes, which means
            * that it has been broadcast, and we don't want to wait
            * anymore.
            */
           while (cycle == barrier->cycle) 
           {
               status = pthread_cond_wait (
                       &barrier->cv, &barrier->mutex);
               if (status != 0) break;
           }
   
           /*
            * GAM : Not supported under Android
           pthread_setcancelstate (cancel, &tmp);
            */
       }

       /*
        * Ignore an error in unlocking. It shouldn't happen, and
        * reporting it here would be misleading -- the barrier wait
        * completed, after all, whereas returning, for example,
        * EINVAL would imply the wait had failed. The next attempt
        * to use the barrier *will* return an error, or hang, due
        * to whatever happened to the mutex.
        */
       pthread_mutex_unlock (&barrier->mutex);
       return status;          /* error, -1 for waker, or 0 */
   }
#endif

