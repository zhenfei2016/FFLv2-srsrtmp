#ifndef _SRS_CORE_WINDOWS_HPP_
#define _SRS_CORE_WINDOWS_HPP_

//
//#include <stdio.h>
#include <net/base/FFL_Net.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <time.h>

#ifndef ERROR_SUCCESS
#define  ERROR_SUCCESS FFL_OK
#endif
//
//// the type used by this header for windows.
//typedef unsigned long long u_int64_t;
//typedef u_int64_t uint64_t;
//typedef long long int64_t;
//typedef unsigned int u_int32_t;
//typedef u_int32_t uint32_t;
//typedef int int32_t;
//typedef unsigned char u_int8_t;
//typedef u_int8_t uint8_t;
////typedef char int8_t;
//typedef unsigned short u_int16_t;
//typedef u_int16_t uint16_t;
//typedef short int16_t;
typedef int64_t ssize_t;
struct iovec {
	void  *iov_base;    /* Starting address */
	size_t iov_len;     /* Number of bytes to transfer */
};

// for pid.
typedef int pid_t;
pid_t getpid(void);

//#include <time.h>
int gettimeofday(struct timeval* tv, struct timezone* tz);

#ifndef PRId64
#define PRId64 "lld"
#endif

// for inet helpers.
//typedef int socklen_t;
//const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);

//#if WIN32
// for mkdir().
#include<direct.h>

//#else
// for mkdir().
#include<direct.h>

// for open().
typedef int mode_t;
#define S_IRUSR 0
#define S_IWUSR 0
#define S_IXUSR 0
#define S_IRGRP 0
#define S_IWGRP 0
#define S_IXGRP 0
#define S_IROTH 0
#define S_IXOTH 0

// for file seek.
#include <io.h>
#include <fcntl.h>
//#define open _open
//#define close _close
//#define lseek _lseek
//#define write _write
//#define read _read
//#endif

// for socket.
ssize_t writev(int fd, const struct iovec *iov, int iovcnt);
//typedef int64_t useconds_t;
//int usleep(useconds_t usec);
int usleep(int64_t usec);
int socket_setup();
int socket_cleanup();

// others.
#define snprintf _snprintf

#endif