 ///
 /// @file    DictProducer.cc
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-11 14:56:54


#include "../../include/off_line_part/DictIndexProducer.h"
//#include "../../include/on_line_part/Configuration/Configuration.h"
#include "../../include/on_line_part/SpellCorrectServer/Editdistance.h"
#include "../../src/on_line_part/SpellCorrectServer/Editdistance.cc"
#include "../../include/off_line_part/rmPunct.h"
#include <fstream>
#include <sstream>
using namespace cjy;


DictProducer::DictProducer(SplitToolCppJieba &splitTool)  //在新建DictProducer对象时，将待解析的原始文件路径传过来
:_splitTool(splitTool)
{}

//读取英文原始文件，并将其单词和词频存入 _map_dict<string,int>中。
void DictProducer::read_english_fromfile_tomapdict()   //读取文件，将word,frenqucy 存入map中
{
	ifstream ifs("../../data/english.txt"); //文件输入流
	if(!ifs)
	{
		cout << "ifs is rot" << endl;
		return;
	}
	string line;
	while(getline(ifs,line),!ifs.eof())
	{
		process_line(line); //先对每一行做处理
		string word;
		istringstream iss(line);
		while(iss >> word,!iss.eof())  //字符串输入流
		{
			++_english_map_dict[word];  //直接++即可，不存在的话会加入到map中去
		}
	}
}

//处理每一行的英文
void DictProducer::process_line(string &line) //处理每一行，让每一行只有单词，没有其他的内容（符号改为空格，大写改为小写）
//大写改小写，不是英文字母的都改为空格
{
	for(auto it=line.begin() ; it!=line.end() ; ++it)  //这里可以使用范围for语句
	{
		if(!isalpha(*it))
		{
			*it=' ';
		}
		else
		{
			*it=tolower(*it);  //保存为小写字母
		}
	}
}

//读取中文的原始文档，将词语和词频放入_chinese_map_dict<string,int>中。
void DictProducer::read_chinese_fromfile_tomapdict()
{
	string s(rmPunt());  //先读取所有的中文文档，得到一个string
	vector<string> chinese_words(_splitTool.cutsentence(s));  //chinese_words中的每个元素是一个中文词语。
	for(const auto &it : chinese_words)
	{
		if(it!=" ")   //不存入空格。
			++_chinese_map_dict[it];  //将对应中文词语的词频++，没有的话就会加入到集合中。（并且对应中文是有序的）
	}
}

//将map词典中的（单词，词频）放入vector词典，以便得到索引。
void DictProducer::get_vectordict_frommapdict()
{
	for(const auto &it : _english_map_dict)  //先存英文字典，再存中文字典。
	{
		_vector_dict.push_back(it);
    }  //得到英文vector。在这里考虑将得到vector先做成英文索引并保存到

	for(const auto &it : _chinese_map_dict)
	{
		_vector_dict.push_back(it);
	}
	//vector中先存的英文，然后存的中文  vector<pair<string,int>>。
}

//从_vector_dict<pair<string,int>>中得到索引，此时不用存入内存，直接将其写到磁盘索引即可。
void DictProducer::store_index_tofile()
{
	//这里得遍历vector_dict的string中的每一个元素（英文)，考虑用map<string,set<int>>类型来的到索引并存到文件
	map<string,set<int>> index;
	int cnt=0;//表示vector的下标。
	for(const auto &it : _vector_dict)  //_vector_dict vector<pair<string,int>>。
	{
		for(int i=0 ; i<(int)it.first.size() ; )
		{
			int l=nBytesCode(it.first[i]);  //得到一个字符或一个中文字的大小
			string s(it.first.substr(i,l)); //得到具体的英文字符或者中文字
			index[s].insert(cnt);  //放入到索引map中。
			i+=l; //下一个英文字符或者中文字
		}
		++cnt;  //下标++。
	}
	ofstream ofs("/home/chenjunyu/20180708-item1/data/indexfile.txt"); //文件输出流
	if(!ofs)
	{
		cout << "ofs error" << endl;
	}
	for(const auto &it : index)
	{
		ofs << it.first << " " ;
		for(const auto &i : it.second)
		{
			ofs << i << " "; //输出set集合。里面存放的是包含索引字母的所有单词的在vector中的下标
		}
		ofs << endl;
	}
}

//将_map_dict<string,int>存入到磁盘文件中。
void DictProducer::store_dict_tofile()
{
	ofstream ofs("/home/chenjunyu/20180708-item1/data/english_dictionary.txt");//文件输出流，
	if(!ofs)
	{
		cout << "error ofs" << endl;
		return;
	}
#if 0
	for(const auto &it : _vector_dict)
	{
		ofs << it.first << " " << it.second << endl;
	}
#endif
	//分别存储为英文词典和中文词典
	for(const auto &it : _english_map_dict)
	{
		ofs << it.first << " " << it.second << endl;  //写入文件
	}
	ofs.close();

	ofs.open("/home/chenjunyu/20180708-item1/data/chinese_dictionary.txt");
	if(!ofs)
	{
		cout << "error ofs" << endl;
		return;
	}
	for(const auto &it : _chinese_map_dict)
	{
		ofs << it.first << " " << it.second << endl;
	}
	ofs.close();
}

void DictProducer::show_dict() const
{
	for(const auto &it : _english_map_dict)
	{
		cout << it.first << " " << it.second << endl;
	}
	for(const auto &it : _chinese_map_dict)
	{
		cout << it.first << " " << it.second << endl;
	}
}


void DictProducer::build_dict()  //用于隐藏内部的接口实现
{
	read_english_fromfile_tomapdict(); //首先读入英文到_map_dict<string,int>中。
	read_chinese_fromfile_tomapdict(); //然后读入中文到_map_dict<string,int>中。
	store_dict_tofile();  //最后将内存的_map_dict<string,int>写到磁盘。
}

void DictProducer::build_index() //用于隐藏内部的接口实现
{
	read_english_fromfile_tomapdict();
	read_chinese_fromfile_tomapdict();
	get_vectordict_frommapdict();
	store_index_tofile();
}



