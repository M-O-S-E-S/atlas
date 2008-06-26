
#ifndef AT_NOTIFIER_H
#define AT_NOTIFIER_H


// INCLUDES
#include <stdio.h>

#include "atOSDefs.h"


// CONSTANTS
// Simple error notification constants used by setNotifyLevel() and notify()
enum atNotifyLevel
{
   AT_INFO = 0x0001,
   AT_WARN = 0x0002,
   AT_ERROR = 0x0004,
   AT_FATAL_ERROR = 0x0008,
   AT_NO_HEADER = 0x0100,
   AT_FORCE = 0x0200,
   AT_DEBUG = 0x0400,
   AT_SILENT = 0x0800,
   AT_LEVEL_MASK = 0x00FF,
   AT_MOD_MASK = 0xFF00
};


class ATLAS_SYM atNotifier
{
   protected:
      char            header_name[80];
      FILE *          output_file_ptr;

      static int      notify_level;

   public:
      atNotifier();
      virtual ~atNotifier();

      virtual void   setName(char * name);
      virtual void   setNotifyLevel(int desiredLevel);
      virtual void   setNotifyDestination(FILE * output);
      virtual void   notify(int level, char * str, ...);
};

#endif

