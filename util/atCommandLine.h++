
#ifndef AT_COMMAND_LINE_HPP
#define AT_COMMAND_LINE_HPP


#include "atNotifier.h++"
#include "atString.h++"
#include "atList.h++"
#include "atMap.h++"


class ATLAS_SYM atCommandLine : public atNotifier
{
   protected:

      atMap    valid_options;

      atMap    options_map;
      atList   args_list;

      void     parseValidOptions(atString optionsSpec);
      bool     takesValue(atString option);
      bool     parseOption(char * arg, char * nextArg);

   public:

      atCommandLine();
      atCommandLine(atString optionsSpec);
      atCommandLine(int argc, char ** argv, atString optionsSpec);
      virtual ~atCommandLine();

      void       setValidOptions(atString optionsSpec);
      void       addValidOption(char option, bool takesValue);

      void       parseCommandLine(int argc, char ** argv);

      bool       hasOption(atString opt);
      atString   getOptionValue(atString opt);

      atList *   getOptionList();
      atList *   getArgumentList();
};


#endif

