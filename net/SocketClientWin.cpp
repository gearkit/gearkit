#include "socket.h"
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/socket.h>

#define DSOCKERR		-1
#define DWOULDBLOCK		EWOULDBLOCK
#define DCONNECTERR		EINPROGRESS

namespace kit {

Socket::Socket(int32_t sock)
{
	setHandle(sock);
}

Socket::Socket(int32_t family, int32_t type, int32_t protocol)
{
	init();
	open(family, type, protocol);
}

void Socket::init()
{
	sock_ = DSOCKERR;
}

Socket:::~Socket()
{
	close();
}

void Socket::startUp()
{
	int32_t noblock = 1;
	int32_t revalue = 1;

	// set socket to nonblock
	::ioctl(FIONBIO, ( char* )&noblock);

	// set socket reuse addr
	::setoption(SOL_SOCKET, SO_REUSEADDR, (char*)&revalue, (int32_t)sizeof(revalue));

	// set socket nodelay
	::setoption(IPPROTO_TCP, TCP_NODELAY, (char*)&revalue, (int32_t)sizeof(revalue));
}

int32_t Socket::open(int32_t family, int32_t type, int32_t protocol)
{
	close();

	sock_ = (int32_t)::socket(family, type, protocol);
	if (sock_ < 0) return -1;

	start_up();

	return 0;
}

int32_t Socket::close()
{
	if (sock_ < 0) return 0;

	::closesocket(sock_);

	sock_ = DSOCKERR;

	return 0;
}

void Socket::setHandle(int32_t sock)
{
	init();

	sock_ = sock;

	startUp();
}

int32_t Socket::connect(const sockaddr* addr)
{
	socklen_t len = (socklen_t)sizeof(struct sockaddr);
	int32_t ret = ::connect(sock_, addr, len);

    if(ret == 0) return 0;

	if(ret == DSOCKERR && DCONNECTERR == getErrno())
	{
        fd_set fdw, fde;
        struct timeval tv;

        FD_ZERO(&fdw);
        FD_ZERO(&fde);
        FD_SET(sock_, &fdw);
        FD_SET(sock_, &fde);
        tv.tv_sec = CONNECT_TIMEOUT;
        tv.tv_usec = 0;

        int32_t ret = select(sock_ + 1, NULL, &fdw, &fde, &tv);
        if(ret > 0 && FD_ISSET(sock_, &fdw) && !FD_ISSET(sock_, &fde)) 
			return 0;
	}

	return -1;
}

int32_t Socket::shutdown(int32_t mode)
{
	if (sock_ < 0) return 0;

    int32_t ret = ::shutdown(sock_, mode);
    if(ret == 0 && _mode == 2)
        sock_ = DSOCKERR;
	return ret;
}

int32_t Socket::bind(const sockaddr* addr)
{
	socklen_t len = (socklen_t)sizeof(struct sockaddr);
	return ::bind(sock_, addr, len);
}

int32_t Socket::listen(int32_t count)
{
	return ::listen(sock_, count);
}

int32_t Socket::accept(sockaddr* addr)
{
	socklen_t len = (socklen_t)sizeof(struct sockaddr);
	return ::accept(sock_, addr, &len);
}

int32_t Socket::ioctl(unsigned long cmd, char* argp)
{
	return ::ioctlsocket(sock_, cmd, argp);
}

int32_t Socket::setoption(int32_t level, int32_t optname, const char* optval, int32_t optlen)
{
	return ::setsockopt(sock_, level, optname, optval, optlen);
}

int32_t Socket::getoption(int32_t level, int32_t optname, char* optval, int32_t* optlen)
{
	socklen_t len = (socklen_t)((optlen) ? *optlen : 0);
	int32_t retval;
	retval = getsockopt(sock_, level, optname, optval, &len);
	if (optlen) 
		*optlen = len;

	return retval;
}

int32_t Socket::getErrno()
{
	return errno;
}

int32_t Socket::send(const char* buf, int32_t size, int32_t mode)
{
	return ::send(sock_, buf, size, mode);
}

int32_t Socket::recv(char* buf, int32_t size, int32_t mode)
{
	return ::recv(sock_, buf, size, mode);
}

int32_t Socket::recvfrom(char* buf, int32_t size, int32_t mode, sockaddr *addr)
{
    socklen_t len = sizeof(struct sockaddr);
	return ::recvfrom(sock_, buf, size, mode, addr, &len);
}

int32_t Socket::sendto(const char* buf, int32_t size, int32_t mode, const sockaddr* addr)
{
    socklen_t len = sizeof(struct sockaddr);
	return ::sendto(sock_, buf, size, mode, addr, len);
}

} // namespace kit
