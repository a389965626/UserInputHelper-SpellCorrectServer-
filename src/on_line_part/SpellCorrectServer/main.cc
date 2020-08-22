 ///
 /// @file    FinalServer.cc
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-14 16:33:31
 ///

#include "../../../include/on_line_part/SpellCorrectServer/SpellCorrectServer.h"
#include "../../../include/on_line_part/Cache/TimerThread.h"
#include "../../../include/on_line_part/Cache/CacheManager.h"
#include <functional>
using namespace std;
using namespace cjy;

int main()
{
	//开始运行之前先读入缓存并初始化。
	CacheManager::init();
	TimerThread timerthread(3,5,std::bind(&CacheManager::periodicUpdateCaches));//3秒后启动定时器，每隔5秒定时器响应一次。
	timerthread.start();  //定时器线程开始运行。

	SpellCorrectServer server;  //文本纠错服务器对象
	server.start();   //开始进行服务

	return 0;
}



