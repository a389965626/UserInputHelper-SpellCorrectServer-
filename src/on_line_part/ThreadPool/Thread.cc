///
/// @file    Thread.cc
/// @author  a389965626(389965626@qq.com)
/// @date    2018-07-03 19:31:13
///

#include "../../../include/on_line_part/ThreadPool/Thread.h"

namespace cjy
{

	namespace current_thread  //此命名空间专门存放线程局部存储的变量。这个变量是一个全局的变量，在整个进程地址空间都可以访问，
		//但是访问到的内容只是属于各个子线程自身所特有的，跟其他子线程是不同的。
	{
		__thread const char *threadName="cjy";  //当没有收到传过来的名字时，我们默认把它称为为cjy线程。
	}

    // 线程所特有的数据
	struct ThreadData  //此结构体专门用于存放回调函数和传送过来的线程名
	{
		using ThreadCallback = function<void()>;  //所有的成员都是共有的，只用于在Thread类中进行访问
		ThreadCallback _cb;
		string _name;  

		ThreadData(ThreadCallback &&cb,const string &name)
			:_cb(std::move(cb))
			 ,_name(name)
		{}
		void run()  //子线程应该执行的回调函数，当没有接受到线程名字时，我们就得到的是空串，此时自行给线程取一个名字
		{
			//线程初始化操作。
			//先为线程局部存储的变量current_thread下的threadName初值，以后在这个线程执行时可以直接得到这个名字。（与cache相关）
			current_thread::threadName = (_name == string() ? "cjyThread" : _name.c_str());
			if(_cb)  //回调函数不为空则去执行回调函数。
			{
				_cb();   //以后在回调函数的执行时可以直接得到这个线程局部存储的值。并且虽然每个子线程的这个值的名字相同，
				//但是它们是本线程所特有的，不同的线程中其值是不同的。
			}
		}	
	};

	Thread::Thread(ThreadCallback &&cb,const string &name)  //构造函数
		:_pthid(0)    //默认初始化时现场线程为0，并且不处于运行状态，并且同时之策回调函数，用function对象_cb来接受函数对象
		 ,_isRunning(false)
		 ,_cb(std::move(cb))
		 ,_name(name)
	{}

	void Thread::start()
	{
		ThreadData *pdata=new ThreadData(std::move(_cb),_name);
		pthread_create(&_pthid,NULL,threadFunc,pdata); //start方法创建一个线程,threadFunc为线程函数的入口地址，
		//并且在threadFunc中调用回调函数
		_isRunning=true;
	}

	void *Thread::threadFunc(void *args)
	{
		ThreadData *pdata=static_cast<ThreadData*>(args); //这里传过来的this指针实际上就是Thread对象自己，并不存在什么派生类。
		if(pdata)
		{
			pdata->run();
		}
		delete pdata; //new得到的指针应该回收。
		return NULL;
	}
	//当通过派生类的对象访问基类的start函数，此时的this指针是指向派生类对象的，在threadFunc用基类指针指向派生类对象，访问到的是派生类的run方法


	void Thread::join()
	{
		pthread_join(_pthid,NULL);  //等待线程结束，不接受返回值。
		_isRunning=false;
	}

	Thread::~Thread()  //析构函数，Thread对象被销毁，但是如果此时线程仍在运行，则我们需要手动脱离线程，detached.
	{
		if(_isRunning)  //如果线程正在运行，
		{
			pthread_detach(_pthid);  //则直接放弃线程，直接与这个线程脱离联系，将运行的线程交给系统进行托管
			_isRunning=false;
		}
	}
}
