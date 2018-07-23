#ifndef SRS_SOCKET_UTIL_HPP
#define SRS_SOCKET_UTIL_HPP

// for srs-librtmp, @see https://github.com/ossrs/srs/issues/213
#ifndef _WIN32
#define SOCKET_ETIME EWOULDBLOCK
#define SOCKET_ECONNRESET ECONNRESET

#define SOCKET_ERRNO() errno
#define SOCKET_RESET(fd) fd = -1; (void)0
#define SOCKET_CLOSE(fd) \
    if (fd > 0) {\
        ::close(fd); \
        fd = -1; \
    } \
    (void)0
#define SOCKET_VALID(x) (x > 0)
#define SOCKET_SETUP() (void)0
#define SOCKET_CLEANUP() (void)0
#else
#define SOCKET_ETIME WSAETIMEDOUT
#define SOCKET_ECONNRESET WSAECONNRESET
#define SOCKET_ERRNO() WSAGetLastError()
#define SOCKET_RESET(x) x=INVALID_SOCKET
#define SOCKET_CLOSE(x) if(x!=INVALID_SOCKET){::closesocket(x);x=INVALID_SOCKET;}
#define SOCKET_VALID(x) (x!=INVALID_SOCKET)
#define SOCKET_BUFF(x) ((char*)x)
#define SOCKET_SETUP() socket_setup()
#define SOCKET_CLEANUP() socket_cleanup()
#endif

// for srs-librtmp, @see https://github.com/ossrs/srs/issues/213
#ifndef _WIN32
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <sys/socket.h>
#endif

#include <errno.h>


#endif

