
#include <string.h>
#include "atString.h++"


atString::atString()
{
   // Initialize string to not being allocated
   local_string = NULL;
   string_length = 0;

   // Set a default string
   setString("\0");
}


atString::atString(char * stringToCopy) 
{
   // Initialize string to not being allocated
   local_string = NULL;
   string_length = 0;

   // Copy the string into our string
   setString(stringToCopy); 
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


atString atString::clone()
{
   return atString(local_string);
}


void atString::setString(const char * stringToCopy)
{
   int   lengthOfNewString;

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
      lengthOfNewString = strlen(stringToCopy);

      // If there is actually a string, allocate space and copy it in
      if (lengthOfNewString > 0)
      {
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


char * atString::getString()
{
   // Return the string stored
   return local_string;
}


const char * atString::getString() const
{
   // Return the string stored
   return (const char *)local_string;
}


char atString::getCharAt(int index)
{
   // Return the NULL char if the index is out of bounds or if the 
   // string itself is null; otherwise, return the requested character
   if ( (index >= string_length) || (index < 0) || (local_string == NULL) )
      return '\0';
   else
      return local_string[index];
}


int atString::getLength()
{
   // Return the length of the string
   return string_length;
}


bool atString::equals(atItem * otherItem)
{
   atString *   strItem;

   // Check the type for Jason's nasty equal checking
   strItem = dynamic_cast<atString *>(otherItem);

   // Return whether the two strings are equal or not
   if ( (strItem != NULL) && (strcmp(strItem->getString(), local_string) == 0) )
      return true;
   else
      return false;
}


void atString::operator=(atString stringToCopy)
{
   // Copy the string from the given atString
   setString(stringToCopy);
}
