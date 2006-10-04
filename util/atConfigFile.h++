
#ifndef AT_CONFIG_FILE_H
#define AT_CONFIG_FILE_H


#include "atNotifier.h++"
#include "atString.h++"
#include "atStringTokenizer.h++"


#define AT_MAX_KEY_LENGTH        256
#define AT_MAX_ARGUMENT_LENGTH   256


typedef struct atCfgArgument
{
   char                     argument[AT_MAX_ARGUMENT_LENGTH];
   struct atCfgArgument *   next;
} atTupleArgument;

typedef struct atCfgHead
{
   atTupleArgument *    argumentList;
   atTupleArgument *    argumentListEnd;
   struct atCfgHead *   next;
} atTupleHead;

typedef struct atCfgKey
{
   char                key[AT_MAX_KEY_LENGTH];
   atTupleHead *       tupleList;
   atTupleHead *       tupleListEnd;
   atTupleHead *       currentTuple;
   struct atCfgKey *   next;
} atTupleKey;


class atConfigFile : public atNotifier
{
   protected:
      atTupleKey *          tuple_list;
      atTupleKey *          tuple_list_end;

      atStringTokenizer *   current_tokenizer;

      atString *          getToken();
      atTupleKey *        findKey(char * key);
      atTupleKey *        addKey(char * key);
      atTupleHead *       addTuple(atTupleKey * key);
      atTupleArgument *   addArgument(atTupleHead * head, char * arg);

   public:
      atConfigFile(char * filename);
      ~atConfigFile();

      virtual bool   getNextTuple(char * key, int * argc, char * argv[]);
      virtual bool   resetTupleList(char * key);
};

#endif

