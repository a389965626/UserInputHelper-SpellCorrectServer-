 ///
 /// @file    CacheManage.cc
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-17 10:24:52
 ///

#include "../../../include/on_line_part/Cache/CacheManager.h"
#include "../../../include/on_line_part/Cache/Cache.h"
#include "../../../include/on_line_part/Configuration/Configuration.h"
#include <iostream>
using namespace cjy;
using namespace std;

Configuration *conf=Configuration::ConfCreateInstance();
vector<Cache> CacheManager::_cacheList;  //静态数据成员在类外进行初始化，初始时只有threadNum个空的Cache对象。
Cache CacheManager::_newcache;

void CacheManager::init()   //cache的管理类，负责将多个cache对象放到vector中。初始化，从磁盘cache文件读入信息，并依次初始化
	//vector中的所有的cache，cache的个数同子线程的个数相同。因此得用到配置文件来求子线程数
{
	Cache temp;
	temp.readFromFile();  //先从磁盘cache中读取缓存信息，然后再依次初始化所有的内存cache

#if 0
	for(const auto &it : temp.gettable())  //从缓存文件读到的单词是没有换行符的。
	{
		cout << it.first << " " << it.second << endl;;
	}
#endif

	for(int i=0; i<(int)conf->getmythreadNum() ; ++i)  //循环复制构造（假定10个子线程）
	{
		_cacheList.push_back(temp); //初始时，所有的cache都是一样的。
	}
	_newcache=temp; //保存最新的cache。
	cout << " 内存 cache init success" << endl;
}

void CacheManager::periodicUpdateCaches()  //定时更新所有的缓存，
{
	//首先得检查cache[0]自身偶没有需要更新到磁盘cache文件的东西。
	for(const auto &it : _cacheList[0].gettable()) 
	{
		if(_newcache.gettable().find(it.first)==_newcache.gettable().end()) //如果没有找到it这个类型，证明it是刚加入到cache[0]的
			//应该写到磁盘
		{
			_cacheList[0].writeToFile(it.first,it.second);
		}
	}
	for(int i=1; i<(int)conf->getmythreadNum() ; ++i)
	{
		_cacheList[i].update(_cacheList[0]);  //依次检查1-9号cache，用来更新0号cache以及磁盘cache。
	}
	//然后在用0号cache依次给1-9号cache赋值，让所有的cache都保存为最新的cache。
	_newcache=_cacheList[0];//保存最新的cache
	for(int i=1; i<(int)conf->getmythreadNum() ; ++i)
	{
		_cacheList[i]=_cacheList[0];
	}

	cout << "cache update" << endl;
}

Cache& CacheManager::getCache(size_t idx)  //获取某个缓存信息。
{
	return _cacheList[idx];  //返回的是引用，因此可以对这个原有的_cacheList[idx]进行查询以及更新操作。
}


