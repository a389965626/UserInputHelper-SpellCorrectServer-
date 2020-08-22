 ///
 /// @file    DictProducer_test.cc
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-11 16:01:14
 ///

#include "../../include/off_line_part/DictIndexProducer.h"
#include "../../include/off_line_part/SplitToolCppJieba.h"
using namespace cjy;

int main()
{
	SplitToolCppJieba splitTool;
	DictProducer dictproducer(splitTool); 
	
	dictproducer.build_dict(); //创建词典
	dictproducer.build_index();  //创建索引。
	
	return 0;
}


