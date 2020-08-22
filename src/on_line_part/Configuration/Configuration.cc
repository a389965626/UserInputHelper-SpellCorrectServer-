 /// @file    Configuration.cc
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-11 22:24:11

#include "../../../include/on_line_part/Configuration/Configuration.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace cjy;
using namespace std;

Configuration * Configuration::ConfpSingle=ConfCreateInstance();  //饱汉模式，提前将配置文件的指针申请并new好，不会造成多线程的冲突。
Configuration::ConfAutoRelease Configuration::_Confauto;

Configuration *Configuration::ConfCreateInstance()
{
	if(ConfpSingle==NULL)
	{
		ConfpSingle=new Configuration();
	}
	return ConfpSingle;
}

map<string,string> & Configuration::getConfigMap()  //通过配置文件的路径获取需要得到的ip,port,词典，索引文件等信息
{
	ifstream ifs(_filepath);
	if(!ifs)
	{
		cout << "error ifs" << endl;
		static map<string,string> temp;
		return temp;
	}
	string line;
	while(getline(ifs,line),!ifs.eof())
	{
		string word1,word2;
		istringstream iss(line);//字符串输入流
		iss >> word1;//读入关键字
		iss >> word2; //读入具体内容
		_configMap[word1]=word2;
	}
	return _configMap;
}

string &Configuration::getmyip() 
{
	getConfigMap();
	return _configMap["myip"];
}

unsigned int Configuration::getmyport()
{
	getConfigMap();
	string sport=_configMap["myport"];
	return (unsigned int)atoi(sport.c_str());
}

unsigned int Configuration::getmythreadNum()
{
	getConfigMap();
	string threadNum=_configMap["mythreadNum"];
	return (unsigned int)atoi(threadNum.c_str());
}

unsigned int Configuration::getmyqueSize()
{
	getConfigMap();
	string queSize=_configMap["myqueSize"];
	return (unsigned int)atoi(queSize.c_str());
}

string &Configuration::getmydict()
{
	getConfigMap();
	return _configMap["mydict"];
}

string &Configuration::getmyindex()
{
	getConfigMap();
	return _configMap["myindex"];
}

string &Configuration::getmycache()
{
	getConfigMap();
	return  _configMap["mycache"];
}


