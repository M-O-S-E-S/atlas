
#include "atDynamic.h"


#ifdef _MSC_VER
   #include <windows.h>

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

   void * dlOpen(const char * filename, int flag)
   {
      char   tmp[1024];

      // Add the extension
      sprintf(tmp, "%s.so", filename);

      // Just call dlopen() and return what it does
      return dlopen(tmp, flag);
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


#endif

