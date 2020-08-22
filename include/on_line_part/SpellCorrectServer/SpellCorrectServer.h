 /// @file    SpellCorrectServer.h
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-12 11:43:02

#ifndef __SPELLCORRECTSERVER_H__
#define __SPELLCORRECTSERVER_H__
#include "../ThreadPool/ThreadPool.h"
#include "../TcpServer/TcpServer.h"

namespace cjy
{
	class SpellCorrectServer  //文本纠错服务器，是一个最具体的服务器，包括3个回调函数的实现，以及其他的作用。
	{
		public:
			SpellCorrectServer();
			void start();//开始提供服务（文件纠错服务）
			//3个贯穿整个的回调函数
			static void onConnection(const TcpConnectionPtr &conn);
			static void onMessage(const TcpConnectionPtr &conn);
			static void onClose(const TcpConnectionPtr &conn);
		private:
			TcpServer _tcpServer;  
			static ThreadPool _threadpool;
	};
}
#endif

