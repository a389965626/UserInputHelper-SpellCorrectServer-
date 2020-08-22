 ///
 /// @file    DictProducer.h
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-11 09:

#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__
#include "SplitToolCppJieba.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
using namespace std;
namespace cjy
{
	class DictProducer
	{
		public:
			DictProducer(SplitToolCppJieba &splitTool); //构造函数，将语料库的文件路径传过来
			
			void build_dict();   //建立原始词典（单词，词频）
			void show_dict() const; //查看词典，作为测试用
		
			void build_index();  //建立索引文件。调用内部接口。
		
		private:
			void read_english_fromfile_tomapdict();//从原始英文文档初始化词典的英文部分（单词，词频）
			void process_line(string &line);//处理每一行

			//这里使用cppjieba分词库来进行分词。
			void read_chinese_fromfile_tomapdict(); //从原始中文文档中读入，并初始化词典的中文部分（中文单词，词频）

			void get_vectordict_frommapdict(); //从map词典（单词，词频）中得到vector词典。用于得到索引。

			void store_dict_tofile();  //将词典写入文件
	        void store_index_tofile(); //将索引写到文件中。
		private:
			map<string,int> _english_map_dict; //专门存放英文map，
			map<string,int> _chinese_map_dict; //专门存放中文map。

			vector<pair<string,int>> _vector_dict; //存放中英文map的pair

			unordered_map<string,set<int>> _english_index;  //存放英文索引
			unordered_map<string,set<int>> _chinese_index;  //存放中文索引

			SplitToolCppJieba &_splitTool; //分词工具，处理中文的每一行或者英文的每一行，通过一个SplitTool的对象来进行分词
	};
}
#endif

