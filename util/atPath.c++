
// ATLAS: Adaptable Tool Library for Advanced Simulation
//
// Copyright 2015 University of Central Florida
//
//
// This library provides many fundamental capabilities used in creating
// virtual environment simulations.  It includes elements such as vectors,
// matrices, quaternions, containers, communication schemes (UDP, TCP, DIS,
// HLA, Bluetooth), and XML processing.  It also includes some extensions
// to allow similar code to work in Linux and in Windows.  Note that support
// for iOS and Android development is also included.
//
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include "atPath.h++"

#include "atFile.h++"
#include "atStringBuffer.h++"
#include "atStringTokenizer.h++"


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


atString atPath::getDirectoryFromPath(atString path)
{
   int    delimiterLength;
   int    pathLength;
   char   pathBuffer[512];
   int    pathIndex;

   // Fetch the length of the delimiter
   delimiterLength = strlen(DELIMITER_STRING);

   // Fetch the length of the input string so we can search from the end
   pathLength = path.getLength();

   // Copy the path string to a new buffer. We need to do this because we're
   // going to alter the path for return.
   strcpy(pathBuffer, path.getString());

   // Search back from the end of the string, searching for the first delimiter
   for (pathIndex = pathLength - delimiterLength; pathIndex > 0; pathIndex--)
   {
      // Test for each delimiter character
      if (strchr(DELIMITER_STRING, pathBuffer[pathIndex]) != NULL)
      {
         // We've found the last delimiter in the string. Truncate the path
         // buffer at this point and return it.
         pathBuffer[pathIndex] = '\0';
         return atString(pathBuffer);
      }
   }

   // If we've reached this point, then we didn't find a single instance of the
   // delimiter in the input path. Return the input path intact.
   return path;
}


atString atPath::getFilenameFromPath(atString path)
{
   atStringTokenizer *   pathTokens;
   atString *            currentToken;
   atString *            nextToken;
   atString              result;

   // If the path is empty, just return it
   if (path.getLength() == 0)
      return path;

   // Tokenize the path, and get the first two tokens from it (basically,
   // we look ahead one token in order to find the last one)
   pathTokens = new atStringTokenizer(path);
   currentToken = pathTokens->getToken(DELIMITER_STRING);
   nextToken = pathTokens->getToken(DELIMITER_STRING);
   while (nextToken)
   {
      // Move the current token forward and ditch the old one
      delete currentToken;
      currentToken = nextToken;

      // Query the next token
      nextToken = pathTokens->getToken(DELIMITER_STRING);
   }

   // The next token is NULL, which means we have no more delimiters. This
   // should mean that our current token contains only the file name.
   result.setString(*currentToken);

   // Clean up
   delete currentToken;
   delete pathTokens;

   // Return the result
   return result;
}


atString atPath::normalize(atString path)
{
   char                  char1;
   char                  char2;
   char                  char3;
   atStringBuffer        head;
   char *                workStr;
   atString              workPath;
   atStringTokenizer *   pathTokens;
   atString *            pathElement;
   int                   skip;
   atList                pathList;
   atList                resultList;
   atStringBuffer        resultBuffer;

   // Deal with invalid paths
   if (path.getLength() == 0)
      return path;

   // If the path indicates the current directory, just return it as-is
   if ((path.getLength() == 1) && (atString(".").equals(&path)))
      return path;

   // Get the first three characters of the path (if they exist)
   char1 = path.getString()[0];
   if (path.getLength() > 1)
      char2 = path.getString()[1];
   else
      char2 = 0;
   if (path.getLength() > 2)
      char3 = path.getString()[2];
   else
      char3 = 0;

   // We don't handle Windows UNC paths, since it's impossible to do so
   // in a cross-platform way
   if ((char1 == '\\') && (char2 == '\\'))
      return path;

   // Check for a path delimiter or drive letter at the front
   if (strchr(DELIMITER_STRING, char1) != NULL)
   {
      // Just put the correct path delimiter at the front
      head.append(DIRECTORY_SEPARATOR);

      // Copy the remaining path to work on
      workStr = path.getString();
      workPath = atString(&workStr[1]);
   }
   else if ((((char1 >= 'a') && (char1 <= 'z')) ||
             ((char1 >= 'A') && (char1 <= 'Z'))) &&
            (char2 == ':'))
   {
      // Path starts with a drive letter, so use that as the head
      head.append(char1);
      head.append(char2);

      // Check if the path after the drive letter is absolute or relative
      if (strchr(DELIMITER_STRING, char3) != NULL)
      {
         // Append a separator after the drive letter
         head.append(DIRECTORY_SEPARATOR);

         // Copy the remaining path (after the separator) to work on
         workStr = path.getString();
         workPath = atString(&workStr[3]);
      }
      else
      {
         // Copy the remaining path (after the drive letter and colon)
         // to work on
         workStr = path.getString();
         workPath = atString(&workStr[2]);
      }
   }
   else
   {
      // Work on the entire path
      workPath = path;
   }

   // Tokenize the path using the delimiter string
   pathTokens = new atStringTokenizer(workPath);

   // Fetch the tokens into a list
   while ((pathElement = pathTokens->getToken(DELIMITER_STRING)) != NULL)
      pathList.addEntry(pathElement);

   // Done with the tokenizer
   delete pathTokens;

   // Traverse the list from right to left and deal with any "." and ".."
   // elements
   skip = 0;
   pathElement = (atString *) pathList.getLastEntry();
   while (pathElement != NULL)
   {
      if (atString("..").equals(pathElement))
      {
         // Skip the next element
         skip++;
      }
      else if (atString(".").equals(pathElement))
      {
         // Just ignore this element
      }
      else if (skip > 0)
      {
         // Decrement the skip counter (we've resolved a ".." at this point)
         skip--;
      }
      else
      {
         // Add this element to the result path
         resultList.addEntry(new atString(*pathElement));
      }

      // Next element
      pathElement = (atString *) pathList.getPreviousEntry();
   }

   // Write out any leading absolute path elements that we found
   resultBuffer.append(head.getString());

   // If there were any ".." elements that weren't resolved during the
   // normalization process, write those first
   while (skip > 0)
   {
      resultBuffer.append("..");
      resultBuffer.append(DIRECTORY_SEPARATOR);
      skip--;
   }

   // Now, write out the rest of the resulting path (note that we also need
   // to read this path back out from right to left, since we constructed
   // it in reverse order in the first place)
   pathElement = (atString *) resultList.getLastEntry();
   while (pathElement != NULL)
   {
      // Append the path element
      resultBuffer.append(*pathElement);

      // Check the next path element now (if we're currently at the end of
      // the list, we don't want to put another separator at the end of the
      // path
      pathElement = (atString *) resultList.getPreviousEntry();

      // Append a directory separator if appropriate
      if (pathElement != NULL)
         resultBuffer.append(DIRECTORY_SEPARATOR);
   }

   // Return the resulting path
   return resultBuffer.getAsString();
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
         return atString(pathString + pathIndex + delimiterLength);
      }
   }

   // Couldn't find an extension
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

