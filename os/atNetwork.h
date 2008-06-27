
#ifndef AT_NETWORK_H
#define AT_NETWORK_H


// Under Windows, define stuff that we need
#ifdef _MSC_VER
    #include <winsock.h>

    #define MAXHOSTNAMELEN   64

    #define EWOULDBLOCK   WSAEWOULDBLOCK
    #define EINPROGRESS   WSAEINPROGRESS
    #define MSG_WAITALL   0

    typedef SOCKET   Socket;
    typedef int      socklen_t;
    typedef char     SocketOptionFlag;

    #define close(x)   closesocket((x))
#else
    #include <unistd.h>
    #include <string.h>
    #include <netdb.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/param.h>
    #include <sys/select.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>

    typedef int   Socket;
    typedef int   SocketOptionFlag;
#endif


void   initNetwork();
void   cleanupNetwork();

void   setBlockingFlag(Socket socket, bool block);
bool   getBlockingFlag(Socket socket);


#endif

