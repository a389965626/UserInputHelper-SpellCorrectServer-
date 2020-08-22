/// @file    Mytask.cc
/// @author  a389965626(389965626@qq.com)
/// @date    2018-07-12 22:35:54
///

#include "../../../include/on_line_part/SpellCorrectServer/MyTask.h"
#include "../../../include/on_line_part/SpellCorrectServer/Mydict.h"
#include "../../../include/on_line_part/Cache/CacheManager.h"  //使用缓存系统！
#include "../../../include/on_line_part/ThreadPool/Thread.h"
#include "../../../include/on_line_part/SpellCorrectServer/Editdistance.h"
#include <jsoncpp/json/json.h>  //使用jsoncpp应该包含的头文件
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <cmath>
using namespace cjy;
using namespace std;
using namespace Json;  //引出json的命名空间

void MyTask::execute()    //这个函数就是线程池的子线程应该执行的一些编解码以及计算操作（）,十分重要
{
	//cout << "before queryIndexTable" << endl;

	//首先应该去查询缓存系统，看缓存中是否有。如果有的话直接得到应打包好的json字符串。
	cout << "query word:" << _query << endl;
	//cout << "before query cache" << endl;
	
	size_t threadidx=atoi(current_thread::threadName);
	//cout << "threadidx:" << threadidx << endl;  //得到了对应Cache的下标
	if(CacheManager::getCache(threadidx).gettable().find(_query) != CacheManager::getCache(threadidx).gettable().end())
		//如果在对应Cache的unordered_map中找到了待查询的词_query，表明缓存中有这个词，直接发送缓存中的信息即可
	{
		cout << "cache success" << endl;
		_conn->sendInLoop(CacheManager::getCache(threadidx).gettable()[_query]);
	}
	else
	{
		cout << "cache fail" << endl;
		queryIndexTable();  //execute中首先执行查询任务，然后将查询结果包装成json字符串（优先队列的前十个元素），然后通过_conn对象
		//发送给IO线程，让IO线程区将这个json字符串包发送给客户端

		//cout << "after queryIndexTable" << endl;

		//得到查询结果，然后包装成json字符串。
		//cout << "before jsonencode" << endl;

		string jsonstring;
		//cout << "before encode,sizeof jsonstring " << jsonstring.size() << endl;

		jsonstring=encode();
		
		//得到jsonstring并写入到自己对应的内存缓存中。
		CacheManager::getCache(threadidx).addElement(_query,jsonstring);

		//cout << "after encode,sizeof jsonstring " << jsonstring.size() << endl;
		//cout << "after jsonencode" << endl;

		//cout << "before sendInLoop" << endl;
		_conn->sendInLoop(jsonstring); //将发送任务交给IO线程来执行。
		//cout << "after sendInLoop" << endl;
	}
}

void MyTask::queryIndexTable()   //根据收到的单词，查询单词的每个字母所对应的索引表，然后根据索引表去查询单例的词典类的词典
	//计算每个单词到查询单词的最小编辑距离，然后加入优先队列
{
	//首先先得到原始词典的vector和索引文件的map（根据词典类Mydict单例对象）
	Mydict *mydict=Mydict::MydictCreateInstance(); //得到词典类的单例对象
	mydict->init();   // 这里不用每次都init，因为是单例对象，所以只需要init一次，可以考虑再服务启动的时候init一次即可
	vector<pair<string,int>> dict(mydict->getDict()); //得到原始词典的vector
	unordered_map<string,set<int>> indextable(mydict->getIndexTable()); //得到词典的索引（以26个英文字母为索引,以及中文字为索引）
	
	set<int> index; //此集合用于存放单词的每个字母所对应索引表的并集（保证查询到的单词不重复）。
	int len=_query.size();
//	cout << "_query size = " << len << endl;
	
	for(int i=0; i<len ;)   //love，陈俊宇
	{
		int l=nBytesCode(_query[i]);  //求每一个字符的size。
		//cout << " l= " << l << endl;
		string s(_query.substr(i,l));
		
		//尝试求多个set集合的并集。
		cout << s << endl; //验证输出每个中文或英文字符。
		
		set<int> copyindex(index); //临时存储集合的变量。
		set<int> emptyindex; 
		index=emptyindex; //置空。
		set_union(indextable[s].begin(),indextable[s].end(),copyindex.begin(),copyindex.end(),inserter(index,index.begin()));
	    i+=l;  //寻找下一个字符或者中文;
	}

	for(const auto &it : index)  //对于set集合中的每一个int下标，都去vector原始词典中寻找对应的单词
	{
		//cout << it << endl;  这是下标
		string word(dict[it].first);  //得到源字符串（与待查询单词相类似的字符串）
		int iFreq=dict[it].second;   //得到其词频
		int iDist=distance(word);  //得到源字符串（与待查询单词相类似的字符串）与待查询字符串的最小编辑距离
		MyResult myresult;
		myresult._word=word;
		myresult._iFreq=iFreq;
		myresult._iDist=iDist;
		_resultQue.push(myresult);   //将查询结果插入优先级队列
	}
}

int MyTask::distance(const string & rhsWord)
{
	return ::editDistance(_query, rhsWord);
}
#if 0
int MyTask::distance(const string &source)  //实现最小编辑距离的算法，网上查询的
	//从源字符串到目标字符串的最小编辑距离。源字符串是索引文件中的单词，目标字符串是待查询的单词
{
	//step 1
	int n = _query.length(); //源字符串
	int m = source.length(); //目标字符串
	if (m == 0) return n;
	if (n == 0) return m;
	//Construct a matrix
	typedef vector< vector<int> >  Tmatrix;
	Tmatrix matrix(n + 1);
	for (int i = 0; i <= n; i++)  matrix[i].resize(m + 1);

	//step 2 Initialize

	for (int i = 1; i <= n; i++) matrix[i][0] = i;
	for (int i = 1; i <= m; i++) matrix[0][i] = i;

	//step 3
	for (int i = 1; i <= n; i++)
	{
		const char si = source[i - 1];
		//step 4
		for (int j = 1; j <= m; j++)
		{

			const char dj = _query[j - 1];
			//step 5
			int cost;
			if (si == dj){
				cost = 0;
			}
			else{
				cost = 1;
			}
			//step 6
			const int above = matrix[i - 1][j] + 1;
			const int left = matrix[i][j - 1] + 1;
			const int diag = matrix[i - 1][j - 1] + cost;
			matrix[i][j] = min(above, min(left, diag));
		}
	}//step7
	return matrix[n][m];
}
#endif


string MyTask::encode()
{
	Json::Value root; //根节点  ，json包中包含1个键值和一个数组（数组中存放10个）
	for(int i=0 ;i<=9 ;++i)
	{
		Json::Value temp_item;
		if(!_resultQue.empty())
		{
			string s=_resultQue.top()._word; //得到最高优先级的单词

		//cout << s << " " <<  _resultQue.top()._iDist << endl;

	     	_resultQue.pop();  //然后出队
    		const char *key=to_string(i).c_str();  //以下标的C风格字符串类型作为关键字
	    	temp_item[key]=s; //直接添加数组元素
	    	root.append(temp_item);
		}
		else
			break;  
		//cout << "append success" << endl;
	}
	Json::FastWriter writer; //用其来对json的value进行编码，得到一个字符串
	return writer.write(root); //进行json并返回打包
}

