
#ifndef AT_BYTE_SWAP_HPP
#define AT_BYTE_SWAP_HPP


// We don't want to use Microsoft's ntonf, etc. functions for floats/doubles
#ifdef _MSC_VER
#define NO_EXTRA_HTON_FUNCTIONS
#endif


#include "atSymbols.h++"


ATLAS_SYM float    htonf(float x);
ATLAS_SYM float    ntohf(float x);
ATLAS_SYM double   htond(double x);
ATLAS_SYM double   ntohd(double x);


#endif

