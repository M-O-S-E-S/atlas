
#ifndef AT_TIMER_HPP
#define AT_TIMER_HPP


#include "atNotifier.h++"


class atTimer : public atNotifier
{
   private:

      static atTimer    *systemTimer;

      double            markTime;
      double            markInterval;

   public:

      atTimer();
      ~atTimer();

      void              mark();
      void              markAtInterval(double intervalTime);
      double            getInterval();
      double            getElapsed();
};

#endif
