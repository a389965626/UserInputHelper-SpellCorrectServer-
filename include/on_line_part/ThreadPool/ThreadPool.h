///
/// @file    ThreadPool.h
/// @author  a389965626(389965626@qq.com)
/// @date    2018-07-04 23:57:01
///

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include "TaskQueue.h" //任务队列
#include <memory>
#include <vector>
#include <functional>
using namespace std;

namespace cjy
{
	class Thread;
	class WorkThread;
	using Task=function<void()>;  //将Task定义为回调函数，将具体要执行的回调函数加入到任务队列
	class ThreadPool
	{
		friend WorkThread;
		public:
			ThreadPool(size_t pthreadNum,size_t queSize)  //构造函数
			:_pthreadNum(pthreadNum)
			,_queSize(queSize)
			,_taskque(_queSize)
			,_isExit(false)
			{
				_threads.reserve(_pthreadNum);
			}
			~ThreadPool();//析构函数
			void start();//开始运行线程池
			void stop();//结束运行线程池
			void addTask(Task &&); 
		private:
			Task getTask();  
			void threadFunc(); 
		private:
			size_t _pthreadNum;//vector预先设置的大小，代表线程池有多少个线程
			size_t _queSize;   //初始化_taskque任务队列的大小
			vector<shared_ptr<Thread>> _threads;  //用vector来存储线程池，在vector中存储的是指向基类Thread的指针。
			TaskQueue _taskque;//组合关系，线程池中有TaskQueue任务队列，存放Task的指针,从而访问具体的Task类中的处理方法
	        bool _isExit;//退出标志，标志是否退出线程池
	};
}//end of namespace cjy 
#endif

