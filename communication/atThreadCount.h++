
#ifndef AT_THREAD_COUNT_H
#define AT_THREAD_COUNT_H


// INCLUDES
#include <sys/types.h>
#include "atItem.h++"
#include "atOSDefs.h++"


class ATLAS_SYM atThreadCount : public atItem
{
   protected:
      u_long   thread_count;

   public:
      atThreadCount();
      virtual ~atThreadCount();

      virtual u_long   getCount();
      virtual void     inc();
      virtual void     dec();
};


#endif

