 ///
 /// @file    TcpServer.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2017-05-12 16:26:05
 ///
 
#include "../../../include/on_line_part/TcpServer/TcpServer.h"
#include "../../../include/on_line_part/TcpServer/InetAddress.h"
#include "../../../include/on_line_part/TcpServer/SocketUtil.h"
#include <iostream>
using std::cout;
using std::endl;


namespace cjy
{

TcpServer::TcpServer(const string & ip, unsigned short port)
: acceptor_(createSocketFd(), InetAddress(ip.c_str(), port))
, poller_(acceptor_)
{
}

void TcpServer::start()
{
	acceptor_.ready();
	poller_.setConnectionCallback(connectionCallback_);
	poller_.setMessageCallback(messageCallback_);
	poller_.setCloseCallback(closeCallback_);
	poller_.loop();
}

void TcpServer::stop()
{
	poller_.unloop();
}
	
void TcpServer::setConnectionCallback(TcpServerCallback cb)
{	connectionCallback_ = cb;	}
void TcpServer::setMessageCallback(TcpServerCallback cb)
{	messageCallback_ = cb;	}
void TcpServer::setCloseCallback(TcpServerCallback cb)
{	closeCallback_ = cb;	}

}//end of namespace wd
