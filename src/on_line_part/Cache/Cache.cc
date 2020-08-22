 ///
 /// @file    Cache.cc
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-16 20:34:02

#include "../../../include/on_line_part/Cache/Cache.h"
#include "../../../include/on_line_part/Configuration/Configuration.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
using namespace cjy;

Configuration *mconf=Configuration::ConfCreateInstance();

Cache::Cache(const Cache &cache)  //复制构造函数
:_table(cache.gettable())
{}

Cache & Cache::operator=(const Cache &rhs)  //重载赋值运算符，考虑自复制的问题
{
	if(&rhs!=this)
	{
		//先考虑清空本对象的unordered_map<string,string> _table。
		//unordered_map<string,string> new_temp;
		_table.clear(); //清空操作。
		_table=rhs.gettable();
	}
	return *this;
}

void Cache::readFromFile() //从cache文件中读取cache信息并保存到内存Cache  unordered_map<string,string> _table。
{
	ifstream ifs(mconf->getmycache()); //文件输入流
	if(!ifs)
	{
		cout << "error ifs" << endl;
		return;
	}
	string line;
	while(getline(ifs,line),!ifs.eof())
	{
		string queryword;
		string jsonstring;
		istringstream iss(line); //字符串输入流。
		iss >> queryword;  //读入查询词。
		iss >> jsonstring;  //读入打包好的json字符串。
		//cout << queryword << jsonstring << endl;
		_table[queryword]=jsonstring;  //放入到内存cache unordered_map中。
	}
}

unordered_map<string,string>  Cache::gettable() const   //获得本对象的内存cache unordered_map
{
	return _table;
}

void Cache::addElement(const string &key,const string &value) //向本本对象的内存cache unordered_map中加入新的条目
{
	 _table[key]=value;
}

void Cache::writeToFile(const string &key,const string &value)
{
	ofstream ofs(mconf->getmycache(),ios::app);
	if(!ofs)
	{
		cout << "error ifs" << endl;
		return;
	}
	ofs << key << " " << value;   //不要输出查询词的换行符
}

void Cache::update(Cache &rhs)   //更新0号Cache的内存cache unordered_map
{
	unordered_map<string,string> temp_map(rhs.gettable());
	for(const auto &it : _table)  //循环检查本对象的内存cache
	{
		if(temp_map.find(it.first)==temp_map.end())  //如果在rhs的内存缓存中没有找到对应的关键字，就把这个条目插入到rhs的cache中
		//并且同时把它写到磁盘
		{
			rhs.addElement(it.first,it.second);
			writeToFile(it.first,it.second);
		}
	}
}


