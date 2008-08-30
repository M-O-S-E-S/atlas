
#include "atSpawn.h++"


#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <stdio.h>


   bool spawn(char * execName, char * cmdParameters)
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
         // user and return false
         printf("Failed to spawn application.\n");
         return false;
      }
      else
      {
         // Return true saying that we succeeded
         return true;
      }
   }
#else
   #include <sys/types.h>
   #include <unistd.h>
   #include "atStringTokenizer.h++"
   #include "atString.h++"

   bool spawn(char * execName, char * cmdParameters)
   {
      pid_t                 pid;
      atStringTokenizer *   tokenizer;
      atString*             tokens[1024];
      char *                params[1024];
      u_long                numParams;
      atString *            token;
      u_long                i;

      // We'll spawn by forking first and then having the child exec the
      // new process
      pid = fork();
      if (pid == 0)
      {
         // All the exec() calls in Linux want each parameter separated
         // out so build that up first
         tokenizer = new atStringTokenizer(atString(cmdParameters));
         params[0] = execName;
         numParams = 1;
         token = tokenizer->getToken(" ");
         while (token != NULL)
         {
            // Store a pointer to this parameter string
            tokens[numParams] = token;
            params[numParams] = token->getString();
            numParams++;

            // Get the next token
            token = tokenizer->getToken(" ");
         }
         
         // Clean-up and NULL terminate the parameter list
         if (tokenizer != NULL)
            delete tokenizer;
         params[numParams] = NULL;

         // Now call exec to start the new process!
         if (execv(execName, params) == -1)
         {
            // An error has occurred
            printf("Exec has failed!\n");
            perror("Exec");

            // Should clean-up here
            for (i=0; i < numParams; i++)
            {
               // Clean-up this token
               if (tokens[i] != NULL)
                  delete tokens[i];
            }
         }
      }
      else if (pid == -1)
      {
         // Failed so return false
         printf("Failed to spawn application.\n");
         return false;
      }
      else
      {
         // Succeeded so return true
         return true;
      }
   }
#endif

