
#include "atFile.h++"


#ifdef _MSC_VER

   #include <Windows.h>
   #include <Strsafe.h>


   int chdir(const char * path)
   {
      // Change to the directory
      ChDir(path);
   }


   bool createDirectory(char * path)
   {
      // Attempt to create the directory (passing NULL as the second parameter
      // means that we will use the security and permission profile of the
      // calling user)
      if (CreateDirectory(path, NULL))
      {
         // The operation succeeded
         return true;
      }

      // Indicate failure
      return false;
   }


   int listFiles(char * path, char ** results, int count)
   {
      char              searchTarget[MAX_PATH];
      HANDLE            searchHandle;
      WIN32_FIND_DATA   fileData;
      DWORD             errorCode;
      int               fileCount;
      int               lengthBytes;

      // Prepare the path string for use with FindFile functions (first, copy
      // the string to a buffer, then append '\*' to the directory name)
      StringCchCopy(searchTarget, MAX_PATH, path);
      StringCchCat(searchTarget, MAX_PATH, TEXT("\\*"));

      // Find the first file in the directory
      searchHandle = FindFirstFile(searchTarget, &fileData);
      if (searchHandle == INVALID_HANDLE_VALUE)
      {
         // errorCode = GetLastError();
         return -1;
      }
 
      // Indicate that we haven't yet found any files
      fileCount = 0;

      // Proceed until our condition is met
      do
      {
         // Check whether the name is part of the list
         if (((fileData.dwFileAttributes) & (FILE_ATTRIBUTE_DIRECTORY)) == 0)
         {
            // Allocate a new string of this length
            lengthBytes = strlen(fileData.cFileName) + 1;
            results[fileCount] = (char *)malloc(lengthBytes * sizeof(char));

            // Copy the file name into the string we just allocated
            StringCchCopy(results[fileCount], lengthBytes, fileData.cFileName);

            // Account for the file we just copied
            fileCount++;
         }
      }
      // Confirm that we haven't run out of file names or space to store them
      while ((FindNextFile(searchHandle, &fileData) != 0) &&
             (fileCount < count));

      // FindNextFile will always set an error code if it returns non-zero
      // (check whether this is actually an error or whether we've just reached
      // the end of the list)
      errorCode = GetLastError();
      if (errorCode == ERROR_NO_MORE_FILES)
      {
         // Close the search handle now that we're done with it
         FindClose(searchHandle);

         // Return the number of filenames we successfully placed in the array
         return fileCount;
      }
      else
      {
         // Close the search handle now that we're done with it (this may fail
         // if the handle is invalid, but there's little we can do about that)
         FindClose(searchHandle);

         // This is a real error (so free the memory we've been allocating)
         while (fileCount >= 0)
         {
            // Free the memory
            free(results[fileCount]);

            // Move down to the previous entry
            fileCount--;
         }

         // Indicate failure
         return -1;
      }
   }

#else

   #include <glob.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <sys/stat.h>
   #include <sys/types.h>


   bool createDirectory(char * path)
   {
      // Attempt to create the directory (0755 is the octal value for the
      // desired permissions: -rwxr-xr-x)
      if (mkdir(path, 0755) == 0)
      {
         // The operation succeeded
         return true;
      }

      // Indicate failure
      return false;
   }


   int listFiles(char * path, char ** results, int count)
   {
      char     targetPath[512];
      glob_t   fileList;
      int      fileCount;
      char *   currentToken;
      char *   nextToken;
      char     pathDelimiter[16];

      // Convert the path into the search pattern
      // TODO: Make sure the path is properly formatted
      sprintf(targetPath, "%s/*", path);

      // Get a list of all the files in the target location
      glob(targetPath, 0, NULL, &fileList);

      // Initialize our result counter
      fileCount = 0;

      // Print the directory separator to the path delimiter string
      sprintf(pathDelimiter, "%c", DIRECTORY_SEPARATOR);

      // Loop until we run out of files or out of space to store them
      while ((fileCount < count) &&
             (fileCount < (int ) fileList.gl_pathc))
      {
         // We want to take only the files themselves, but the vector contains
         // paths as well (tokenizing on the delimiter string will allow us to
         // extract only the filename. Begin by pulling off the first token)
         currentToken = strtok(fileList.gl_pathv[fileCount], pathDelimiter);

         // Keep attempting to fetch the next token so long as there is one
         nextToken = strtok(NULL, pathDelimiter);
         while (nextToken)
         {
            // Move the current token forward
            currentToken = nextToken;

            // Query the next token
            nextToken = strtok(NULL, pathDelimiter);
         }

         // The next token is NULL, which means there are no more delimiters,
         // which means that our current token contains only the file name so
         // allocate a new string of this length.
         results[fileCount] = (char *)
            malloc((strlen(currentToken) + 1) * sizeof(char));

         // Copy the file name into the string we just allocated
         strcpy(results[fileCount], currentToken);

         // Account for the file we just copied
         fileCount++;
      }

      // Free the original path list
      globfree(&fileList);

      // Return the number of files we found
      return fileCount;
   }

#endif

