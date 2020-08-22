 ///
 /// @file    SplitTool.h
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-19 10:51:54
 ///

#ifndef __SPLITTOOLCPPJIEBA_H__
#define __SPLITTOOLCPPJIEBA_H__
#include <vector>
#include <string>
using namespace std;

namespace cjy
{
	class SplitToolCppJieba
	{
		public:
			vector<string> cutsentence(string & sentence); //利用cppjieba提供的分词方法进行分词。
		private:
	};
}

#endif

