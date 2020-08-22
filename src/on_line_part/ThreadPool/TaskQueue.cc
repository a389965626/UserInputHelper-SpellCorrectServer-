///
/// @file    TaskQueue.cc
/// @author  a389965626(389965626@qq.com)
/// @date    2018-07-14 18:01:38
///

#include "../../../include/on_line_part/ThreadPool/TaskQueue.h"
using namespace cjy;

void TaskQueue::push(Task &&task)
{
	//访问队列前得先加锁,使用互斥锁的保护类
	MutexLockGuard autoLock(_mutex);
	while(full())  //队列满便等待队列不满的信号的到来
	{
		_notFull.wait();
	}
	_que.push(task);//入队,此时队列不为空则通知所有线程都可以来取，
	_notEmpty.broadcast();
}

Task TaskQueue::pop()
{
	MutexLockGuard autoLock(_mutex);
	while(_flag && empty())  //如果队列为空，则等到notempty信号的到来
	{
		_notEmpty.wait();  //一直阻塞在这里，队列为空永远不会退出，但是我们有一种情况：当队列为空时，我们不想再在队列
		//中添加任务，想要退出。但是此时会一直无法退出，因为一直在等待队列不空的条件成立。为了能够在阻塞时退出，我们可以
		//考虑用_notEmpty.broadcast()来唤醒所有的阻塞在这上面的线程
	}
	//为了让队列为空也能成功返回，的进行判断
	if(_flag)
	{
		Task task=_que.front();
		_que.pop(); //出队,队列为空时是不能进行这个操作的
		_notFull.broadcast();  //当消费一个队列不满，通知生产者可以进行生产数据并放入缓冲区
		return task;
	}
	else
	{
		return NULL;
	}
}

bool TaskQueue::empty() const
{
	return _que.size()==0; 

}


bool TaskQueue::full() const
{
	return _que.size()==_queSize; 
}

