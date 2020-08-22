 ///
 /// @file    ThreadPool.cc
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-05 19:22:15

#include "../../../include/on_line_part/ThreadPool/ThreadPool.h"
#include "../../../include/on_line_part/ThreadPool/Thread.h"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
using namespace cjy;
using namespace std;

void ThreadPool::start()  //开始运行线程池
{
	for(size_t i=0 ; i!=_pthreadNum; ++i)
	{
		shared_ptr<Thread> pthread(new Thread(std::bind(&ThreadPool::threadFunc,this),to_string(i)));//将下标作为子线程的名字并
		//转为string数据再传过去。！！！！！！！！！！！！
		//直接创建具体的Thread对象，然后将bind参数传递过来
		_threads.push_back(std::move(pthread)); 
	}
	//对于vector _threads中的每一个元素（即智能指针），都开始运行其所指向的工作线程
	for(auto &pThread : _threads)
	{
		pThread->start();
	}
}

void ThreadPool::addTask(Task &&task)
{
	_taskque.push(std::move(task));
}

Task ThreadPool::getTask()
{
	return _taskque.pop();
}

void ThreadPool::threadFunc()  //线程函数，即线程需要做的事情,线程一直都在取任务，每个线程都在取任务，因此一直在执行。
{
	//从任务队列获取任务，获取到的话就做事，没得到就一直在那里等
	while(!_isExit)  //当退出标志为false时，则一直执行线程函数；一旦遇到_isExit为true，就会退出循环，即退出子线程函数
	{
		Task task=getTask(); //得到任务
		if(task)
		{
			task(); //如果得到了任务的话，就执行run方法，执行Task派生类实际的操作。
		}
	}
}

void ThreadPool::stop()  //关闭线程池，让各个线程回收  ,注意，有一点，有可能当主线程执行到stop时，添加任务和执行任务还没有结束
	//此时会导致提前退出，因此，我们在stop函数中应该保证当所有的任务全部添加完并执行完之后才才能退出。
{
	//可以考虑当taskque任务队列不为空时，我们等待它为空，此时自然没有任务，任务全部正在执行或者已经执行完毕，此时才可以设置
	//退出标志并退出
	
	if(!_isExit)  //当退出标志为false时，将退出标志置为true
	{
		while(!_taskque.empty())  //如果任务队列不为空，表明还有队列中还有任务等待执行，因此稍微等一会，等任务队列为空时，
			//我们再去执行回收操作
		{
			sleep(2); //等一会
		}
		//此时执行到下面，表明任务队列已经为空但是队列为空并不代表已经执行完了，此时有可能线程还在执行任务。
		cout << "_taskqu is empty" << endl;

		_isExit=true;//表示要退出线程池了

		//但是，当执行到这里时会出现新的问题（死锁），由于当任务队列为空时，此时由于子线程执行的较快，在我们将_isExit设为true之前
		//，此时可能会已经有子线程等待在gettask上（threadFunc函数中），那么这时由于任务队列已经为空，这个子线程会一直阻塞在等待
		//gettask上，而主线程中的stop方法中的join方法又在那里等待子线程结束并回收，因此出现循环等待，产生死锁。
		//由于问题出在_notempty.wait()之上，因此我们只需要唤醒所有的子线程，通过_notempth.broadcast(),韩星所有等待在
		//tempty.wait()之上的线程，即可解决问题
		
		_taskque._notEmpty_broadcast();//执行广播操作，唤醒所有等待在gettask的子线程，此后gettask便不会阻塞。
		//但是此时仍然会回到while(empty())循环继续阻塞，为了不静茹while循环，我们设置一个flag=false，此时聚会突出循环，pop成功
		//返回，然后再次判断_isExit时发现为true，边成功推出，之后主线程的join成功，便不会再死锁，线程池成功退出。

		for(auto &pThread : _threads)
		{
			pThread->join();//回收每个子线程。
		}
	}
}

ThreadPool::~ThreadPool()
{
	if(!_isExit) //当退出标志为false时，直接调用stop方法
	{
		stop(); 
	}
}


