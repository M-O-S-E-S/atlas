
#ifndef AT_CONFIG_FILE_H
#define AT_CONFIG_FILE_H


#include "atNotifier.h++"


#define AT_MAX_KEY_LENGTH        255
#define AT_MAX_ARGUMENT_LENGTH   255


typedef struct tupleArgument
{
   char                     argument[AT_MAX_ARGUMENT_LENGTH];
   struct tupleArgument *   next;
} atTupleArgument;

typedef struct tupleHead
{
   atTupleArgument *    argumentList;
   atTupleArgument *    argumentListEnd;
   struct tupleHead *   next;
} atTupleHead;

typedef struct tupleKey
{
   char                key[AT_MAX_KEY_LENGTH];
   atTupleHead *       tupleList;
   atTupleHead *       tupleListEnd;
   atTupleHead *       currentTuple;
   struct tupleKey *   next;
} atTupleKey;


class atConfigFile : public atNotifier
{
   protected:
      atTupleKey *   tuple_list;
      atTupleKey *   tuple_list_end;

      char *              getToken(char *buffer);
      atTupleKey *        findKey(char *key);
      atTupleKey *        addKey(char *key);
      atTupleHead *       addTuple(atTupleKey *key);
      atTupleArgument *   addArgument(atTupleHead *head, char *arg);

   public:
      atConfigFile(char *filename);
      ~atConfigFile();

      virtual bool   getNextTuple(char *key, int *argc, char *argv[]);
      virtual bool   resetTupleList(char *key);
};

#endif

