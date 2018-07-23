/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2013-2017 OSSRS(winlin)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <net/base/FFL_Net.h>
#include "srs_service_io.hpp"
#include "srs_socket_util.hpp"


using namespace std;

#include <srs_kernel_error.hpp>
#include <srs_kernel_log.hpp>
#include "srs_service_utility.hpp"
#include <srs_kernel_utility.hpp>
#include <FFL.h>
#include <thread/FFL_mutex.h>
#include <thread/FFL_thread.h>


//#ifdef __linux__
//#include <sys/epoll.h>
//
//bool srs_st_epoll_is_supported(void)
//{
//    struct epoll_event ev;
//    
//    ev.events = EPOLLIN;
//    ev.data.ptr = NULL;
//    /* Guaranteed to fail */
//    epoll_ctl(-1, EPOLL_CTL_ADD, -1, &ev);
//    
//    return (errno != ENOSYS);
//}
//#endif
//
//srs_error_t srs_st_init()
//{
//
//    
//    return srs_success;
//}
//
//void srs_close_stfd(srs_netfd_t& stfd)
//{
//    if (stfd) {
//      
//        stfd = NULL;
//    }
//}
//
//void srs_fd_close_exec(int fd)
//{
//  
//}
//
//void srs_socket_reuse_addr(int fd)
//{
//    int v = 1;
//    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(int));
//}
//
//srs_thread_t srs_thread_self()
//{
//    return (srs_thread_t)st_thread_self();
//}
//
//int srs_socket_connect(string server, int port, int64_t tm, srs_netfd_t* pstfd)
//{
//    int ret = ERROR_SUCCESS;
//    
//    st_utime_t timeout = ST_UTIME_NO_TIMEOUT;
//    if (tm != SRS_CONSTS_NO_TMMS) {
//        timeout = (st_utime_t)(tm * 1000);
//    }
//    
//    *pstfd = NULL;
//    srs_netfd_t stfd = NULL;
//    sockaddr_in addr;
//    
//    int sock = socket(AF_INET, SOCK_STREAM, 0);
//    if(sock == -1){
//        ret = ERROR_SOCKET_CREATE;
//        srs_error("create socket error. ret=%d", ret);
//        return ret;
//    }
//    
//    srs_fd_close_exec(sock);
//    
//    srs_assert(!stfd);
//    stfd = st_netfd_open_socket(sock);
//    if(stfd == NULL){
//        ret = ERROR_ST_OPEN_SOCKET;
//        srs_error("st_netfd_open_socket failed. ret=%d", ret);
//        return ret;
//    }
//    
//    // connect to server.
//    std::string ip = srs_dns_resolve(server);
//    if (ip.empty()) {
//        ret = ERROR_SYSTEM_IP_INVALID;
//        srs_error("dns resolve server error, ip empty. ret=%d", ret);
//        goto failed;
//    }
//    
//    addr.sin_family = AF_INET;
//    addr.sin_port = htons(port);
//    addr.sin_addr.s_addr = inet_addr(ip.c_str());
//    
//    if (st_connect((st_netfd_t)stfd, (const struct sockaddr*)&addr, sizeof(sockaddr_in), timeout) == -1){
//        ret = ERROR_ST_CONNECT;
//        srs_error("connect to server error. ip=%s, port=%d, ret=%d", ip.c_str(), port, ret);
//        goto failed;
//    }
//    srs_info("connect ok. server=%s, ip=%s, port=%d", server.c_str(), ip.c_str(), port);
//    
//    *pstfd = stfd;
//    return ret;
//    
//failed:
//    if (stfd) {
//        srs_close_stfd(stfd);
//    }
//    return ret;
//}
//

struct srs_cond
{
	FFL_cond* cond;
	FFL_mutex* mutex;
};
srs_cond_t srs_cond_new()
{
	srs_cond* cond = new srs_cond();
	cond->cond=FFL_CreateCond();
	cond->mutex = FFL_CreateMutex();
	return cond;
}

int srs_cond_destroy(srs_cond_t cond)
{
	FFL_DestroyCond(cond->cond);
	FFL_DestroyMutex(cond->mutex);
	delete cond;

	return 0;
}

int srs_cond_wait(srs_cond_t cond)
{
	FFL_LockMutex(cond->mutex);
	FFL_CondWait(cond->cond, cond->mutex);
	FFL_UnlockMutex(cond->mutex);
	//return st_cond_wait((st_cond_t)cond);
	return 0;
}

int srs_cond_timedwait(srs_cond_t cond, srs_utime_t timeout)
{
	FFL_LockMutex(cond->mutex);
	FFL_CondWaitTimeout(cond->cond, cond->mutex, timeout);
	FFL_UnlockMutex(cond->mutex);
	//return st_cond_timedwait((st_cond_t)cond, (st_utime_t)timeout);
	return 0;
}

int srs_cond_signal(srs_cond_t cond)
{
	FFL_CondSignal(cond->cond);
	return 0;
}

//srs_mutex_t srs_mutex_new()
//{
//    return (srs_mutex_t)st_mutex_new();
//}
//
//int srs_mutex_destroy(srs_mutex_t mutex)
//{
//    return st_mutex_destroy((st_mutex_t)mutex);
//}
//
//int srs_mutex_lock(srs_mutex_t mutex)
//{
//    return st_mutex_lock((st_mutex_t)mutex);
//}
//
//int srs_mutex_unlock(srs_mutex_t mutex)
//{
//    return st_mutex_unlock((st_mutex_t)mutex);
//}
//
//int srs_netfd_fileno(srs_netfd_t stfd)
//{
//    return st_netfd_fileno((st_netfd_t)stfd);
//}
//
//int srs_usleep(srs_utime_t usecs)
//{
//    return st_usleep((st_utime_t)usecs);
//}
//
//srs_netfd_t srs_netfd_open_socket(int osfd)
//{
//    return (srs_netfd_t)st_netfd_open_socket(osfd);
//}
//
//srs_netfd_t srs_netfd_open(int osfd)
//{
//    return (srs_netfd_t)st_netfd_open(osfd);
//}
//
//int srs_recvfrom(srs_netfd_t stfd, void *buf, int len, struct sockaddr *from, int *fromlen, srs_utime_t timeout)
//{
//    return st_recvfrom((st_netfd_t)stfd, buf, len, from, fromlen, (st_utime_t)timeout);
//}
//
//srs_netfd_t srs_accept(srs_netfd_t stfd, struct sockaddr *addr, int *addrlen, srs_utime_t timeout)
//{
//    return (srs_netfd_t)st_accept((st_netfd_t)stfd, addr, addrlen, (st_utime_t)timeout);
//}
//
//ssize_t srs_read(srs_netfd_t stfd, void *buf, size_t nbyte, srs_utime_t timeout)
//{
//    return st_read((st_netfd_t)stfd, buf, nbyte, (st_utime_t)timeout);
//}

SrsStSocket::SrsStSocket()
{
    stfd = NULL;
    stm = rtm = SRS_CONSTS_NO_TMMS;
    rbytes = sbytes = 0;
	mSocket = new FFL::CSocket(stfd);
}

SrsStSocket::~SrsStSocket()
{
}

int SrsStSocket::initialize(srs_netfd_t fd)
{
    stfd = fd;
	mSocket->setFd(stfd);
    return ERROR_SUCCESS;
}

bool SrsStSocket::is_never_timeout(int64_t tm)
{
    return tm == SRS_CONSTS_NO_TMMS;
}

void SrsStSocket::set_recv_timeout(int64_t tm)
{
    rtm = tm;
}

int64_t SrsStSocket::get_recv_timeout()
{
    return rtm;
}

void SrsStSocket::set_send_timeout(int64_t tm)
{
    stm = tm;
}

int64_t SrsStSocket::get_send_timeout()
{
    return stm;
}

int64_t SrsStSocket::get_recv_bytes()
{
    return rbytes;
}

int64_t SrsStSocket::get_send_bytes()
{
    return sbytes;
}

int SrsStSocket::read(void* buf, size_t size, ssize_t* nread)
{
    int ret = ERROR_SUCCESS;
    
	size_t nbRead=0;
	if (FFL_OK != mSocket->read((uint8_t*)buf, size, (size_t*)&nbRead)) {
		FFL_LOG_ERROR("Failed to SrsStSocket::read");
		nbRead = 0;
		return ERROR_SOCKET_READ;
	}
	
    if (nread) {
        *nread = nbRead;
    }
    
    // On success a non-negative integer indicating the number of bytes actually read is returned
    // (a value of 0 means the network connection is closed or end of file is reached).
    // Otherwise, a value of -1 is returned and errno is set to indicate the error.
    if (nbRead <= 0) {
        // @see https://github.com/ossrs/srs/issues/200
        if (nbRead < 0 && errno == ETIME) {
            return ERROR_SOCKET_TIMEOUT;
        }
        
        if (nbRead == 0) {
            errno = ECONNRESET;
        }
        
        return ERROR_SOCKET_READ;
    }
    
    rbytes += nbRead;
    
    return ret;
}

int SrsStSocket::read_fully(void* buf, size_t size, ssize_t* nread)
{
    int ret = ERROR_SUCCESS;
    
	size_t nbRead;
  //  if (rtm == SRS_CONSTS_NO_TMMS) {
  //      nb_read = FFL_socket_read(stfd,(char*) buf, size);
  //  } else {
		//nb_read = FFL_socket_read(stfd, (char*)buf, size);// rtm * 1000);
  //  }		
	if (FFL_OK != mSocket->read((uint8_t*)buf, size, (size_t*)&nbRead)) {
		FFL_LOG_ERROR("Failed to SrsStSocket::read_fully");
		nbRead = 0;
		return ERROR_SOCKET_READ;
	}
    
    if (nread) {
        *nread = nbRead;
    }
    
    // On success a non-negative integer indicating the number of bytes actually read is returned
    // (a value less than nbyte means the network connection is closed or end of file is reached)
    // Otherwise, a value of -1 is returned and errno is set to indicate the error.
    if (nbRead != (ssize_t)size) {
        // @see https://github.com/ossrs/srs/issues/200
        if (nbRead < 0 && errno == ETIME) {
            return ERROR_SOCKET_TIMEOUT;
        }
        
        if (nbRead >= 0) {
            errno = ECONNRESET;
        }
        
        return ERROR_SOCKET_READ_FULLY;
    }
    
    rbytes += nbRead;
    
    return ret;
}

int SrsStSocket::write(void* buf, size_t size, ssize_t* nwrite)
{
    int ret = ERROR_SUCCESS;
    
    size_t nb_write;
	if (FFL_OK != mSocket->write((uint8_t*)buf, size, (size_t*)&nb_write)) {
		FFL_LOG_ERROR("Failed to SrsStSocket::write");
		nb_write = 0;
		return ERROR_SOCKET_WRITE;
	}


  //  if (stm == SRS_CONSTS_NO_TMMS) {
		//ret = FFL_socketWrite(stfd, buf, size, &nb_write);
  //  } else {
		//ret = FFL_socketWrite(stfd, buf, size, &nb_write);
  //  }
    
    if (nwrite) {
        *nwrite = nb_write;
    }
    
    // On success a non-negative integer equal to nbyte is returned.
    // Otherwise, a value of -1 is returned and errno is set to indicate the error.
    if (nb_write <= 0) {
        // @see https://github.com/ossrs/srs/issues/200
        if (nb_write < 0 && errno == ETIME) {
            return ERROR_SOCKET_TIMEOUT;
        }
        
        return ERROR_SOCKET_WRITE;
    }
    
    sbytes += nb_write;
    
    return ret;
}

int SrsStSocket::writev(const iovec *iov, int iov_size, ssize_t* nwrite)
{
    int ret = ERROR_SUCCESS;
    
	//FFL_ASSERT_LOG(0, " SrsStSocket::writev");
    size_t nb_write=0;
	size_t socket_ret;
	for (int io = 0; io < iov_size; io++) {
		//if (stm == SRS_CONSTS_NO_TMMS) {
		//	socket_ret = FFL_socketWrite(stfd, (const char*)iov[io].iov_base, iov[io].iov_len);
		//}
		//else {
		//	socket_ret = FFL_socketWrite(stfd, (const char*)iov[io].iov_base, iov[io].iov_len);
		//}

		size_t nWrite;
		if (FFL_OK != mSocket->write((uint8_t*)iov[io].iov_base, iov[io].iov_len, (size_t*)&nWrite)) {
			FFL_LOG_ERROR("Failed to SrsStSocket::write");
			break;
		}

		
		nb_write += nWrite;
	}
    
    if (nwrite) {
        *nwrite = nb_write;
    }
    
    // On success a non-negative integer equal to nbyte is returned.
    // Otherwise, a value of -1 is returned and errno is set to indicate the error.
    if (nb_write <= 0) {
        // @see https://github.com/ossrs/srs/issues/200
        if (nb_write < 0 && errno == ETIME) {
            return ERROR_SOCKET_TIMEOUT;
        }
        
        return ERROR_SOCKET_WRITE;
    }
    
    sbytes += nb_write;
    
    return ret;
}

SrsTcpClient::SrsTcpClient(string h, int p, int64_t tm)
{
    stfd = NULL;
    io = new SrsStSocket();
    
    host = h;
    port = p;
    timeout = tm;
}

SrsTcpClient::~SrsTcpClient()
{
    close();
    
    srs_freep(io);
}

int SrsTcpClient::connect()
{
    int ret = ERROR_SUCCESS;
    
    close();
    
    //srs_assert(stfd == NULL);
    //if ((ret = srs_socket_connect(host, port, timeout, &stfd)) != ERROR_SUCCESS) {
    //    srs_error("connect tcp://%s:%d failed, to=%" PRId64 "ms. ret=%d", host.c_str(), port, timeout, ret);
    //    return ret;
    //}
    //
    //if ((ret = io->initialize(stfd)) != ERROR_SUCCESS) {
    //    return ret;
    //}
    
    return ret;
}

void SrsTcpClient::close()
{
    // Ignore when already closed.
    if (!io) {
        return;
    }
    
   // srs_close_stfd(stfd);
}

bool SrsTcpClient::is_never_timeout(int64_t tm)
{
    return io->is_never_timeout(tm);
}

void SrsTcpClient::set_recv_timeout(int64_t tm)
{
    io->set_recv_timeout(tm);
}

int64_t SrsTcpClient::get_recv_timeout()
{
    return io->get_recv_timeout();
}

void SrsTcpClient::set_send_timeout(int64_t tm)
{
    io->set_send_timeout(tm);
}

int64_t SrsTcpClient::get_send_timeout()
{
    return io->get_send_timeout();
}

int64_t SrsTcpClient::get_recv_bytes()
{
    return io->get_recv_bytes();
}

int64_t SrsTcpClient::get_send_bytes()
{
    return io->get_send_bytes();
}

int SrsTcpClient::read(void* buf, size_t size, ssize_t* nread)
{
    return io->read(buf, size, nread);
}

int SrsTcpClient::read_fully(void* buf, size_t size, ssize_t* nread)
{
    return io->read_fully(buf, size, nread);
}

int SrsTcpClient::write(void* buf, size_t size, ssize_t* nwrite)
{
    return io->write(buf, size, nwrite);
}

int SrsTcpClient::writev(const iovec *iov, int iov_size, ssize_t* nwrite)
{
    return io->writev(iov, iov_size, nwrite);
}

