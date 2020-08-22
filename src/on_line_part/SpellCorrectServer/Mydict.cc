///
/// @file    Mydict.cc
/// @author  a389965626(389965626@qq.com)
/// @date    2018-07-13 11:45:07
///

#include "../../../include/on_line_part/SpellCorrectServer/Mydict.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
using namespace cjy;

Mydict *Mydict::MydictpSingle=MydictCreateInstance();//饱汉模式
Mydict::MydictAutoRelease Mydict::_Mydictauto;

Mydict *Mydict::MydictCreateInstance()
{
	if(MydictpSingle==NULL)
	{
		MydictpSingle=new Mydict();
	}
	return MydictpSingle; 
}

void Mydict::init()  //初始化词典
{
	string dictpath1("/home/chenjunyu/20180708-item1/data/chinese_dictionary.txt");  //得到原始词典的路径
	string dictpath2("/home/chenjunyu/20180708-item1/data/english_dictionary.txt");
	string indexpath("/home/chenjunyu/20180708-item1/data/indexfile.txt"); //得到索引文件的路径

	//首先读取原始词典
	ifstream ifs(dictpath2);  //单词+词频。
	if(!ifs)
	{
		cout << "error ifs" << endl;
		return;
	}
	string line;
	string word;
	int frequency;

	//cout << "before read dict" << endl;
	//先读英文的原始词典。
	while(getline(ifs,line),!ifs.eof()) 
	{
		istringstream iss(line);
		iss >> word;
		iss >> frequency;
		_dict.push_back(make_pair(word,frequency)); //读到vector中去
	}
	ifs.close();

	//再读中文的原始词典。
	ifs.open(dictpath1);
	if(!ifs)
	{
		cout << "error ifs" << endl;
		return;
	}
	while(getline(ifs,line),!ifs.eof()) 
	{
		istringstream iss(line);
		iss >> word;
		iss >> frequency;
		_dict.push_back(make_pair(word,frequency)); //读到vector中去
	}

	//再读索引文件。
	ifs.close();
	ifs.open(indexpath);
	if(!ifs)
	{
		cout << "error ifs" << endl;
		return;
	}
	while(getline(ifs,line),!ifs.eof())
	{
		istringstream iss(line);
		iss >> word; //先考虑输入索引的英文字母（关键字），然后再考虑循环输入所有的下标到map的set中去。从而成功读入索引文件
		int index;
		while(iss >> index,!iss.eof())  //之后循环读入剩下的关于vector的下标。
		{
			_indextable[word].insert(index); //插入到索引文件表indextable中去。得到索引文件
		}
	}
}

vector<pair<string,int>> &Mydict::getDict() //得到词典文件
{
	return _dict;
}

unordered_map<string,set<int>> &Mydict::getIndexTable()  //得到索引文件
{
	return _indextable;
}

