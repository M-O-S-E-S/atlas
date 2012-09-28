
#include "atSpawn.h++"


#ifdef _MSC_VER

   #include <stdio.h>

   atProcessHandle spawnProcess(char * execName, char * cmdParameters)
   {
      STARTUPINFO           si;
      char                  fullCmdLine[1024];
      PROCESS_INFORMATION   pi;

      // Initialize the start-up stuff
      memset(&si, 0, sizeof(si));
      si.cb = sizeof(si);

      // Make a string with the exec name and all the parameters
      sprintf(fullCmdLine, "%s %s", execName, cmdParameters);

      // Create the process
      if (CreateProcess(execName, fullCmdLine, NULL, NULL, false, 0,
                        NULL, NULL, &si, &pi) == 0)
      {
         // We failed to start the process for some reason so notify the
         // user and return 0 for the process handle
         printf("Failed to spawn application.\n");
         return (atProcessHandle) 0;
      }
      else
      {
         // Close the thread handle (we don't need it)
         CloseHandle(pi.hThread);

         // Return the process handle
         return pi.hProcess;
      }
   }

   int getProcessStatus(atProcessHandle p, int * exitCode)
   {
      BOOL   result;
      DWORD  code;

      // Get the process status
      result = GetExitCodeProcess(p, &code);
      if (result)
      {
         if (code == STILL_ACTIVE)
         {
            // Process is still running
            if (exitCode != NULL)
               *exitCode = 0;
            return 1;
         }
         else
         {
            // Process has ended
            if (exitCode != NULL)
               *exitCode = (int) code;
            return 0;
         }
      }
      else
      {
         // Couldn't get the status
         if (exitCode != NULL)
            *exitCode = -1;
         return -1;
      }
   }

   bool exitProcess(atProcessHandle p)
   {
      BOOL result;

      // Terminate the process (Windows doesn't have a "nice" way to stop a
      // remote process, so this call works the same as killProcess() )
      result =  TerminateProcess(p, -1);

      // Return the result of the terminate call.  A value of true only
      // means that the terminate call was carried out (use getProcessStatus()
      // to determine if the process has exited)
      return (bool) TerminateProcess(p, -1);
   }

   bool killProcess(atProcessHandle p)
   {
      BOOL result;

      // Terminate the process
      result =  TerminateProcess(p, -1);

      // Return the result of the terminate call.  A value of true only
      // means that the terminate call was carried out (use getProcessStatus()
      // to determine if the process has exited)
      return (bool) TerminateProcess(p, -1);
   }

#elif defined(__ANDROID__)

   // No process control on Android

#else

   #include <sys/types.h>
   #include <sys/wait.h>
   #include <unistd.h>
   #include <signal.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>

   atProcessHandle spawnProcess(char * execName, char * cmdParameters)
   {
      pid_t                 pid;
      char *                cmdLine;
      char *                lastToken;
      char *                params[1024];
      u_long                numParams;
      u_long                i;

      // We'll spawn by forking first and then having the child exec the
      // new process
      pid = fork();
      if (pid == 0)
      {
         // All the exec() calls in Linux want each parameter separated
         // out so build that up first
         cmdLine = (char *) malloc(strlen(cmdParameters) + 1);
         strcpy(cmdLine, cmdParameters);
         params[0] = execName;
         numParams = 1;
         params[numParams] = strtok_r(cmdLine, " ", &lastToken);
         while (params[numParams] != NULL)
         {
            // Increment the parameter count
            numParams++;

            // Get the next token
            params[numParams] = strtok_r(NULL, " ", &lastToken);
         }
         
         // Now call exec to start the new process!
         if (execv(execName, params) == -1)
         {
            // An error has occurred
            printf("Exec has failed!\n");
            perror("Exec");

            // Should clean-up here
            free(cmdLine);

            // Exit the forked process (we don't want two copies of our
            // original program running around).  Note that we use the
            // C99-standard _Exit function here (underscore and capital "E")
            // This exits the child process as quietly as possible,  without
            // calling any exit handlers, triggering signals, or flushing
            // inherited file descriptors, which is what we want in this
            // unusual case
            _Exit(1);
         }
      }
      else if (pid == -1)
      {
         // Failed so return zero for the PID
         printf("Failed to spawn application.\n");
         return 0;
      }
      else
      {
         // Return the pid of the spawned process
         return pid;
      }
   }

   int getProcessStatus(atProcessHandle p, int * exitCode)
   {
      pid_t   result;
      int     status;
      int     code;

      // Get the process status
      result = waitpid(p, &status, WNOHANG);

      // See if we got an exit code
      if (result == 0)
      {
         // The process is still running
         if (exitCode != NULL)
            *exitCode = 0;
         return 1;
      }
      else if (result < 0)
      {
         // Failed to get the status
         if (exitCode != NULL)
            *exitCode = 0;
         return -1;
      }
      else
      {
         // See if the process exited
         if (WIFEXITED(status))
         {
            // Check the status code
            if (status == 0)
            {
               // The process exited cleanly
               if (exitCode != NULL)
                  *exitCode = 0;
               return 0;
            }
            else
            {
               // The process exited normally, but returned a non-zero value
               if (exitCode != NULL)
                  *exitCode = WEXITSTATUS(status);
               return 0;
            }
         }
         else if (WIFSIGNALED(status))
         {
            // The process exited abnormally
            if (exitCode != NULL)
               *exitCode = WTERMSIG(status);
            return 0;
         }
         else
         {
            // The process isn't running, but it isn't terminated either
            // (unusual state).  We'll just call it terminated
            if (exitCode != NULL)
               *exitCode = 0;
            return 0;
         }
      }
   }

   bool exitProcess(atProcessHandle p)
   {
      int result;

      // Send the process a SIGTERM
      result = kill(p, SIGTERM);

      // Return the result.  A result of true only means that the signal was
      // sent (use getProcessStatus() to determine if the process has
      // exited)
      return (result == 0);
   }

   bool killProcess(atProcessHandle p)
   {
      int result;

      // Send the process a SIGKILL
      result = kill(p, SIGKILL);

      // Return the result.  A result of true only means that the signal was
      // sent (use getProcessStatus() to determine if the process has
      // exited)
      return (result == 0);
   }

#endif

