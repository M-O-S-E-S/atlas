
#include "atSem.h++"


#ifdef _MSC_VER
   #include <stdio.h>

   bool semGet(SemKey key, SemID * id)
   {
      char    semName[256];
      SemID   semID;

      // Get a name for the semaphore
      sprintf(semName, "%lld_mutex", key);

      // Create a semaphore (well, in Windows we really use a mutex)
      semID = CreateMutex(NULL, FALSE, semName);

      // Set the return "semaphore" ID
      *id = semID;

      // Return whether we created it
      if (GetLastError() == ERROR_ALREADY_EXISTS)
         return false;
      else
         return true;
   }


   void semRemove(SemID id)
   {
      // Clear the "semaphore" (mutex)
      CloseHandle(id);
   }


   int semLock(SemID id)
   {
      u_long   waitResult;

      // Try to do the lock and return either success or failure (return
      // 1 if we succeeded, -1 if we were interrupted, or 0 for all other
      // failures)
      waitResult = WaitForSingleObject(id, INFINITE);
      if (waitResult == WAIT_OBJECT_0)
         return 1;
      else if (waitResult == WAIT_FAILED)
         return -1;
      else
         return 0;
   }


   int semUnlock(SemID id)
   {
      // Try to do the unlock and return either success or failure
      if (ReleaseMutex(id) == FALSE)
         return 0;
      else
         return 1;
   }
#else
   // CONSTANTS
   #define AT_SHQ_LOCK_SEMAPHORE_NUM   0

   #define AT_SHQ_NUM_LOCK_SEQUENCE_OPS     2
   #define AT_SHQ_NUM_UNLOCK_SEQUENCE_OPS   1


   union semunion
   {
      int               val;
      struct semid_ds   *buf;
      ushort            *array;
   };

   // In certain cases we need to define the "union semun" (see comments)
   #if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
   // union semun is defined by including <sys/sem.h>
   #else
   // according to X/OPEN we have to define it ourselves
   union semun
   {
      int val;                  /* value for SETVAL */
      struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
      unsigned short *array;    /* array for GETALL, SETALL */
                                /* Linux specific part: */
      struct seminfo *__buf;    /* buffer for IPC_INFO */
   };
   #endif


   // GLOBAL VARIABLES 
   struct sembuf atShqLockSequence[AT_SHQ_NUM_LOCK_SEQUENCE_OPS] =
   {
      AT_SHQ_LOCK_SEMAPHORE_NUM, 0, 0,
      AT_SHQ_LOCK_SEMAPHORE_NUM, 1, 0
   };

   struct sembuf atShqUnlockSequence[AT_SHQ_NUM_UNLOCK_SEQUENCE_OPS] =
   {  
      AT_SHQ_LOCK_SEMAPHORE_NUM, -1, 0
   };


   bool semGet(SemKey key, SemID * id)
   {
      int           semID;
      bool          created;
      union semun   zero;

      semID = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
      if (semID == -1)
      {
         // See if we failed because it exists already
         if (errno == EEXIST)
         {
            // It does exist so get it "non-exclusively" then
            semID = semget(key, 1, 0666 | IPC_CREAT);

            // Save the fact that we did not create the control semaphore
            created = false;
         }
      }
      else
      {
         // Save the fact that we actually created the semaphore
         created = true;

         // Initialize the semaphore value
         zero.val = 0;
         semctl(semID, AT_SHQ_LOCK_SEMAPHORE_NUM, SETVAL, zero);
      }

      // Store the return ID
      *id = semID;

      // Return whether we created it or just attached
      return created;
   }


   void semRemove(SemID id)
   {
      union semunion   zero;

      // Remove semaphore
      zero.val = 0;
      semctl(id, 1, IPC_RMID, zero);
   }


   int semLock(SemID id)
   {
      // Try to do the lock and return either success or failure.  If we 
      // failed because of a reason besides that we were interrupted 
      // (by a signal), then notify that fact to the user as well.
      if (semop(id, &atShqLockSequence[0], 
                AT_SHQ_NUM_LOCK_SEQUENCE_OPS) == -1)
      {
         // See if an interrupt occured that caused the failure
         if (errno != EINTR)
         {
            // Return that we were interrupted
            return -1;
         }

         // Return that we failed
         return 0;
      }
      else
      {
         // Return that we succeeded
         return 1;
      }
   }


   int semUnlock(SemID id)
   {
      // Try to do the unlock and return either success or failure.  If we 
      // failed because of a reason besides that we were interrupted 
      // (by a signal), then notify that fact to the user as well.
      if (semop(id, &atShqUnlockSequence[0], 
                AT_SHQ_NUM_UNLOCK_SEQUENCE_OPS) == -1)
      {
         // See if an interrupt occured that caused the failure
         if (errno != EINTR)
         {
            // Return that we were interrupted
            return -1;
         }

         // Return that we failed
         return 0;
      }
      else
      {  
         // Return that we succeeded
         return 1;
      }
   }
#endif

