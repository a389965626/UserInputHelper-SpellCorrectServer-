 ///
 /// @file    Configuration.h
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-11 21:51:19

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__
#include <string>
#include <map>
using namespace std;

//配置文件类最好也声明为单例模式，否则到处都要用到配置文件，会在传参的过程中造成混乱
namespace cjy
{
	class Configuration  //配置文件类，我们所有的输入都是通过配置文件输入的。
	{
		public:
			string &getmyip();
			unsigned int getmyport();
			string &getmydict();
			string &getmyindex();
			unsigned int getmythreadNum();
			unsigned int getmyqueSize();
			string &getmycache();
			static Configuration* ConfCreateInstance(); 

		private:
			map<string,string> &getConfigMap();  //获取存放配置文件内容的map，这个map实际就是本配置文件对象的map;
			
			Configuration()  //构造函数声明为虚函数，定义单例模式
			:_filepath("/home/chenjunyu/20180708-item1/data/myconfiguration")
			{}

		private:
			class ConfAutoRelease   //内部类，用于定义一个自动析构的对象，执行单例对象的自动回收
			{
				public:
					~ConfAutoRelease()
					{
						if(ConfpSingle)
						{
							delete ConfpSingle;
						}
					}
			};
		private:
			static Configuration *ConfpSingle;
			static ConfAutoRelease _Confauto;
			string _filepath;  //配置文件的存放位置，考虑自己实际上先在磁盘中写好一个配置文件，然后在读取即可。
			map<string,string> _configMap;  //配置文件的内中，将其存放到map中去
	};
}
#endif

