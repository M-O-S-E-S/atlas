
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
   // Toss the strings
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
      // Create a string with this token
      newStr = new atString(&the_chars[string_index],
                            (u_long ) (minChar - &the_chars[string_index]));

      // Move the string index to after this location
      string_index = (u_long)(minChar - the_chars + 1);
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
         string_index = (u_long)strlen(the_chars);
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

