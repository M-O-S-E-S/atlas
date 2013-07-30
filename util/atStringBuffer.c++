
#include <string.h>
#include "atStringBuffer.h++"


atStringBuffer::atStringBuffer()
{
   // Initialize string buffer to starting size
   buffer_size = 256;
   local_buffer = (char *) calloc((size_t) buffer_size, sizeof(char));
   buffer_tail = local_buffer;

   // Set a default string
   setString("\0");
}


atStringBuffer::atStringBuffer(const char * stringToCopy) 
{
   // Initialize string to not being allocated
   buffer_size = 256;
   local_buffer = (char *) calloc((size_t) buffer_size, sizeof(char));
   buffer_tail = local_buffer;

   // Copy the string into our string
   setString(stringToCopy); 
}


atStringBuffer::atStringBuffer(const char * stringToCopy, u_long maxLength) 
{
   // Initialize string to not being allocated
   buffer_size = 256;
   local_buffer = (char *) calloc((size_t) buffer_size, sizeof(char));
   buffer_tail = local_buffer;

   // Copy the string into our string
   setString(stringToCopy, maxLength); 
}


atStringBuffer::~atStringBuffer()
{
   // Get rid of the local string if it exists
   if (local_buffer != NULL)
   {
      free(local_buffer);
   }

   // Clear the buffer 
   buffer_size = 0;
   local_buffer = NULL;
   buffer_tail = NULL;
}


atStringBuffer atStringBuffer::clone() const
{
   return atStringBuffer(local_buffer);
}


void atStringBuffer::append(const atString & stringToAppend)
{
   // Call the other form
   append(stringToAppend.getString());
}


atStringBuffer atStringBuffer::concat(const atString & stringToConcat) const
{
   // Call the other form
   return concat(stringToConcat.getString());
}


void atStringBuffer::append(const char * stringToAppend)
{
   u_long   bufferLength;
   u_long   appendLength;
   u_long   need;
   u_long   newBufferSize;
   u_long   offset;

   // Get the current length of the buffer as well as the length of the
   // string to append
   bufferLength = buffer_tail - local_buffer;
   appendLength = strlen(stringToAppend);

   // Make sure we have enough space to append this string
   need = (u_long ) (bufferLength + appendLength + 1);
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
      buffer_tail = local_buffer + bufferLength;
      buffer_size = newBufferSize;
   }

   // Then append the string
   if (stringToAppend != NULL)
   {
      // Append the string
      strcpy(buffer_tail, stringToAppend);

      // Update the buffer tail pointer
      buffer_tail += appendLength;
   }
}


atStringBuffer atStringBuffer::concat(const char * stringToConcat) const
{
   atStringBuffer   result;

   // Create a new string with the contents of this string
   result.setString(local_buffer);

   // Append the contents of the given string
   result.append(stringToConcat);

   // Return the result
   return result;
}


void atStringBuffer::append(char charToAppend)
{
   char   str[2];

   // Create a string with only the character we want to append
   str[0] = charToAppend;
   str[1] = 0;

   // Call the append method for strings
   append(str);
}


atStringBuffer atStringBuffer::concat(char charToConcat) const
{
   atStringBuffer   result;

   // Create a new string with the contents of this string
   result.setString(local_buffer);

   // Append the character to the new string
   result.append(charToConcat);

   // Return the result
   return result;
}


void atStringBuffer::setString(const char * stringToCopy)
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
      buffer_tail = local_buffer;
   }

   // Make sure there is something to copy
   if (stringToCopy != NULL)
   {
      // There is so copy the string in
      strcpy(local_buffer, stringToCopy);

      // Update the buffer tail pointer to point to the end of the string
      buffer_tail = local_buffer + need - 1;
   }
   else
   {
      // We received a NULL string (even though we shouldn't have) so
      // just set an empty string
      local_buffer[0] = '\0';
      buffer_tail = local_buffer;
   }
}


void atStringBuffer::setString(const char * stringToCopy, u_long maxLength)
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
      buffer_tail = local_buffer;
   }

   // Make sure there is something to copy
   if (stringToCopy != NULL)
   {
      // There is so copy the string in
      strncpy(local_buffer, stringToCopy, (size_t ) need);
      local_buffer[need] = '\0';

      // Update the buffer tail pointer to point to the end of the string
      buffer_tail = local_buffer + need;
   }
   else
   {
      // We received a NULL string (even though we shouldn't have) so
      // just set an empty string
      local_buffer[0] = '\0';
      buffer_tail = local_buffer;
   }
}


void atStringBuffer::setString(const atString & stringToCopy)
{
   // Call setString() with the contents of the string to copy
   setString(stringToCopy.getString());
}


char * atStringBuffer::getString() const
{
   // Return the string stored
   return local_buffer;
}


atString atStringBuffer::getAsString() const
{
   // Return an atString with buffer
   return atString(local_buffer);
}


char atStringBuffer::getCharAt(u_long index) const
{
   // Return the NULL char if the index is out of bounds or if the 
   // string itself is null; otherwise, return the requested character
   if ( (index >= (buffer_tail - local_buffer)) || (index < 0) || 
        (local_buffer == NULL) )
      return '\0';
   else
      return local_buffer[index];
}


u_long atStringBuffer::getLength() const
{
   // Return the length of the string
   return (u_long ) (buffer_tail - local_buffer);
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


atStringBuffer & atStringBuffer::operator=(const atString & stringToCopy)
{
   // Copy the string from the given atString
   setString(stringToCopy);

   // Return this object to allow for chaining assignment
   return *this;
}


atStringBuffer & atStringBuffer::operator=(
   const atStringBuffer & stringBufferToCopy)
{
   // Avoid setting the string in the case of a self-assignment
   if (this != &stringBufferToCopy)
   {
      // Copy the string from the given atString
      setString(stringBufferToCopy.getString());
   }

   // Return this object to allow for chaining assignment
   return *this;
}

