 ///
 /// @file    Timerfd.h
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-10 17:55:16
 
#ifndef __TIMER_H__
#define __TIMER_H__
#include <functional>
namespace cjy
{
	class Timer      //定时器类，能够进行定时操作
	{
		public:
			using TimerCallback=std::function<void()>;
			Timer(int initalTime,int intervalTime,TimerCallback &&cb);
			~Timer();
			void start();//开始执行定时器，监听定时器是否到时而产生读事件，到时的话监听的描述符就会返回
			void stop();//停止执行定时器，不再去监听Timer的文件描述符
		private:
			int createTimerfd();
			void setTimer(int initalTime,int intervalTime);  //设置定时器的开始时间和监听的时间间隔,在start开始执行定时器时调用
		    void handleRead(); //监听到超时时，先固定做的一些事情
		private:
			int _fd;
			int _initialTime;
			int _intervalTime;
			TimerCallback _cb;//回调函数，每当定时器到固定的时间时，就去执行某一个任务。
			bool _isStarted; //表明是否开始进行监听
	};
}

#endif

