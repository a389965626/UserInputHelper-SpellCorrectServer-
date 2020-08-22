 ///
 /// @file    TimerThread.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-06 16:43:53
 ///
 
#ifndef __WD_TIMERTHREAD_H__
#define __WD_TIMERTHREAD_H__

#include "Timer.h"
#include "../ThreadPool/Thread.h"
#include <functional>

namespace cjy
{
	
class TimerThread
{
public:
	using TimerCallback = std::function<void()>;
	TimerThread(int, int, TimerCallback && cb); //包含Timer。

	void start();
	void stop();

	~TimerThread();

private:
	Timer _timer;
	Thread _subThread;
	bool _isStarted;
};
}//end of namespace wd

#endif
