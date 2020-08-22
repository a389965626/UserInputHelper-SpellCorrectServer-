 ///
 /// @file    SplitToolCppJieba.cc
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-19 11:06:08
 ///


#include "../../include/off_line_part/SplitToolCppJieba.h"
#include "../../../cppjieba-master/include/cppjieba/Jieba.hpp"
using namespace cjy;

const char *const DICT_PATH = "../../../cppjieba-master/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../../../cppjieba-master/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../../../cppjieba-master/dict/user.dict.utf8";
const char* const IDF_PATH = "../../../cppjieba-master/dict/idf.utf8";
const char* const STOP_WORD_PATH = "../../../cppjieba-master/dict/stop_words.utf8";


vector<string> SplitToolCppJieba::cutsentence(string & sentence)
{
	cppjieba::Jieba jieba(DICT_PATH,
			HMM_PATH,
			USER_DICT_PATH,
			IDF_PATH,
			STOP_WORD_PATH);
	vector<string> words;   //存放分好词的容器，每个元素是一个中文词语（词语，不是一个中文字）
	jieba.Cut(sentence,words,true);
	
	//验证是否分词成功。
#if 0
	for(const auto &it : words)
	{
		cout << it << "\n";
	}
#endif
	return words;
}



