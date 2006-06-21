
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "atConfigFile.h++"


//*************************** CONSTRUCTOR/DESTRUCTOR *************************//


atConfigFile::atConfigFile(char *filename)
{
   FILE *          infile;
   char            line[255];
   char *          lineToken;
   atTupleKey *    key;
   atTupleHead *   head;
   char *          arg;

   // Initialize the tuple list to be empty
   tuple_list = NULL;
   tuple_list_end = NULL;

   // Open the config file
   if ( (infile = fopen(filename, "r")) == NULL )
   {
      // Warn the user if we failed to open the list
      notify(AT_WARN, "Can't open configuration file!\n");
   }
   else
   {
      // Go through the file
      while (!feof(infile))
      {
         // Get a line and process it (if we weren't at the end of file)
         fgets(line, sizeof(line), infile);
         if (!feof(infile))
         {
            // Get a token from the line
            lineToken = getToken(line);

            // If we received something and it's not the comment character (#)
            if ( (lineToken != NULL) && (strncmp(lineToken, "#", 1) != 0) )
            {
               // Find the tuple of this input line.  Create one if needed.
               key = findKey(lineToken);
               if (key == NULL)
                  key = addKey(lineToken);

               // Add in a new argument tuple to the list of the key
               head = addTuple(key);

               // Loop while arguments exist
               arg = getToken(NULL);
               while (arg != NULL)
               {
                  // Add argument to argument tuple
                  addArgument(head, arg);

                  // Get next argument
                  arg = getToken(NULL);
               }
            }
         }
      }

      // Close file
      fclose(infile);
   }
}


atConfigFile::~atConfigFile()
{
   atTupleKey *        currentKey;
   atTupleHead *       currentHead;
   atTupleArgument *   currentArg;
   atTupleKey *        oldKey;
   atTupleHead *       oldHead;
   atTupleArgument *   oldArg;

   // Go through all the keys in the list
   currentKey = tuple_list;
   while (currentKey != NULL)
   {
      // Go through all the arguments of the key
      currentHead = currentKey->tupleList;
      while (currentHead != NULL)
      {
          // Go through all the parameters of the argument
          currentArg = currentHead->argumentList;
          while (currentArg != NULL)
          {
             // Remove the argument from the list
             oldArg = currentArg;
             currentArg = currentArg->next;

             // Deallocate the memory for the argument
             free(oldArg);
          }

          // Remove the argument list from the key tuple
          oldHead = currentHead;
          currentHead = currentHead->next;

          // Deallocate the memory for the argument list
          free(oldHead);
      }

      // Remove the key from the tuple list
      oldKey = currentKey;
      currentKey = currentKey->next;

      // Deallocate the memory of the key
      free(oldKey);
   }
}


//************************ PROTECTED MEMBER FUNCTIONS ************************//


char * atConfigFile::getToken(char *buffer)
{
   char *   lineToken;
   char *   lineToken2;

   // Get the token
   lineToken = strtok(buffer, " \t\n");

   // If the token starts with a quote, we're getting a string
   if ( (lineToken != NULL) && (lineToken[0] == '"') )
   {
      // Check to see if the quoted string is actually just a single word
      // (meaning the last strtok call read through the second quote as well)
      if (lineToken[strlen(lineToken)-1] == '"')
      {
         // Overwrite second quote to get the single word
         lineToken[strlen(lineToken)-1] = '\0';
      }
      else
      {
         // Get the next part of string (until terminating quote)
         lineToken2 = strtok(NULL, "\"");

         // Overwrite previous NULL to put string back together
         lineToken[strlen(lineToken)] = ' ';
      }

      // Return everything after the quotation mark
      return &lineToken[1];
   }
   else
   {
      // No quote so return everything
      return lineToken;
   }
}


atTupleKey * atConfigFile::findKey(char *key)
{
   atTupleKey *   current;

   // Go through the tuple list
   current = tuple_list;
   while (current != NULL)
   {
      // See if this node's key matches our key; if it does, return this node
      if (strcmp(current->key, key) == 0)
         return current;

      // Otherwise, go to the next node
      current = current->next;
   }

   // We didn't find it so return NULL
   return NULL;
}


atTupleKey * atConfigFile::addKey(char *key)
{
   atTupleKey *   newKey;

   // Allocate space for new key
   newKey = (atTupleKey *) calloc(1, sizeof(atTupleKey));
   if (newKey == NULL)
      fprintf(stderr, "WARNING: Unable to allocate memory!\n");

   // Initialize record for new key
   strcpy(newKey->key, key);
   newKey->tupleList = NULL;
   newKey->tupleListEnd = NULL;
   newKey->currentTuple = NULL;
   newKey->next = NULL;

   // Add new key record to tuple list
   if (tuple_list_end == NULL)
   {
      // List is empty so just add this list as the only node
      tuple_list = newKey;
      tuple_list_end = newKey;
   }
   else
   {
      // Add the node to the end of the list
      tuple_list_end->next = newKey;
      tuple_list_end = newKey;
   }

   // Return a pointer to this key (used in adding tuples of arguments)
   return newKey;
}


atTupleHead * atConfigFile::addTuple(atTupleKey * key)
{
   atTupleHead *   head;

   // Allocate space for new argument tuple
   head = (atTupleHead *) calloc(1, sizeof(atTupleHead));
   if (head == NULL)
      fprintf(stderr, "WARNING: Unable to allocate memory!\n");

   // Initialize record for new argument tuple
   head->argumentList = NULL;
   head->argumentListEnd = NULL;
   head->next = NULL;

   // Add new argument tuple to key list
   if (key->tupleListEnd == NULL)
   {
      // Node is only node in new list so added it accordingly
      key->tupleList = head;
      key->tupleListEnd = head;

      // Make current tuple pointer point at the first entry
      key->currentTuple = key->tupleList;
   }
   else
   {
      // Add node to the end of the list
      key->tupleListEnd->next = head;
      key->tupleListEnd = head;
   }

   // Return the tuple head (used for adding argument tuples)
   return head;
}


atTupleArgument * atConfigFile::addArgument(atTupleHead * head, char * arg)
{
   atTupleArgument *   argument;

   // Allocate space for an argument tuple
   argument = (atTupleArgument *) calloc(1, sizeof(atTupleArgument));
   if (argument == NULL)
      fprintf(stderr, "WARNING: Unable to allocate memory!\n");

   // Create a node for this argument
   strcpy(argument->argument, arg);
   argument->next = NULL;

   // Add argument to the argument list
   if (head->argumentListEnd == NULL)
   {
      // Argument list is empty so add this node as only node
      head->argumentList = argument;
      head->argumentListEnd = argument;
   }
   else
   {
      // Argument list is not empty so add this node to the end of the list
      head->argumentListEnd->next = argument;
      head->argumentListEnd = argument;
   }

   // Return the argument record
   return argument;
}


//*************************** PUBLIC MEMBER FUNCTIONS ************************//


bool atConfigFile::getNextTuple(char * key, int * argc, char * argv[])
{
   atTupleKey *        tupleKey;
   atTupleArgument *   arg;

   // Search for key.  If not found, return error
   tupleKey = findKey(key);
   if (tupleKey == NULL)
      return false;

   // If pointer is at end of list already, return error
   if (tupleKey->currentTuple == NULL)
      return false;

   // Otherwise, initialize information
   *argc = 0;

   // Copy data into array
   arg = tupleKey->currentTuple->argumentList;
   while (arg != NULL)
   {
      // Copy pointer to the string into array, and advance count
      argv[*argc] = arg->argument;
      (*argc)++;

      // Advance to next argument
      arg = arg->next;
   }

   // Advance pointer
   tupleKey->currentTuple = tupleKey->currentTuple->next;

   // We found a tuple so return that we did
   return true;
}


bool atConfigFile::resetTupleList(char * key)
{
   atTupleKey *   tupleKey;

   // Find the corresponding key in the list
   tupleKey = findKey(key);

   // If we didn't find it, return an error
   if (tupleKey == NULL)
      return false;

   // Otherwise, move the current pointer back to the front of the tuple list
   tupleKey->currentTuple = tupleKey->tupleList;

   // Return that the reset occurred fine
   return true;
}

