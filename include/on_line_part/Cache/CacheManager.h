 /// @file    CacheManager.h
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-16 18:28:15

#ifndef __CACHEMANAGER_H__
#define __CACHEMANAGER_H__

#include "Cache.h"
#include <vector>
namespace cjy              //Cache文件的路径通过配置文件获得。
{
	class CacheManager
	{
		public:  //全部设为静态成员函数，可以在TimerThread中进行访问
			static void init();  //系统启动时，首先会从磁盘文件中读取缓存信息（0号cache执行）。即定义一个cache去读，然后依次
			//进队,将其放到vector<Cache> _cacheList中。
			static void periodicUpdateCaches(); //定时更新所有的缓存，这个操作很重要。定时更新所有的内存Cache以及物理磁盘Cache。
			static Cache& getCache(size_t idx);  //获取某个缓存信息。
		private:
			static vector<Cache> _cacheList; //此vector用于存放每一个子线程所各自对应的缓存Cache。静态成员，在类外进行初始化
			//，初始为空。
			static Cache _newcache;   //用于保存最新的cache。
	};
}

#endif

