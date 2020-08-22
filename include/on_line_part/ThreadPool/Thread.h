 ///
 /// @file    Thread.h
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-03 19:22:57
 ///
//线程的封装 

#ifndef __THREAD_BIND_FUNCTION_H__
#define __THREAD_BIND_FUNCTION_H__
#include "Noncopyable.h"
#include <pthread.h>
#include <functional>
#include <string>
using std::function;
using std::string;
namespace cjy
{
	namespace current_thread
	{
		extern __thread const char * threadName;
	}
	class Thread
	:Noncopyable   //私有继承自Noncopyable
	{
	public:
		using ThreadCallback = function<void()>;//给函数对象起别名，用于接受bind传过来的run方法
		Thread(ThreadCallback &&,const string &name=string()); //无参构造函数，接受函数对象用于注册回调函数
		//为了与之前的线程类型兼容，可以考虑给这个name提供一个默认的形参。
		~Thread(); //虚析构函数
		void start();//创建子线程的函数
		void join() ;//回收子线程的函数

		bool isRunning() const   //检查线程的运行状态
		{
			return _isRunning;
		}

	private:
		static void *threadFunc(void *);  //线程函数的入口地址，且这个函数只能由start来调用（创建线程时调用，因此为私有的）
//而且这个函数必须是静态的，如果是非静态的成员函数，那么实际上在threadFunc的参数表中第一个参数实际上是一个隐含的this指针，
//指向本对象。但我们需要的是void*类型，因此把它设为static，消除this指针。在这个函数中执行回调函数。
	
	private:
		pthread_t _pthid;//线程号

		int _pthindex;  //子线程在线程池的vector中的下标。标志这个子线程只能操控Cache中对应下标的缓存系统。

		bool _isRunning; //是否正在运行的标志位
		ThreadCallback _cb;  //定义一个function对象，用于接受回调函数
		string _name; //线程名字
	};
}


#endif 
