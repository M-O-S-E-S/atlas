
#include "atByteSwap.h++"


// Need to have winsock in the windows version
#ifdef _MSC_VER
   #define WIN32_LEAN_AND_MEAN
   #include <winsock2.h>
   #undef WIN32_LEAN_AND_MEAN
#else
   #include <netinet/in.h>
#endif


float htonf(float x)
{
   char   *tmp;
   char   c;

   if (htons(1) != 1)
   {
      // Machine is little endian
      tmp = (char *) &x;

      c = tmp[0];
      tmp[0] = tmp[3];
      tmp[3] = c;
      c = tmp[1];
      tmp[1] = tmp[2];
      tmp[2] = c;
   }

   return x;
}


float ntohf(float x)
{
   return htonf(x);
}


double htond(double x)
{
   char   *tmp;
   char   c;

   if (htons(1) != 1)
   {
      // Machine is little endian
      tmp = (char *) &x;

      c = tmp[0];
      tmp[0] = tmp[7];
      tmp[7] = c;
      c = tmp[1];
      tmp[1] = tmp[6];
      tmp[6] = c;
      c = tmp[2];
      tmp[2] = tmp[5];
      tmp[5] = c;
      c = tmp[3];
      tmp[3] = tmp[4];
      tmp[4] = c;
   }

   return x;
}


double ntohd(double x)
{
   return htond(x);
}

