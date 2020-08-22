 ///
 /// @file    Socket.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2015-11-04 17:42:38
 ///

#include "../../../include/on_line_part/TcpServer/Socket.h"
#include "../../../include/on_line_part/TcpServer/SocketUtil.h"
#include "../../../include/on_line_part/TcpServer/InetAddress.h"

namespace cjy
{

Socket::Socket(int sockfd)
: sockfd_(sockfd)
{}

Socket::Socket()
: sockfd_(createSocketFd())
{
}

Socket::~Socket()
{
	::close(sockfd_);
}

void Socket::nonblock()
{
	setNonblock(sockfd_);
}


void Socket::shutdownWrite()
{
	if(::shutdown(sockfd_, SHUT_WR) == -1)
	{
		perror("shudown write error!");
	}
}


InetAddress Socket::getLocalAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(sockaddr_in);
	if(::getsockname(sockfd, (struct sockaddr *)&addr, &len) == -1)
	{
		perror("getsockname error");
	}
	return InetAddress(addr);
}

InetAddress Socket::getPeerAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(sockaddr_in);
	if(::getpeername(sockfd, (struct sockaddr *)&addr, &len) == -1)
	{
		perror("getpeername error");
	}
	return InetAddress(addr);
}

}// end of namespace wd
