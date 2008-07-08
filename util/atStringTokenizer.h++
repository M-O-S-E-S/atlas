
#ifndef AT_STRING_TOKENIZER_H
#define AT_STRING_TOKENIZER_H


#include "atNotifier.h++"
#include "atOSDefs.h++"
#include "atString.h++"


class ATLAS_SYM atStringTokenizer : public atNotifier
{
   protected:
      atString *   the_string;
      char *       the_chars;

      u_long       string_index;

   public:
      atStringTokenizer(atString str);
      virtual ~atStringTokenizer();

      atString *   getToken(atString delimiters);
      atString *   getToken(char delimiters[]);
};


#endif

