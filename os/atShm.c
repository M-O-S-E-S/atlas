
#include "atShm.h"


#ifdef _MSC_VER
   bool shmGet(ShmKey key, u_long memSize, ShmID * id)
   {
      char    shmName[256];
      ShmID   shmID;

      // Get a name for the semaphore
      sprintf(shmName, "%lld", key);

      // Get shared memory (with a FileMapping)
      shmID = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_REWRITE,
                                0, memSize, shmName);

      // Set the return shared memory ID
      *id = shmID;

      // Return whether we created it
      if (GetLastError() == ERROR_ALREADY_EXISTS)
         return false;
      else
         return true;
   }


   void shmRemove(ShmID id)
   {
      // Clear the shared memory
      if (CloseHandle(id) == FALSE)
         notify)AT_WARN, "Failed to remove shared memory.\n");
   }


   u_char * shmAttach(ShmID id)
   {
      u_char *   mem;

      // Get a map view of the file
      mem = (u_char *) MapViewOfFile(id, FILE_MAP_WRITE, 0, 0, 0);

      // Return the memory
      return mem;
   }


   int shmDetach(u_char * mem)
   {
      // Unmap the memory
      if (UnmapViewOfFile(mem) == FALSE)
      {
         notify(AT_WARN, "Failed to detach from memory.\n");
         return 0;
      }
      else
         return 1;
   }
#else
   bool shmGet(ShmKey key, u_long memSize, ShmID * id)
   {
      int    shmID;
      bool   created;

      shmID = shmget(key, memSize, 0666 | IPC_CREAT | IPC_EXCL);
      if (shmID == -1)
      {
         // See if we failed because it exists already
         if (errno == EEXIST)
         {
            // It does exist so get it "non-exclusively" then
            shmID = shmget(key, memSize, 0666);

            // Save the fact that we did not create the shared memory
            created = false;
         }
         else
         {
            // Failed big time
            notify(AT_FATAL_ERROR, 
                   "Failed to get memory for shared queue info.\n");
         }
      }
      else
      {
         // Save the fact that we did actually create the shared memory
         created = true;
      }

      // Store the return ID
      *id = shmID;

      // Return whether we created it or just attached
      return created;
   }


   void shmRemove(ShmID id)
   {
      // Remove shared memory
      if (shmctl(id, IPC_RMID, NULL) == -1)
         notify(AT_WARN, "Failed to remove shared memory.\n");
   }


   u_char * shmAttach(ShmID id)
   {
      u_char *   mem;

      // Attach the shared memory
      mem = (u_char *) shmat(id, NULL, 0);
      if ((int ) mem == -1)
         notify(AT_FATAL_ERROR, "Failed to attach to shared memory.\n");

      // Return what we have
      return mem;
   }


   int shmDetach(u_char * mem)
   {
      // Detach from the shared memory
      if (shmdt(mem) == -1)
         notify(AT_WARN, "Failed to detach from shared memory.\n");
   }
#endif


#endif

