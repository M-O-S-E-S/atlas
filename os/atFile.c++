
#include "atFile.h++"


#ifdef _MSC_VER

   #include <Windows.h>
   #include <Strsafe.h>

   int listFiles(char * path, char ** results, int count)
   {
      char              searchTarget[MAX_PATH];
      HANDLE            searchHandle;
      WIN32_FIND_DATA   fileData;
      DWORD             errorCode;
      int               fileCount;
      int               lengthBytes;

      // Prepare the path string for use with FindFile functions. First, copy
      // the string to a buffer, then append '\*' to the directory name.
      StringCchCopy(searchTarget, MAX_PATH, path);
      StringCchCat(searchTarget, MAX_PATH, TEXT("\\*"));

      // Find the first file in the directory.
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
            //strcpy(results[fileCount], fileData.cFileName);
            StringCchCopy(results[fileCount], lengthBytes, fileData.cFileName);

            // Account for the file we just copied
            fileCount++;
         }
      }
      // Confirm that we haven't run out of file names or space to store them
      while ((FindNextFile(searchHandle, &fileData) != 0) &&
             (fileCount < count));

      // FindNextFile will always set an error code if it returns non-zero.
      // Check whether this is actually an error or whether we've just reached
      // the end of the list.
      errorCode = GetLastError();
      if (errorCode != ERROR_NO_MORE_FILES)
      {
         // This is a real error. Free the memory we've been allocating
         while (fileCount >= 0)
         {
            // Free the memory
            free(results[fileCount]);

            // Move down to the previous entry
            fileCount--;
         }

         //notify(AT_ERROR, "Error: %d\n", errorCode);

         // Indicate failure
         return -1;
      }

      // Close the search handle now that we're done with it
      FindClose(searchHandle);

      // Return the number of filenames we successfully placed in the array
      return fileCount;
   }

#else

   #include <glob.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>

   int listFiles(char * path, char ** results, int count)
   {
      char     targetPath[512];
      glob_t   fileList;
      int      fileCount;

      // Convert the path into the search pattern
      // TODO: Make sure the path is properly formated
      sprintf(targetPath, "%s/*", path);

      // Get a list of all the files in the target location
      glob(targetPath, 0, NULL, &fileList);

      // Initialize our result counter
      fileCount = 0;

      // Loop until we run out of files or out of space to store them
      while ((fileCount < count) &&
             (fileCount < (int)fileList.gl_pathc))
      {
         // Allocate a new string of this length
         results[fileCount] = (char *)
            malloc((strlen(fileList.gl_pathv[fileCount]) + 1) * sizeof(char));

         // Copy the file name into the string we just allocated
         strcpy(results[fileCount], fileList.gl_pathv[fileCount]);

         // Account for the file we just copied
         fileCount++;
      }

      // Free the original path list
      globfree(&fileList);

      // Return the number of files we found
      return fileCount;
   }

#endif

