
#include "atCommandLine.h++"


atCommandLine::atCommandLine()
{
   int          i;

   // Set our notifier name
   setName("[CommandLine] ");
}


atCommandLine::atCommandLine(atString optionsSpec)
{
   // Set our notifier name
   setName("[CommandLine] ");

   // Set the valid options
   parseValidOptions(optionsSpec);
}


atCommandLine::atCommandLine(int argc, char ** argv, atString optionsSpec)
{
   // Set our notifier name
   setName("[CommandLine] ");

   // Set the valid options
   parseValidOptions(optionsSpec);

   // parse the command line
   parseCommandLine(argc, argv);
}


atCommandLine::~atCommandLine()
{
}


void atCommandLine::parseValidOptions(atString optionsSpec)
{
   char *       opts;
   char *       optPtr;
   char         optionStr[2];
   atString *   option;
   atString *   takesValue;

   // Get the options specifier as a primitive string, so we can more easily
   // process it
   opts = optionsSpec.getString();
   optPtr = opts;

   // If the first character in the options specifier is a colon, skip it
   // (unlike getopt(), we'll always report missing option arguments)
   if (*optPtr == ':')
      optPtr++;

   // Iterate over the specifier string and add the valid options to the
   // map
   while (*optPtr != 0)
   {
      // Make sure this is a letter or digit
      if (((*optPtr >= 'a') && (*optPtr <= 'z')) ||
          ((*optPtr >= 'A') && (*optPtr <= 'Z')) ||
          ((*optPtr >= '0') && (*optPtr <= '9')))
      {
         // This is a valid option character
         optionStr[0] = *optPtr;
         optionStr[1] = 0;
         option = new atString(optionStr);
         optPtr++;

         // See if this option takes a value
         if (*optPtr == ':')
         {
            takesValue = new atString("1");
            optPtr++;
         }
         else
            takesValue = new atString("0");

         // Add the option to the valid options
         valid_options.addEntry(option, takesValue);
      }
      else
      {
         // This is an invalid option character
         notify(AT_WARN, "Invalid option character: '%c'\n", *optPtr);
         optPtr++;

         // If this invalid option was supposed to take a value, skip
         // the following colon as well
         if (*optPtr == ':')
            optPtr++;
      }
   }
}


bool atCommandLine::takesValue(atString option)
{
   atString *   takesValue;

   // Look up the option in the valid options map
   if (valid_options.containsKey(&option))
   {
      // Get the value and convert it to a bool
      takesValue = (atString *) valid_options.getValue(&option);
      if (atoi(takesValue->getString()) == 0)
         return false;
      else
         return true;
   }
   else
   {
      // Default to no value
      return false;
   }
}


bool atCommandLine::parseOption(char * arg, char * nextArg)
{
   char         optStr[2];
   atString *   newArg;
   atString *   newOpt;
   atString *   newValue;
   char *       optPtr;

   // Create an option string for this option
   optStr[0] = arg[0];
   optStr[1] = 0;
   newOpt = new atString(optStr);

   // See if this is a valid option
   if (valid_options.containsKey(newOpt))
   {
      // See if this option takes a value
      if (takesValue(*newOpt))
      {
         // See if there are any remaining characters in this argument
         if (arg[1] == 0)
         {
            // Grab the value from the next argument (making sure the next
            // argument isn't also an option)
            if ((nextArg != NULL) && (nextArg[0] != '-'))
            {
               // Create the value and add the option and value to the map
               newValue = new atString(nextArg);
               options_map.addEntry(newOpt, newValue);

               // We're done processing this argument, as well as the next
               // one, so return true
               return true;
            }
            else
            {
               // This option requires a value, but none is specified
               notify(AT_WARN, "Missing argument for option '-%c'\n",
                      optStr[0]);

               // Toss the option and return false, since we didn't
               // consume the next argument
               return false;
            }
         }
         else
         {
            // Grab the value from the rest of this argument and add the
            // option and value to the map
            newValue = new atString(&arg[1]);
            options_map.addEntry(newOpt, newValue);

            // Return false, since we didn't consume the next argument
            return false;
         }
      }
      else
      {
         // Add this option to the map, with an empty string for the value
         options_map.addEntry(newOpt, new atString(""));

         // See if there are other options in this argument
         if (arg[1] != 0)
         {
            // Call this method recursively to process any remaining options
            optPtr = &arg[1];
            return parseOption(optPtr, nextArg);
         }
         else
         {
            // Return false, since we haven't consumed the next argument
            return false;
         }
      }
   }
   else
   {
      // Argument isn't a recognized option, so ignore it
      notify(AT_WARN, "Unrecognized option: -%c\n", optStr[0]);
      delete newOpt;

      // See if there are other options in this argument
      if (arg[1] != 0)
      {
         // Call this method recursively to process any remaining options
         optPtr = &arg[1];
         return parseOption(optPtr, nextArg);
      }
      else
      {
         // Return false, since we haven't consumed the next argument
         return false;
      }
   }
}


void atCommandLine::setValidOptions(atString optionsSpec)
{
   // Clear the valid options map
   valid_options.clear();

   // Parse the new options from the specification string
   parseValidOptions(optionsSpec);
}


void atCommandLine::addValidOption(char option, bool takesValue)
{
   char         optStr[2];
   atString *   newOpt;
   atString *   newValue;

   // Make sure this is a letter or digit
   if (((option >= 'a') && (option <= 'z')) ||
       ((option >= 'A') && (option <= 'Z')) ||
       ((option >= '0') && (option <= '9')))
   {
      // This is a valid option character
      optStr[0] = option;
      optStr[1] = 0;
      newOpt = new atString(optStr);

      // See if this option takes a value
      if (takesValue)
         newValue = new atString("1");
      else
         newValue = new atString("0");

      // Add the option to the valid options
      valid_options.addEntry(newOpt, newValue);
   }
   else
   {
      // This is an invalid option character
      notify(AT_WARN, "Invalid option character: '%c'\n", option);
   }
}


void atCommandLine::parseCommandLine(int argc, char ** argv)
{
   char *       opts;
   int          i;
   bool         endOpts;
   char *       arg;

   // Parse the options from the arguments list
   i = 0;
   endOpts = false;
   while ((i < argc) && (!endOpts))
   {
      // Search for an option in this argument
      if (argv[i][0] == '-')
      {
         // Check for a '--' option, which indicates the end of the options
         if ((argv[i][1] == '-') && (argv[i][2] == 0))
         {
            // Set the end of options flag.  The remaining arguments will
            // be processed as non-option arguments
            endOpts = true;
         }
         else if (argv[i][1] == '-')
         {
            // TODO: Should we handle GNU-style long options, as in
            //       "--option=value"?

            // Argument isn't a recognized option, so ignore it
            notify(AT_WARN, "Unrecognized option: %s\n", argv[i]);
         }
         else if (argv[i][1] == 0)
         {
            // This is a '-' character by itself.  Unix typically treats
            // this as a non-option argument, so we will too
            args_list.addEntry(new atString(argv[i]));
         }
         else
         {
            // Parse this argument as an option ("-o val" or "-oval").  Note
            // that this argument may turn into several options ("-abD").
            // We also need to pass the next argument, in case an option
            // in this argument takes a value that wasn't specified as
            // part of this argument ("-o val")
            if (i+1 < argc)
            {
               // Pass the next argument, in case we need it for a value
               if (parseOption(&argv[i][1], argv[i+1]))
               {
                  // We consumed the next argument already, so increment
                  // the loop index an extra time
                  i++;
               }
            }
            else
            {
               // There isn't a next argument, so pass NULL
               parseOption(&argv[i][1], NULL);
            }
         }
      }
      else
      {
         // Treat this argument as a non-option argument, and add it to the
         // list
         args_list.addEntry(new atString(argv[i]));
      }

      // Next argument
      i++;
   } 

   // If we didn't get through all of the arguments (i.e.: a "--" option
   // was specified), process the remaining arguments as non-option arguments
   for (i; i < argc; i++)
   {
      // Add the argument to the arguments list
      args_list.addEntry(new atString(argv[i]));
   }
}


bool atCommandLine::hasOption(atString opt)
{
   // Return whether the given option was specified on the command line
   return options_map.containsKey(&opt);
}


atString atCommandLine::getOptionValue(atString opt)
{
   atString * valueStr;

   // Return the value associated with the given option
   valueStr = (atString *) options_map.getValue(&opt);
   if (valueStr == NULL)
      return atString("");
   else
      return *valueStr;
}


atList * atCommandLine::getOptionList()
{
   atList       keyList;
   atString *   option;
   atList *     newList;
   atString *   optionCopy;

   // Create a copy of the options specified on the command line (without
   // any option values)
   options_map.getSortedList(&keyList, NULL);
   newList = new atList();
   option = (atString *) keyList.getFirstEntry();
   while (option != NULL)
   {
      // Add a copy of this option to the new list
      optionCopy = new atString(*option);
      newList->addEntry(optionCopy);

      // Remove this option from the key list, so it isn't deleted when
      // the list goes out of scope
      keyList.removeCurrentEntry();

      // Next (first) option
      option = (atString *) keyList.getFirstEntry();
   }

   // Return the copied list
   return newList;
}


atList * atCommandLine::getArgumentList()
{
   atList *     newList;
   atString *   arg;
   atString *   argCopy;

   // Create a copy of the list of non-option arguments on the command line
   newList = new atList();
   arg = (atString *) args_list.getFirstEntry();
   while (arg != NULL)
   {
      // Add a copy of this option to the new list
      argCopy = new atString(*arg);
      newList->addEntry(argCopy);

      // Next argument
      arg = (atString *) args_list.getNextEntry();
   }

   // Return the copied list
   return newList;
}

