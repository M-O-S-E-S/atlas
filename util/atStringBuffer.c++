
#include <string.h>
#include "atStringBuffer.h++"


atStringBuffer::atStringBuffer()
{
   // Initialize string buffer to starting size
   buffer_size = 256;
   local_buffer = (char *) calloc((size_t) buffer_size, sizeof(char));

   // Set a default string
   setString("\0");
}


atStringBuffer::atStringBuffer(char * stringToCopy) 
{
   // Initialize string to not being allocated
   buffer_size = 256;
   local_buffer = (char *) calloc((size_t) buffer_size, sizeof(char));

   // Copy the string into our string
   setString(stringToCopy); 
}


atStringBuffer::atStringBuffer(char * stringToCopy, u_long maxLength) 
{
   // Initialize string to not being allocated
   buffer_size = 256;
   local_buffer = (char *) calloc((size_t) buffer_size, sizeof(char));

   // Copy the string into our string
   setString(stringToCopy, maxLength); 
}


atStringBuffer::~atStringBuffer()
{
   // Get rid of the local string if it exists
   if (local_buffer != NULL)
   {
      free(local_buffer);
      local_buffer = NULL;
   }
}


atStringBuffer atStringBuffer::clone()
{
   return atStringBuffer(local_buffer);
}


void atStringBuffer::append(const atString & stringToAppend)
{
   // Call the other form
   append((char *) stringToAppend.getString());
}


atStringBuffer atStringBuffer::concat(const atString & stringToConcat)
{
   // Call the other form
   return concat((char *) stringToConcat.getString());
}


void atStringBuffer::append(char * stringToAppend)
{
   u_long   need;
   u_long   newBufferSize;

   // First, make sure we have enough space to append this string
   need = (u_long ) (strlen(local_buffer) + strlen(stringToAppend) + 1);
   if (need >= buffer_size)
   {
      // Make more space if we need it
      newBufferSize = buffer_size;
      while (need >= newBufferSize)
      {
         // Double the size we have now
         newBufferSize *= 2;
      }

      // Expand the buffer
      local_buffer = (char *) realloc(local_buffer, (size_t) newBufferSize);
      buffer_size = newBufferSize;
   }

   // Then append the string
   if (stringToAppend != NULL)
   {
      // Append the string
      strcat(local_buffer, stringToAppend);
   }
}


atStringBuffer atStringBuffer::concat(char * stringToConcat)
{
   atStringBuffer   result;

   // Create a new string with the contents of this string
   result.setString(local_buffer);

   // Append the contents of the given string
   result.append(stringToConcat);

   // Return the result
   return result;
}


void atStringBuffer::setString(char * stringToCopy)
{
   u_long   need;
   u_long   newBufferSize;

   // First, make sure we have enough space to append this string
   need = (u_long ) (strlen(stringToCopy) + 1);
   if (need >= buffer_size)
   {
      // Make more space if we need it
      newBufferSize = buffer_size;
      while (need >= newBufferSize)
      {
         // Double the size we have now
         newBufferSize *= 2;
      }

      // Expand the buffer
      local_buffer = (char *) realloc(local_buffer, (size_t) newBufferSize);
      buffer_size = newBufferSize;
   }

   // Make sure there is something to copy
   if (stringToCopy != NULL)
   {
      // There is so copy the string in
      strcpy(local_buffer, stringToCopy);
   }
   else
   {
      // We received a NULL string (even though we shouldn't have) so
      // just set an empty string
      local_buffer[0] = '\0';
   }
}


void atStringBuffer::setString(char * stringToCopy, u_long maxLength)
{
   u_long   need;
   u_long   newBufferSize;

   // Handle the case where there are fewer characters than given
   if (strlen(stringToCopy) < maxLength)
      need = (u_long ) strlen(stringToCopy);
   else
      need = maxLength;

   // First, make sure we have enough space to append this string
   if (need >= buffer_size)
   {
      // Make more space if we need it
      newBufferSize = buffer_size;
      while (need >= newBufferSize)
      {
         // Double the size we have now
         newBufferSize *= 2;
      }

      // Expand the buffer
      local_buffer = (char *) realloc(local_buffer, (size_t) newBufferSize);
      buffer_size = newBufferSize;
   }

   // Make sure there is something to copy
   if (stringToCopy != NULL)
   {
      // There is so copy the string in
      strncpy(local_buffer, stringToCopy, (size_t ) need);
      local_buffer[need] = '\0';
   }
   else
   {
      // We received a NULL string (even though we shouldn't have) so
      // just set an empty string
      local_buffer[0] = '\0';
   }
}


void atStringBuffer::setString(atString & stringToCopy)
{
   // Call setString() with the contents of the string to copy
   setString(stringToCopy.getString());
}


char * atStringBuffer::getString()
{
   // Return the string stored
   return local_buffer;
}


atString atStringBuffer::getAsString()
{
   // Return an atString with buffer
   return atString(local_buffer);
}


char atStringBuffer::getCharAt(u_long index)
{
   // Return the NULL char if the index is out of bounds or if the 
   // string itself is null; otherwise, return the requested character
   if ( (index >= strlen(local_buffer)) || (index < 0) || 
        (local_buffer == NULL) )
      return '\0';
   else
      return local_buffer[index];
}


u_long atStringBuffer::getLength()
{
   // Return the length of the string
   return (u_long ) strlen(local_buffer);
}


bool atStringBuffer::equals(atItem * otherItem)
{
   atString *   strItem;

   // Try to convert it to a string to make sure it is a string
   strItem = dynamic_cast<atString *>(otherItem);

   // Return whether the two strings are equal or not
   if ( (strItem != NULL) && (strcmp(strItem->getString(), local_buffer) == 0) )
      return true;
   else
      return false;
}


int atStringBuffer::compare(atItem * otherItem)
{
   atString *   strItem;

   // Try to cast to an atString
   strItem = dynamic_cast<atString *>(otherItem);

   // See if the other item is valid
   if (strItem != NULL)
   {
      // Return the string comparison of the two native strings
      return strcmp(local_buffer, strItem->getString());
   }
   else
   {
      // Return the default atItem comparison
      return atItem::compare(otherItem);
   }
}


void atStringBuffer::operator=(atString stringToCopy)
{
   // Copy the string from the given atString
   setString(stringToCopy);
}

