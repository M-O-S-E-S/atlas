
#include <string.h>
#include "atString.h++"

#include "atStringBuffer.h++"

atString::atString()
{
   // Initialize string to not being allocated
   local_string = NULL;
   string_length = 0;

   // Set a default string
   setString("\0");
}


atString::atString(const char * stringToCopy)
{
   // Initialize string to not being allocated
   local_string = NULL;
   string_length = 0;

   // Copy the string into our string
   setString(stringToCopy);
}


atString::atString(const char * stringToCopy, u_long maxLength) 
{
   // Initialize string to not being allocated
   local_string = NULL;
   string_length = 0;

   // Copy the string into our string
   setString(stringToCopy, maxLength); 
}


atString::atString(const atString & stringToCopy)
{
   // Initialize string to not being allocated
   local_string = NULL;
   string_length = 0;

   // Copy the contents of the other string
   setString(stringToCopy.getString());
}


atString::~atString()
{
   // Get rid of the local string if it exists
   if (local_string != NULL)
   {
      free(local_string);
      local_string = NULL;
   }
}


atString atString::clone() const
{
   return atString(local_string);
}


void atString::append(const atString & stringToAppend)
{
   u_long   lengthOfNewString;
   char *   oldString;

   // If we don't have a local string already, just set the string to
   // the given string
   if (local_string == NULL)
   {
      setString(stringToAppend);
   }
   else if (stringToAppend.getString() != NULL)
   {
      // Get the combined length of the current string and the new string
      lengthOfNewString = (u_long ) (strlen(local_string) + 
                                     strlen(stringToAppend.getString()));

      // Keep a copy of the old string until we're done
      oldString = local_string;

      // Allocate space for the combined string (adding one to the length to
      // include the \0 character)
      local_string = (char *) calloc((lengthOfNewString + 1), sizeof(char));

      // Make sure we allocated space okay
      if (local_string != NULL)
      {
         // We did so copy the existing string
         strcpy(local_string, oldString);

         // Concatenate the appending string
         strcat(local_string, stringToAppend.getString());

         // Update the length
         string_length = lengthOfNewString;

         // Free the old string
         free(oldString);
      }
      else
      {
         // Failed to allocate memory, so just keep the old string
         local_string = oldString;

         // Notify the user of the failure
         notify(AT_ERROR, "Unable to append string (not enough memory)\n");
      }
   }
}


atString atString::concat(const atString & stringToConcat) const
{
   atString result;

   // Create a new string with the contents of this string
   result.setString(local_string);

   // Append the contents of the given string
   result.append(stringToConcat);

   // Return the result
   return result;
}


void atString::setString(const char * stringToCopy)
{
   u_long   lengthOfNewString;

   // If we had a string stored already, get rid of it
   if (local_string != NULL)
   {
      free(local_string);
      local_string = NULL;
   }

   // Make sure there is something to copy
   if (stringToCopy != NULL)
   {
      // Get the length of the new string
      lengthOfNewString = (u_long ) strlen(stringToCopy);

      // Here we add one to the length to include the \0 character
      local_string = (char *) calloc((lengthOfNewString + 1), sizeof(char));

      // Make sure we allocated space okay
      if (local_string != NULL)
      {
         // We did so copy the string in
         strcpy(local_string, stringToCopy);
         string_length = lengthOfNewString;
      }
   }
   else
   {
      // We received a NULL string (even though we shouldn't have) so
      // just set an empty string
      local_string = (char *) calloc(1, sizeof(char));
      if (local_string != NULL)
         local_string[0] = '\0';
      string_length = 0;
   }
}


void atString::setString(const char * stringToCopy, u_long maxLength)
{
   u_long   lengthOfNewString;

   // Handle the case where there are fewer characters than given
   if (strlen(stringToCopy) < maxLength)
      lengthOfNewString = (u_long ) strlen(stringToCopy);
   else
      lengthOfNewString = maxLength;

   // If we had a string stored already, get rid of it
   if (local_string != NULL)
   {
      free(local_string);
      local_string = NULL;
   }

   // Make sure there is something to copy
   if (stringToCopy != NULL)
   {
      // Here we add one to the length to include the \0 character
      local_string = (char *) calloc((size_t ) (lengthOfNewString + 1), 
                                     sizeof(char));

      // Make sure we allocated space okay
      if (local_string != NULL)
      {
         // We did so copy the string in
         strncpy(local_string, stringToCopy, lengthOfNewString);
         local_string[lengthOfNewString] = '\0';
         string_length = lengthOfNewString;
      }
   }
   else
   {
      // We received a NULL string (even though we shouldn't have) so
      // just set an empty string
      local_string = (char *) calloc(1, sizeof(char));
      if (local_string != NULL)
         local_string[0] = '\0';
      string_length = 0;
   }
}


void atString::setString(const atString & stringToCopy)
{
   // Call setString() with the contents of the string to copy
   setString(stringToCopy.getString());
}


char * atString::getString() const
{
   // Return the string stored
   return local_string;
}


char atString::getCharAt(u_long index) const
{
   // Return the NULL char if the index is out of bounds or if the 
   // string itself is null; otherwise, return the requested character
   if ( (index >= string_length) || (local_string == NULL) )
      return '\0';
   else
      return local_string[index];
}


u_long atString::getLength() const
{
   // Return the length of the string
   return string_length;
}


bool atString::equals(atItem * otherItem)
{
   atString *   strItem;

   // Try to convert it to a string to make sure it is a string
   strItem = dynamic_cast<atString *>(otherItem);

   // Return whether the two strings are equal or not
   if ( (strItem != NULL) && (strcmp(strItem->getString(), local_string) == 0) )
      return true;
   else
      return false;
}


int atString::compare(atItem * otherItem)
{
   atString *   strItem;

   // Try to cast to an atString
   strItem = dynamic_cast<atString *>(otherItem);

   // See if the other item is valid
   if (strItem != NULL)
   {
      // Return the string comparison of the two native strings
      return strcmp(local_string, strItem->getString());
   }
   else
   {
      // Return the default atItem comparison
      return atItem::compare(otherItem);
   }
}


void atString::replaceAll(char * stringToReplace, char * replacementString)
{
   char *             subString;
   atStringBuffer *   buffer;
   int                index;
   char *             stringToAppend;
   atString *         copyString; 

   // Create a copy of the local string
   copyString = new atString(local_string);

   // Start a buffer that will hold the final string
   buffer = new atStringBuffer("");

   // Get the first occurence of the string to replace from the copy of 
   // our local string
   subString = strstr(copyString->getString(), stringToReplace);

   // Check that we still have an occurence of the string being replaced
   while (subString != NULL)
   {
      // Find the index where the string to be replaced starts, we do this
      // by subtracting the memory address of the copied string from the
      // current location inside of the copied string, then dividing by 
      // the size of a character byte
      index = (subString - copyString->getString()) / sizeof(char);

      // Create memory for a temporary string that will hold all the 
      // characters before the string being replaced was found
      stringToAppend = (char *) calloc((index + 1), sizeof(char));

      // Copy the starting characters into the temporary string, append the
      // string to the buffer, and append the replacement string
      strncpy(stringToAppend, copyString->getString(), index);
      buffer->append(stringToAppend);
      buffer->append(replacementString);

      // Move the copied string to the end of the replaced string
      cpy = cpy->subString(index + strlen(stringToReplace));

      // Find another occurence of the string to be replaced
      subString = strstr(copyString->getString(), stringToReplace);

      // Free the memory we declared for the temporary string
      free(stringToAppend);
   }

   // Append the end of the string to the buffer
   buffer->append(copyString->getString());

   // Change the local string to the string that has the string replaced
   setString(buffer->getString());

   // Clean up the buffer
   delete buffer;
}


atString * atString::subString(int start)
{
   // Return the substring starting at location start that includes the 
   // remainder of the string
   return subString(start, strlen(local_string));
}


atString * atString::subString(int start, int end)
{
   int      newLength;
   char *   newString;
   int      i;

   // Find the length of the substring
   newLength = end + 1 - start;

   // Allocate memory for the substring
   newString = (char *) calloc(newLength, sizeof(char));

   // Run through the strings starting at start and copy each character 
   // into the substring
   for (i = start; i <= end; i++)
   {
      newString[i - start] = local_string[i];
   }

   // Return the substring
   return new atString(newString);
}


atString & atString::operator=(const atString & stringToCopy)
{
   // Avoid setting the string in the case of a self-assignment
   if (this != &stringToCopy)
   {
      // Copy the string from the given atString
      setString(stringToCopy);
   }

   // Return this object to allow for chaining assignment
   return *this;
}

