 ///
 /// @file    Mydict.h
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-13 11:30:31
 
//词典类，通过配置文件来读取相应的路径信息。将词典类声明为单例模式，因为所有的使用的词典都应该是相同的词典类。
#ifndef __MYDICT_H__
#define __MYDICT_H__
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <utility>
using namespace std;

namespace cjy
{
	class Mydict    //单例模式的词典类
	{
		public:
			vector<pair<string, int> > & getDict();  //获取词典，为外提供接口
			unordered_map<string, set<int> > & getIndexTable();  //获取索引表，为外界提供接口
			void init();  //通过中文和英文词典文件路径初始化词典（这里是通过配置文件的方式）
			static Mydict *MydictCreateInstance();

		private:        //将构造函数声明为私有的，设计单例模式
			Mydict()  //直接通过配置文件进行输入词典。
			{}

		
		private:
			class MydictAutoRelease   //内部类，用于定义一个自动析构的对象，执行单例对象的自动回收
			{
				public:
					//MydictAutoRelease();
					~MydictAutoRelease()
					{
						if(MydictpSingle)
						{
							delete MydictpSingle;
						}
					}
			};
		private:
			static Mydict *MydictpSingle;
			static MydictAutoRelease _Mydictauto;
			vector<pair<string,int> > _dict;  //用于存读独到的原始词典的vector
			unordered_map<string,set<int> > _indextable;  //用于存放索引表
	};
}
#endif

