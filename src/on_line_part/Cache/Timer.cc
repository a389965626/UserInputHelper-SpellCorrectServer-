 ///
 /// @file    Timer.cc
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-10 20:15:41

#include "../../../include/on_line_part/Cache/Timer.h"
#include <sys/timerfd.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>
#include <iostream>
using namespace cjy;
using namespace std;
int Timer::createTimerfd()
{
	int fd=timerfd_create(CLOCK_REALTIME,0);
	if(fd==-1)
	{
		perror("timerfd_create");
	}
	return fd;
}

Timer::Timer(int initalTime,int intervalTime,TimerCallback &&cb)
:_fd(createTimerfd())
,_initialTime(initalTime)
,_intervalTime(intervalTime)
,_cb(std::move(cb))
,_isStarted(false)
{}

void Timer::setTimer(int initalTime,int intervalTime)
{
	struct itimerspec new_value;
	new_value.it_value.tv_sec=initalTime; //定时器的开始时间是在几秒之后？
	new_value.it_value.tv_nsec=0;
	new_value.it_interval.tv_sec=intervalTime;  //定时器的间隔时间是几秒？
	new_value.it_interval.tv_nsec=0;
	int ret=timerfd_settime(_fd,0,&new_value,NULL);
	if(ret==-1)
	{
		perror("timerfd_settime");
	}

}

void Timer::start() //开始进行监听文件文件描述符，观察是否有超时时间产生,并且执行相应的操纵。
{
	struct pollfd pfd;
	pfd.fd=_fd;
	pfd.events=POLLIN;
	_isStarted=true;
	//开始之前先设置定时器的开始时间和间隔时间
	setTimer(_initialTime,_intervalTime); 
	while(_isStarted)   //循环监听文件描述符，观察是否有超时发生
	{
		int nready=poll(&pfd,1,5000);  //监听的等待时间是5s，监听的描述符是一个。
		if(nready==-1 && errno==EINTR)
		{
			continue;
		}
		else if(nready==-1)
		{
			perror("poll error");
			exit(EXIT_FAILURE);
		}
		else if(nready==0)
		{
			cout << "timer poll timeout" << endl;
		}
		else
		{
			if(pfd.revents & POLLIN)
			{
				handleRead();  //先处理定时器，再执行回调函数。
				if(_cb)
				{
					_cb();
				}
			}
		}
	}
}
void Timer::stop()
{
	setTimer(0,0); //先听停止定时器，将开始时间和间隔时间，如果都为0则表示停止定时器
	_isStarted=false;   //这个方法必须在另外一个现场执行。如果两个都在同一个线程执行，那么Timer.start执行之后永远不会执行到stop方法。
}

void Timer::handleRead()
{
	uint64_t howmany;
	int ret=read(_fd,&howmany,sizeof(uint64_t));
	if(ret!=sizeof(uint64_t))
	{
		perror("read error,timer howmany");
	}
}

Timer::~Timer()
{
	if(_isStarted)
	{
		stop();
	}
}

