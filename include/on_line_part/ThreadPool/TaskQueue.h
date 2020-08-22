///
/// @file    Task.h
/// @author  a389965626(389965626@qq.com)
/// @date    2018-07-04 09:49:33

#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__
#include "../TcpServer/MutexLock.h"
#include "Condition.h"
#include <queue>
#include <functional>
using namespace std;

namespace cjy
{
		using Task=function<void()>;
		class TaskQueue   //生产者，消费者之间的任务队列，即缓冲区，存放已经生产的待消费的产品或任务
		{
			public:
				TaskQueue(size_t queSize)//默认构造函数
					:_queSize(queSize)
					,_notFull(_mutex)
					 ,_notEmpty(_mutex)
					 ,_flag(true)
			{}

				void push(Task &&);//进队
				Task pop();//出队
				bool empty() const;
				bool full() const;
				void _notEmpty_broadcast()
				{
					_flag=false; //将_flag设为flase，便不会再进入while循环
					_notEmpty.broadcast();
				}
			private:
				size_t _queSize;  //队列最大容量
				queue<Task> _que;//队列，用于存放元素
				MutexLock _mutex; //队列访问的互斥锁
				Condition _notFull; //条件变量，由消费者发出
				Condition _notEmpty;// 条件变量，由生产者发出
				bool _flag;//用于帮助线程池正常退出的glag。
		};
}
#endif

