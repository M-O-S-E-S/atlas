
#ifndef AT_LANG_H
#define AT_LANG_H

#ifdef __OBJC__
#define OBJC_CLASS(name) @class name
#else
#define OBJC_CLASS(name) typedef struct objc_object name
#endif

#endif

