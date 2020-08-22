 ///
 /// @file    Task.h
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-12 21:46:15

//Task类，表明线程池中的子线程具体应该执行的一些操作
#ifndef __MYTASK_H__
#define __MYTASK_H__
#include "../TcpServer/TcpConnection.h"
#include "../TcpServer/TcpServer.h"
#include <iostream>
#include <queue>
#include <functional>

using namespace std;

namespace cjy
{ 
	//通过查询词典，得到一系列候选词（相类似的词），然后放到优先队列（按照最小编辑距离由低到高排序），
	//最后从优先级队列取出前几个的单次即可
	
	typedef struct MyResult //将处理结果放到这个结构体中，然后再放到优先级队列中去。
	{
		string _word;  //候选词
		int _iFreq;  //词频
		int _iDist;  //与查询词的最小编辑距离
	}MyResult;

	struct MyCompare
	{
		bool operator()(const MyResult &mr1,const MyResult &mr2)
    	{
	    	if(mr1._iDist != mr2._iDist)  //优先选择较小的最小编辑距离
		    	return mr1._iDist > mr2._iDist;
    		else if(mr1._iFreq != mr2._iFreq)  //在最小编辑距离相同的情况下选择词频较多的
	    		return mr1._iFreq > mr2._iFreq;
	    	else
		    	return mr1._word > mr2._word;    //在最小编辑距离和词频均相同的情况下，选择较大的单词
		}
	};

	class MyTask
	{
		public:
			MyTask(const string &query,TcpConnectionPtr conn)  //构造函数
			:_query(query)   //_query不包含换行符!!
			,_conn(conn)
			{}
			void execute();  //在子线程中执行的任务，用bind绑定到这个函数上在传入到线程池的任务队列中

		private:
			void queryIndexTable();//查询索引,需要有索引文件（考虑根据查询单词的每一个字母得到其索引，然后在每个字母所对应的
			//索引表中查询，按照单词，词频，和最小编辑距离的方式放入到优先队列即可。）
			int distance(const string & rhs);	//计算最小编辑距离，根据在索引表查询到的string单词来计算最小编辑距离。

			string encode(); //得到json包的编码函数
		private:
			string _query;  //接受具体的查询单词，交给子线程（线程池）来执行
			TcpConnectionPtr _conn;  //需要一个tcpconnection对象的指针，用于将具体要执行的操作发送给IO线程执行。
			priority_queue<MyResult, vector<MyResult>, MyCompare>  _resultQue; //优先级队列，用于存放查询到的单词，然后将
			//优先级队列的前几个单词传送到客户端即可。
	};
}

#endif

