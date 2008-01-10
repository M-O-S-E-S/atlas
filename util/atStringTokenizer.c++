
#include <string.h>
#include "atStringTokenizer.h++"


atStringTokenizer::atStringTokenizer(atString str)
{
   // Keep the string
   the_string = new atString(str);
   the_chars = the_string->getString();

   // Initialize where we are in tokenizing the string
   string_index = 0;
}


atStringTokenizer::~atStringTokenizer()
{
   // Toss the string
   if (the_string != NULL)
      delete the_string;
}


atString * atStringTokenizer::getToken(atString delimiters)
{
   // Just call the other method
   return getToken(delimiters.getString());
}


atString * atStringTokenizer::getToken(char delimiters[])
{
   char *       minChar;
   u_long       i;
   char *       ch;
   char *       token;
   atString *   newStr;

   // First, if we're currently at a delimiter, we need to skip
   // them
   while ( (string_index < strlen(the_chars)) &&
           (strchr(delimiters, the_chars[string_index]) != NULL) )
   {
      // Move up the index
      string_index++;
   }

   // Go through the delimiters and find the "earliest" one in
   // the string so far
   minChar = strchr(&the_chars[string_index], delimiters[0]);
   for (i=1; i < strlen(delimiters); i++)
   {
      // Get the first instance of the current delimiter
      ch = strchr(&the_chars[string_index], delimiters[i]);

      // See if this is earlier than the best so far and save if so
      if ( ((ch != NULL) && (ch < minChar)) || (minChar == NULL) )
         minChar = ch;
   }

   // Prepare the token
   if (minChar != NULL)
   {
      // Copy the token out, not forgetting to add a NULL
      token = (char *) calloc(minChar - &the_chars[string_index] + 1,
                              sizeof(char));
      if (token != NULL)
      {
         strncpy(token, &the_chars[string_index], 
                 minChar - &the_chars[string_index]);
         token[minChar - &the_chars[string_index]] = '\0';
      }

      // Move the string index to after this location
      string_index = minChar - the_chars + 1;

      // Create a string with this token
      newStr = new atString(token);

      // Clean-up memory
      if (token != NULL)
         free(token);
   }
   else
   {
      // We didn't find a delimiter but check if we have bytes left
      // over in the string (we just need to return this last token)
      if (string_index < strlen(the_chars))
      {
         // We had a substring left so we have an implicit delimiter
         // of the NULL character (\0)
         newStr = new atString(&the_chars[string_index]);
         string_index = strlen(the_chars);
      }
      else
      {
         // We're just at the end of the string so return no token!
         newStr = NULL;
      }
   }

   // Return the new string
   return newStr;
}

