
#include "atFile.h++"


#define AT_FILE_NUM_PERSONAL_PATHS   9

static char *   atPersonalPaths[AT_FILE_NUM_PERSONAL_PATHS] = { 0 };


#ifdef _MSC_VER

   #include <Windows.h>
   #include <Strsafe.h>
   #include <Shlobj.h>
   #include <sys/types.h>
   #include <sys/stat.h>
   #include <wchar.h>


   bool isDirectory(char * path)
   {
      struct stat   fileStat;

      // Make sure we have a path at all
      if (path == NULL)
      {
         // No path is not a directory
         return false;
      }

      // Make sure the target on this path exists
      if (access(path, F_OK) != 0)
      {
         // The target does not exist thus it can't be a directory
         return false;
      }

      // Get the file information
      stat(path, &fileStat);

      // Check if the file is a directory and return accordingly
      if (fileStat.st_mode & _S_IFDIR)
         return true;
      else
         return false;
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


   void initPersonalPaths()
   {
      HRESULT     result;
      char *      dir;
      wchar_t *   wbuffer;
      char        buffer[MAX_PATH];

      // Get the home directory (the profile directory)
      result = SHGetKnownFolderPath(FOLDERID_Profile, 0, NULL, &wbuffer);
      if (result != S_OK)
      {
         // Try to read the environment for USERPROFILE or HOME instead
         dir = getenv("USERPROFILE");
         if (dir == NULL)
            dir = getenv("HOME");

         // Failing this, use the Windows directory (not ideal, but we
         // have to call some place our home)
         if (dir == NULL)
         {
            if (GetWindowsDirectory(buffer, MAX_PATH) > 0)
               dir = &buffer[0];
         }

         // If this still fails (very unlikely), use the root of the
         // C: drive
         if (dir == NULL)
         {
            sprintf(buffer, "C:\\");
            dir = &buffer[0];
         }

         // Store the home directory
         atPersonalPaths[AT_PATH_HOME] = (char *) malloc(strlen(dir) + 1);
         strcpy(atPersonalPaths[AT_PATH_HOME], dir);
      }
      else
      {
         // Store the home directory
         wcstombs(buffer, wbuffer, MAX_PATH);
         CoTaskMemFree(wbuffer);
         atPersonalPaths[AT_PATH_HOME] = 
            (char *) malloc(strlen(buffer) + 1);
         strcpy(atPersonalPaths[AT_PATH_HOME], buffer);
      }

      // Get the remaining path locations (if these fail, we'll just leave
      // them NULL), first the desktop
      SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &wbuffer);
      if (wbuffer != NULL)
      {
         wcstombs(buffer, wbuffer, MAX_PATH);
         CoTaskMemFree(wbuffer);
         atPersonalPaths[AT_PATH_DESKTOP] = 
            (char *) malloc(strlen(buffer) + 1);
         strcpy(atPersonalPaths[AT_PATH_DESKTOP], buffer);
      }

      // Find the Documents path
      SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &wbuffer);
      if (wbuffer != NULL)
      {
         wcstombs(buffer, wbuffer, MAX_PATH);
         CoTaskMemFree(wbuffer);
         atPersonalPaths[AT_PATH_DOCUMENTS] = 
            (char *) malloc(strlen(buffer) + 1);
         strcpy(atPersonalPaths[AT_PATH_DOCUMENTS], buffer);
      }

      // Find the Downloads path
      SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &wbuffer);
      if (wbuffer != NULL)
      {
         wcstombs(buffer, wbuffer, MAX_PATH);
         CoTaskMemFree(wbuffer);
         atPersonalPaths[AT_PATH_DOWNLOADS] = 
            (char *) malloc(strlen(buffer) + 1);
         strcpy(atPersonalPaths[AT_PATH_DOWNLOADS], buffer);
      }

      // Find the Music path
      SHGetKnownFolderPath(FOLDERID_Music, 0, NULL, &wbuffer);
      if (wbuffer != NULL)
      {
         wcstombs(buffer, wbuffer, MAX_PATH);
         CoTaskMemFree(wbuffer);
         atPersonalPaths[AT_PATH_MUSIC] = 
            (char *) malloc(strlen(buffer) + 1);
         strcpy(atPersonalPaths[AT_PATH_MUSIC], buffer);
      }

      // Find the Pictures path
      SHGetKnownFolderPath(FOLDERID_Pictures, 0, NULL, &wbuffer);
      if (wbuffer != NULL)
      {
         wcstombs(buffer, wbuffer, MAX_PATH);
         CoTaskMemFree(wbuffer);
         atPersonalPaths[AT_PATH_PICTURES] = 
            (char *) malloc(strlen(buffer) + 1);
         strcpy(atPersonalPaths[AT_PATH_PICTURES], buffer);
      }

      // Find the Public path
      SHGetKnownFolderPath(FOLDERID_Public, 0, NULL, &wbuffer);
      if (wbuffer != NULL)
      {
         wcstombs(buffer, wbuffer, MAX_PATH);
         CoTaskMemFree(wbuffer);
         atPersonalPaths[AT_PATH_PUBLIC] = 
            (char *) malloc(strlen(buffer) + 1);
         strcpy(atPersonalPaths[AT_PATH_PUBLIC], buffer);
      }

      // Find the Templates path
      SHGetKnownFolderPath(FOLDERID_Templates, 0, NULL, &wbuffer);
      if (wbuffer != NULL)
      {
         wcstombs(buffer, wbuffer, MAX_PATH);
         CoTaskMemFree(wbuffer);
         atPersonalPaths[AT_PATH_TEMPLATES] = 
            (char *) malloc(strlen(buffer) + 1);
         strcpy(atPersonalPaths[AT_PATH_TEMPLATES], buffer);
      }

      // Find the Videos path
      SHGetKnownFolderPath(FOLDERID_Videos, 0, NULL, &wbuffer);
      if (wbuffer != NULL)
      {
         wcstombs(buffer, wbuffer, MAX_PATH);
         CoTaskMemFree(wbuffer);
         atPersonalPaths[AT_PATH_VIDEOS] = 
            (char *) malloc(strlen(buffer) + 1);
         strcpy(atPersonalPaths[AT_PATH_VIDEOS], buffer);
      }
   }

   char * getPersonalPath(atPersonalPathKey key)
   {
      // Initialize the personal path array, if necessary
      if (atPersonalPaths[AT_PATH_HOME] == NULL)
         initPersonalPaths();

      // Return the requested path (may be NULL if it doesn't exist on this
      // system)
      return atPersonalPaths[key];
   }

#else

   #include <glob.h>
   #include <pwd.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <unistd.h>
   #include <sys/stat.h>
   #include <sys/types.h>


   bool isDirectory(char * path)
   {
      struct stat   fileStat;

      // Make sure we have a path at all
      if (path == NULL)
      {
         // No path is not a directory
         return false;
      }

      // Make sure the target on this path exists
      if (access(path, F_OK) != 0)
      {
         // The target does not exist thus it can't be a directory
         return false;
      }

      // Get the file information
      stat(path, &fileStat);

      // Check if the file is a directory and return accordingly
      if (fileStat.st_mode & S_IFDIR)
         return true;
      else
         return false;
   }


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
      char *   ptr;

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
         currentToken = strtok_r(fileList.gl_pathv[fileCount],
                                 pathDelimiter, &ptr);

         // Keep attempting to fetch the next token so long as there is one
         nextToken = strtok_r(NULL, pathDelimiter, &ptr);
         while (nextToken)
         {
            // Move the current token forward
            currentToken = nextToken;

            // Query the next token
            nextToken = strtok_r(NULL, pathDelimiter, &ptr);
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


   void initPersonalPaths()
   {
      struct passwd *   pw;
      char *            dir;
      FILE *            fp;
      char              buffer[1024];
      char              line[1024];
      char *            ptr1;
      char *            ptr2;
      char *            key;
      char *            path;
      int               enumKey;

      // Get the user's home directory, preferably by checking the 
      // authentication database
      pw = getpwuid(getuid());
      if (pw != NULL)
      {
         dir = pw->pw_dir;
      }
      else
      {
         // Grab the home directory from the environment
         dir = getenv("HOME");
         if (dir == NULL)
         {
            // Just use the temporary directory
            dir = TMP_DIR;
         }
      }

      // Copy the home directory
      atPersonalPaths[AT_PATH_HOME] = (char *) malloc(strlen(dir) + 1);
      strcpy(atPersonalPaths[AT_PATH_HOME], dir);

      // Now, open the XDG configuration file (this specifies the location
      // of personal data directories in Unix)
      dir = getenv("XDG_CONFIG_DIR");
      if (dir == NULL)
         dir = atPersonalPaths[AT_PATH_HOME];
      sprintf(buffer, "%s/.config/user-dirs.dirs", dir);
      fp = fopen(buffer, "r");

      // Parse the file and get the directories
      if (fp != NULL)
      {
         while (fgets(line, sizeof(line), fp))
         {
            // Skip comments
            if ((line[0] != '#') && (strlen(line) > 0))
            {
               // Initialize
               key = NULL;
               path = NULL;

               // Trim newline
               ptr1 = strchr(line, '\n');
               if (ptr1 != NULL)
                  *ptr1 = 0;

               // Get the directory key
               ptr1 = &line[0];
               ptr2 = strchr(line, '=');

               // No '=' means an invalid line
               if (ptr2 != NULL)
               {
                  // Back up from the '=' one space
                  ptr2--;

                  // Trim whitespace
                  while ((*ptr1 == ' ') || (*ptr1 == '\t'))
                     ptr1++;
                  while ((*ptr2 == ' ') || (*ptr2 == '\t'))
                     ptr2--;

                  // NULL-terminate the key (we'll keep the key and path in
                  // place, like strtok does with its tokens)
                  ptr2++;
                  *ptr2 = 0;
                  key = ptr1;

                  // Now, parse the path.  Look for the first '"' (required
                  // by XDG standard)
                  ptr1 = ptr2+1;
                  ptr1 = strchr(ptr1, '"');
                  if (ptr1 != NULL)
                  {
                     // Path starts one character past the '"'
                     path = ptr1+1;

                     // Look for the end
                     ptr2 = strchr(path, '"');
                     if (ptr2 != NULL)
                     {
                        // NULL-terminate the path here
                        *ptr2 = 0;
                     }
                     else
                     {
                        // Invalid format, so no path
                        path = NULL;
                     }
                  }
               }

               // See if we parsed out the key and path OK
               if ((key != NULL) && (path != NULL))
               {
                  // Translate the key to one of our enumerated keys
                  if (strcmp(key, "XDG_DESKTOP_DIR") == 0)
                  {
                     enumKey = AT_PATH_DESKTOP;
                  }
                  else if (strcmp(key, "XDG_DOWNLOAD_DIR") == 0)
                  {
                     enumKey = AT_PATH_DOWNLOADS;
                  }
                  else if (strcmp(key, "XDG_TEMPLATES_DIR") == 0)
                  {
                     enumKey = AT_PATH_TEMPLATES;
                  }
                  else if (strcmp(key, "XDG_PUBLICSHARE_DIR") == 0)
                  {
                     enumKey = AT_PATH_PUBLIC;
                  }
                  else if (strcmp(key, "XDG_DOCUMENTS_DIR") == 0)
                  {
                     enumKey = AT_PATH_DOCUMENTS;
                  }
                  else if (strcmp(key, "XDG_MUSIC_DIR") == 0)
                  {
                     enumKey = AT_PATH_MUSIC;
                  }
                  else if (strcmp(key, "XDG_PICTURES_DIR") == 0)
                  {
                     enumKey = AT_PATH_PICTURES;
                  }
                  else if (strcmp(key, "XDG_VIDEOS_DIR") == 0)
                  {
                     enumKey = AT_PATH_VIDEOS;
                  }
                  else
                  {
                     enumKey = -1;
                  }

                  // See if we got a valid key
                  if (enumKey >= 0)
                  {
                     // See if this is a relative path
                     if (path[0] == '$')
                     {
                        // The path is relative.  By standard, it must be
                        // relative to the user's home directory.  First,
                        // strip the $HOME token away
                        ptr1 = strchr(path, '/');
                        if (ptr1 == NULL)
                        {
                           // Just use the home directory itself
                           sprintf(buffer, "%s",
                                   atPersonalPaths[AT_PATH_HOME]);
                        }
                        else
                        {
                           // Combine the home directory with the rest of the
                           // path
                           path = ptr1+1;
                           sprintf(buffer, "%s%c%s",
                                   atPersonalPaths[AT_PATH_HOME],
                                   DIRECTORY_SEPARATOR, path);
                        }

                        // Allocate and store the path
                        atPersonalPaths[enumKey] = (char *)
                           malloc(strlen(buffer) + 1);
                        strcpy(atPersonalPaths[enumKey], buffer);
                     }
                     else
                     {
                        // The path is absolute, so just copy it as is
                        atPersonalPaths[enumKey] = (char *)
                           malloc(strlen(path) + 1);
                        strcpy(atPersonalPaths[enumKey], path);
                     }
                  }
               }
            }
         }

         // Close the file
         fclose(fp);
      }
   }


   char * getPersonalPath(atPersonalPathKey key)
   {
      // Initialize the personal path array, if necessary
      if (atPersonalPaths[AT_PATH_HOME] == NULL)
         initPersonalPaths();

      // Return the requested path (may be NULL if it wasn't defined in the
      // XDG configuration file)
      return atPersonalPaths[key];
   }


#endif

