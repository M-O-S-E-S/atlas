
#include "atDynamic.h++"
#include "atSymbols.h++"


#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>


   static char dlErrorMessage[1024] = "No Error";


   void * dlOpen(const char * filename, int flag)
   {
      HMODULE   moduleHandle;
      char      newFilename[1024];
      char *    ch;
      DWORD     dlError;

      // If the filename pointer is NULL, we return the local module
      if (filename == NULL)
      {
         moduleHandle = GetModuleHandle(NULL);
      }
      else
      {
         // We need to convert any forward slashes into backslashes for the
         // LoadLibrary function to work properly
         strcpy(newFilename, filename);

         // Now, go through the new string and replace all the '/' with '\\'
         ch = (char *) strchr(filename, '/');
         while (ch != NULL)
         {
            // Make it a backslash
            *ch = '\\';

            // Look for the next slash
            ch = (char *) strchr(filename, '/');
         }

         // Now add the extension
         strcat(newFilename, ".dll");

         // Finally, call the Win32 LoadLibrary function to try to load the
         // specified dll
         moduleHandle = LoadLibrary(newFilename);
      }

      // If there was a problem, get the error code and message and store
      // them for later retrieval (using dlerror)
      if (moduleHandle == NULL)
      {
         // Get the error code
         dlError = GetLastError();

         // Get the nicely formatted error message from the system
         FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dlError, 0,
                       dlErrorMessage, sizeof(dlErrorMessage)-1, NULL);
      }

      // Return the handle as a void * (it may be NULL)
      return (void *) moduleHandle;
   }


   char * dlError()
   {
      // Return the error
      return dlErrorMessage;
   }


   void * dlSym(void * handle, const char * symbol)
   {
      FARPROC  symbolAddr;
      DWORD    dlError;

      // Use the Win32 function GetProcAddress to look up the symbol
      symbolAddr = GetProcAddress((HMODULE ) handle, symbol);

      // If there was a problem, get the error code and message and store
      // them for later retrieval (using dlerror)
      if (symbolAddr == NULL)
      {
         // Get the error code
         dlError = GetLastError();

         // Get the nicely formatted error message from the system
         FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dlError, 0,
                       dlErrorMessage, sizeof(dlErrorMessage)-1, NULL);
      }

      // Return the symbol address as a void * (it may be NULL)
      return (void *) symbolAddr;
   }


   int dlClose(void * handle)
   {
      DWORD  dlError;

      // Try to free the DLL
      if (FreeLibrary((HMODULE ) handle) == FALSE)
      {
         // Get the error code
         dlError = GetLastError();

         // Get the nicely formatted error message from the system
         FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dlError, 0,
                       dlErrorMessage, sizeof(dlErrorMessage)-1, NULL);

         // Return non-zero to indicate failure
         return 1;
      }
      else
      {
         // Return zero to indicate success
         return 0;
      }
   }
#else
   #include <dlfcn.h>
   #include <stdio.h>
   #include <string.h>

   void * dlOpen(const char * filename, int flag)
   {
      char *   slash;
      char     tmp[1024];

      // Handle when filename is NULL (meaning we're opening the "local"
      // app
      if (filename == NULL)
      {
         // Just call dlopen() and return what it does
         return dlopen(NULL, flag);
      }
      else
      {
         // Add the extension, but handle directory names so look for a slash
         slash = (char *) strrchr(filename, '/');
         if (slash != NULL)
         {
            // We found the last slash so copy everything up to that point and
            // then add the rest (this is not straight-forward because we
            // have to insert the "lib" between the directory and the
            // SO filename)
            strncpy(tmp, filename, slash - filename + 1);
            strcat(tmp, "lib");
            strcat(tmp, slash + 1);
            strcat(tmp, ".so");
         }
         else
         {
            // No slash so just make a standard filename
            sprintf(tmp, "lib%s.so", filename);
         }

         // Just call dlopen() and return what it does
         return dlopen(tmp, flag);
      }
   }


   char * dlError()
   {
      // Just call dlerror() and return what it does
      return dlerror();
   }


   void * dlSym(void * handle, const char * symbol)
   {
      // Just call dlsym() and return what it does
      return dlsym(handle, symbol);
   }


   int dlClose(void * handle)
   {
      // Just call dlclose() and return what it does
      return dlclose(handle);
   }
#endif

