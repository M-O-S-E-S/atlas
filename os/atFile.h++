
#ifndef AT_FILE_HPP
#define AT_FILE_HPP


// Microsoft has a different name for the access function.
// The flags for read access, write access, and so on also need to be defined.
#ifdef _MSC_VER
   #define access   _access
   #define F_OK   0x00
   #define W_OK   0x02
   #define R_OK   0x04
#endif


#endif

