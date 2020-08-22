 ///
 /// @file    SpellCorrectServer.cc
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-12 21:01:53

#include "../../../include/on_line_part/SpellCorrectServer/SpellCorrectServer.h"
#include "../../../include/on_line_part/TcpServer/TcpServer.h"
#include "../../../include/on_line_part/Configuration/Configuration.h"
#include "../../../include/on_line_part/SpellCorrectServer/MyTask.h"
#include <iostream>
#include <string>
#include <functional>
using namespace std;
using namespace cjy;

Configuration *myconf = Configuration::ConfCreateInstance(); //单例对象
ThreadPool SpellCorrectServer::_threadpool(myconf->getmythreadNum(),myconf->getmyqueSize()); //在类外定义线程池（静态成员）

SpellCorrectServer::SpellCorrectServer()  //构造函数
:_tcpServer(myconf->getmyip(),myconf->getmyport())
{}

void SpellCorrectServer::onConnection(const TcpConnectionPtr &conn)  //刚连接时应该执行的一些常规操作
{
	cout << conn->toString() << endl;  //首先在服务器显示连接信息
}

void SpellCorrectServer::onClose(const TcpConnectionPtr &conn)  //服务器收到连接断开时，显示连接断开的消息即可，不用做任何事情。
{
	 cout << conn->toString().c_str() << " close" << endl;
}

void SpellCorrectServer::onMessage(const TcpConnectionPtr &conn)  //当服务器收到旧连接发来的请求单词时，应该做的一些具体操作
{
	string s(conn->receive());  //先得到要查询的单词，之后要进行的操作就封装成一个具体的类，在类中的任务函数中实现具体应该
	//执行的一些操作。
	s=s.substr(0,s.size()-1); //去掉收到的换行符，这点很重要。！！！！！！！！
	cout << "queryword: " << s << endl;
  
	MyTask mytask(s,conn);   //将查询单词以及tcpconnection对象作为任务队列的参数，将Task类的execute函数作为任务队列的函数，
	//表明要子线程具体执行的一些操作（编解码，以及计算任务）
	_threadpool.addTask(std::bind(&MyTask::execute,mytask));
	
	//cout << "bind seccess" << endl;
}

void SpellCorrectServer::start()   //正式开始运行文本纠错服务器
{
	_threadpool.start() ; //开始运行线程池，然后将回调函数传递过去，就额可以证实运行
	_tcpServer.setConnectionCallback(&onConnection);
	_tcpServer.setMessageCallback(&onMessage);
	_tcpServer.setCloseCallback(&onClose);

	_tcpServer.start(); //开始运行tcp服务器，开始监听文件描述符。
}

