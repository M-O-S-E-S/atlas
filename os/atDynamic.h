
#ifndef AT_DYNAMIC_H
#define AT_DYNAMIC_H


#ifdef _MSC_VER
   #define RTLD_GLOBAL   0
   #define RTLD_LAZY   0
   #define RTLD_NOW   0
#endif


extern "C"
{
   void *   dlOpen(const char * filename, int flag);
   char *   dlError();
   void *   dlSym(void * handle, const char * symbol);
   int      dlClose(void * handle);
}


#endif

