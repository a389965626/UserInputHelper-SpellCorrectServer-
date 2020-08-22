 ///
 /// @file    Cache.h
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-16 17:14:56
 ///

#ifndef __CACHE_H__
#define __CACHE_H__

#include <unordered_map>
#include <string>
using namespace std;
namespace cjy
{
	class Cache
	{
		public:
			Cache(){}  //默认的无参构造函数。
			Cache(const Cache &cache); //复制构造函数，其主要目的便是用0号缓存来复制构造其他缓存。每次程序启动时，先直接构造并初始化
			//vector中第0个Cache，之后便可以直接进行复制构造其余的Cache。
			Cache & operator=(const Cache &rhs); //用rhs（0号Cache）对其他的Cache进行赋值，所以重载赋值运算符。
			
			void readFromFile(); //从磁盘缓存文件中读取缓存信息。程序开始时做的事情，且仅让0号Cache来做。
			
			void update(Cache &rhs);  //更新0号Cache的操作。将所有1-9号Cache的内容写到0号cache中。然后再用0号Cache来更新
			//1-9号Cache。在update中调用rhs的getCache方法，调用rhs的addElement，并调用自己的writeToFile方法。
			
			unordered_map<string,string>  gettable() const;//获得本Cache的unordered_map的引用，以便更新。
			
			void addElement(const string &key,const string &value);  //将缓存的信息加入无序容器unordered_map。无序容器
			//unordered_map中存放的是查询字符串到打包好的json字符串的映射关系。
		
			void writeToFile(const string &key,const string &value);  //将缓存信息写入到文件中。我们测试时由于服务器会很快断开，因此每隔10s钟
			//将缓存写回磁盘Cache（一般在服务器不关闭时是建设成5-10min）。
			//磁盘文件中存放的是一个关键字string（待查询单词）和这个单词所对应的json字符串（已经打包好的）
		
		private:
			unordered_map<string,string> _table;  //存储缓存信息的map，即内存cache。
	};
}
#endif
