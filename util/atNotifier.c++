
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "atNotifier.h++"


// Define the "notify level" (we make this a class variable so that
// altering the level will affect all those calling notify())
int atNotifier::notify_level = AT_INFO;


atNotifier::atNotifier()
{
   // Initialize the instance variables (an empty header and output to stdout)
   memset(header_name, 0, sizeof(header_name));
   output_file_ptr = stdout;
}


atNotifier::~atNotifier()
{
}


void atNotifier::setName(char * name)
{
   // Copy the name into the header (use strncpy to avoid writing over
   // the end of the array)
   strncpy(header_name, name, sizeof(header_name) - 1);

   // Guarantee a NULL character (covers the case when strncpy does copy
   // all "n" characters)
   header_name[sizeof(header_name) - 1] = '\0';
}


void atNotifier::setNotifyLevel(int desiredLevel)
{
   // Make sure the user chose a valid desired level
   if ( (desiredLevel == AT_INFO) || (desiredLevel == AT_WARN) ||
        (desiredLevel == AT_ERROR) || (desiredLevel == AT_FATAL_ERROR) ||
        (desiredLevel == AT_SILENT) )
   {
      // Set the level to the requested level
      notify_level = desiredLevel;
   }
   else
   {
      // Otherwise, output a warning about the illegal level
      notify(AT_WARN, "Attempt to set illegal notify level [%04x].\n", 
             desiredLevel);
   }
}


void atNotifier::setNotifyDestination(FILE *output)
{
   // Set the output to the FILE given by the user (note that this class
   // does not open nor close the file)
   output_file_ptr = output;
}


void atNotifier::notify(int level, char *str, ...)
{
   va_list   args;

   // When in SILENT level, *never* notify
   if (notify_level & AT_SILENT)
      return;

   // Output if debug or no_header flag is set or if level is greater than 
   // or equal to the current notify level
   if ( (level & AT_DEBUG) || (level & AT_FORCE) ||
        ((level & AT_LEVEL_MASK) >= notify_level) )
   { 
      // If the "no header" bit is not set, output the header
      if (!(level & AT_NO_HEADER))
      {
         // Clear all modification flags
         level = level & AT_LEVEL_MASK;

         // Print the initial level as appropriate
         switch (level)
         {
            case AT_INFO: 
               fprintf(output_file_ptr, "INFO: ");
               break;
            case AT_WARN:
               fprintf(output_file_ptr, "WARN: ");
               break;
            case AT_ERROR:
               fprintf(output_file_ptr, "ERROR: ");
               break;
            case AT_FATAL_ERROR:
               fprintf(output_file_ptr, "\n\nFATAL_ERROR: ");
               break;
            default:
               fprintf(output_file_ptr,"**** UNKNOWN ERROR LEVEL [%d] ****", 
                       level);
               break; 
         }

         // Then print the header name
         fprintf(output_file_ptr, header_name);
      }

      // Finally print the message itself with parameters (like printf)
      va_start(args, str);
      vfprintf(output_file_ptr, str, args);
      va_end(args);

      // If the user called this a fatal error, bail out harshly
      if ((level & AT_FATAL_ERROR) > 0)
      {
         // AT_FATAL_ERROR exits
         exit(AT_FATAL_ERROR);
      }
   }
}

