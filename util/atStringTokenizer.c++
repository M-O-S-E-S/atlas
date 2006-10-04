
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
   char         token[4096];
   atString *   newStr;

   // Go through the delimiters and find the "earliest" one in
   // the string so far
   minChar = strchr(&the_chars[string_index], delimiters[0]);
   for (i=1; i < strlen(delimiters); i++)
   {
      // Get the first instance of the current delimiter
      ch = strchr(&the_chars[string_index], delimiters[i]);

      // See if this is earlier than the best so far and save if so
      if ( (ch != NULL) && (ch < minChar) )
         minChar = ch;
   }

   // Prepare the token
   if (minChar != NULL)
   {
      // Copy the token out
      strncpy(token, &the_chars[string_index], minChar - the_chars);

      // Move the string index to after this location
      string_index = minChar - the_chars + 1;

      // Create a string with this token
      newStr = new atString(token);
   }
   else
      newStr = NULL;

   // Return the new string
   return newStr;
}

