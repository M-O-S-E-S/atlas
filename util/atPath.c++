
#include "atPath.h++"

#include "atFile.h++"


bool atPath::createDirectory(atString path)
{
   return ::createDirectory(path.getString());
}


atList * atPath::listFiles(atString path)
{
   atList *     resultList;
   char *       fileList[65536];
   int          fileCount;
   int          currentFile;
   atString *   currentString;

   // Create a new list to contain the results
   resultList = new atList();

   // Fetch the list of files from the lower-level atFile method
   fileCount = ::listFiles(path.getString(), fileList, 65536);
      
   // Iterate across our results and add them to the list we just created
   for (currentFile = 0; currentFile < fileCount; currentFile++)
   {
      // Wrap the result in an atString
      currentString = new atString(fileList[currentFile]);

      // Free the pointer that was allocated to hold this string
      free(fileList[currentFile]);

      // Add the new atString to the list of results
      resultList->addEntry(currentString);
   }

   // Return the list of results.
   return resultList;
}


atString atPath::getFilenameFromPath(atString path)
{
   char     pathBuffer[512];
   char     delimiters[128];
   char *   currentToken;
   char *   nextToken;

   // Copy the string to a new buffer. We need to do this because we're about
   // to use strtok which is destructive to its input string
   strcpy(pathBuffer, path.getString());

   // Pull the first token, split by the delimiter
   currentToken = strtok(pathBuffer, DELIMITER_STRING);

   // Keep processing until we reach the last token
   nextToken = strtok(NULL, DELIMITER_STRING);
   while (nextToken)
   {
      // Move the current token forward
      currentToken = nextToken;

      // Query the next token
      nextToken = strtok(NULL, DELIMITER_STRING);
   }

   // The next token is NULL, which means we have no more delimiters. This
   // should mean that our current token contains only the file name.
   return atString(currentToken);
}


atString atPath::getExtension(atString path, atString delimiter)
{
   int    delimiterLength;
   int    pathLength;
   char *   pathString;
   int    pathIndex;

   // Fetch the length of the delimiter
   delimiterLength = delimiter.getLength();

   // If the delimiter is an empty string, return at empty string
   if (delimiterLength == 0)
      return atString();

   // Fetch the internal string, because we're going to be performing indexing
   // operations directly on it
   pathString = path.getString();

   // Calculate the length the input string so we can search from the end
   pathLength = strlen(pathString);

   // Search back from the end of the string, searching for the first delimiter
   for (pathIndex = pathLength - delimiterLength; pathIndex >= 0; pathIndex--)
   {
      // Test whether the current index contains the delimiter
      if (strncmp(&(pathString[pathIndex]), delimiter.getString(),
          delimiterLength) == 0)
      {
         // We've found the last instance of the delimiter in the string.
         // Return the substring beginning after the end of this delimiter.
         return atString(pathString + delimiterLength);
      }
   }

   // If we've reached this point, then we didn't find a single instance of the
   // delimiter in the input path. Return an empty string
   return atString();
}


atString atPath::getFilenameWithoutExtension(atString path, atString delimiter)
{
   int    delimiterLength;
   int    pathLength;
   char   pathBuffer[512];
   int    pathIndex;

   // Fetch the length of the delimiter
   delimiterLength = delimiter.getLength();

   // If the delimiter is an empty string, return the whole path unmodified
   if (delimiterLength == 0)
      return path;

   // Fetch the length of the input string so we can search from the end
   pathLength = strlen(path.getString());

   // Copy the path string to a new buffer. We need to do this because we're
   // going to alter the path for return.
   strcpy(pathBuffer, path.getString());

   // Search back from the end of the string, searching for the first delimiter
   for (pathIndex = pathLength - delimiterLength; pathIndex > 0; pathIndex--)
   {
      // Test whether the current index contain
      if (strncmp(&(pathBuffer[pathIndex]), delimiter.getString(),
          delimiterLength) == 0)
      {
         // We've found the last instance of the delimiter in the string.
         // Truncate the path buffer at this point and return.
         pathBuffer[pathIndex] = '\0';
         return atString(pathBuffer);
      }
   }

   // If we've reached this point, then we didn't find a single instance of the
   // delimiter in the input path. Return the input path intact.
   return path;
}

