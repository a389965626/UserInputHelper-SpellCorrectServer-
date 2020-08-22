 ///
 /// @file    Condition.cc
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-06-11 23:00:28
 ///

#include "../../../include/on_line_part/ThreadPool/Condition.h"
#include "../../../include//on_line_part/TcpServer/MutexLock.h"

using namespace cjy;

//构造函数
Condition::Condition(MutexLock &mutex)
:_mutex(mutex)
{
	pthread_cond_init(&_cond,NULL);
}
//析构函数
Condition::~Condition()
{
	pthread_cond_destroy(&_cond);
}
//wait函数
void Condition::wait()
{
	pthread_cond_wait(&_cond,_mutex.getMutexLockPtr());//_mutex为引用，因此得在MutexLock类中新写一个能够返回互斥锁地址的成员函数
}
//signal函数
void Condition::signal()
{
	pthread_cond_signal(&_cond);
}
//broadcast函数
void Condition::broadcast()
{
	pthread_cond_broadcast(&_cond);
}

