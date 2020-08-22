 ///
 /// @file    Condition.h
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-06-11 22:36:39
#ifndef __CONDITION_H__
#define __CONDITION_H__
#include "Noncopyable.h"
#include <pthread.h>
namespace cjy
{
class MutexLock;//前向引用声明
class Condition
:Noncopyable
{
public:
	Condition(MutexLock &mutex);//将一个外部定义的锁传过来
	~Condition();

	void wait();
	void signal();
	void broadcast();//广播
private:
	pthread_cond_t _cond;//条件变量，条件变量需要与互斥锁一起使用，但是其实这个互斥锁并不属于条件变量这个类，这个锁应该是在
	//外部，与某一个临界资源相关。因此这个与condition相关的互斥锁应该是在外部已经定义好的，我们只是在内部使用这个锁而已
	//因此，我们在类内部只能呢使用这个锁，因此得在类内部声明一个这个锁的引用，在构造函数中将这个外部的互斥锁传过来。
	MutexLock &_mutex;//外部互斥锁的引用,MutexLock是一个未定义的类，得使用前向引用声明来声明它是一个类，可以减少头文件的包含
};
}
#endif
