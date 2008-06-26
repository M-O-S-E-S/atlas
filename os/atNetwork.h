
#ifndef AT_NETWORK_H
#define AT_NETWORK_H


// Under Windows, define stuff that we need
#ifdef _MSC_VER
    #include <winsock.h>

    typedef SOCKET   Socket;
    typedef int      socklen_t;
#else
    #include <fcntl.h>
    #include <netinet/in.h>

    typedef int   Socket;
#endif


void   initNetwork();
void   cleanupNetwork();

int   setBlockingFlag(Socket socket, bool block);


#endif

