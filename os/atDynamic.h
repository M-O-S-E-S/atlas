
#ifndef AT_DYNAMIC_H
#define AT_DYNAMIC_H


void *   dlOpen(const char * filename, int flag);
char *   dlError();
void *   dlSym(void * handle, const char * symbol);
int      dlClose(void * handle);


#endif

