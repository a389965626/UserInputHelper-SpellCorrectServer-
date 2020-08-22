 ///
 /// @file    TimerThread.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-07-06 16:46:17
 ///
 
#include "../../../include/on_line_part/Cache/TimerThread.h"
using namespace cjy;

TimerThread::TimerThread(int initialTime, int intervalTime, TimerCallback && cb)  //这里的回调函数实际上是cachemanager的
//periodicUpdateCaches()方法。
: _timer(initialTime, intervalTime, std::move(cb))
, _subThread(std::bind(&Timer::start, &_timer)) //将Timer的start方法给子线程执行。
, _isStarted(false)
{}

void TimerThread::start()   //必须让Timer的start方法在子线程中执行，让Timer的stop方法在另一个线程执行，否则无法退出start方法。
{
	_isStarted = true;
	_subThread.start();
}

void TimerThread::stop()
{
	if(_isStarted) {
		_timer.stop();
		_subThread.join();
		_isStarted = false;
	}
}

TimerThread::~TimerThread()
{
	if(_isStarted)
		stop();
}
