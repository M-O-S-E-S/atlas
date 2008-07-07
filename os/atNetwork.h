
#ifndef AT_NETWORK_H
#define AT_NETWORK_H


// Under Windows, define stuff that we need
#ifdef _MSC_VER
    #include <winsock.h>

    #define EWOULDBLOCK   WSAEWOULDBLOCK
    #define EINPROGRESS   WSAEINPROGRESS
    #define MSG_WAITALL   0

    #define MAXHOSTNAMELEN   64

    typedef SOCKET   Socket;
    typedef int      socklen_t;
    typedef char     SocketOptionFlag;
    typedef int      SocketOptionValue;
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

    #ifndef MAXHOSTNAMELEN
       #define MAXHOSTNAMELEN   64
    #endif

    typedef int   Socket;
    typedef int   SocketOptionFlag;
    typedef int   SocketOptionValue;
#endif


extern "C"
{
    void   initNetwork();
    void   cleanupNetwork();

    Socket   openSocket(int domain, int type, int protocol);
    void     closeSocket(Socket socket);

    void   setBlockingFlag(Socket socket, bool block);
    bool   getBlockingFlag(Socket socket);
}


#endif

