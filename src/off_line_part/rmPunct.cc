///
/// @file    testCn.cc
/// @author  fxyatoi(fxy538351@163.com)
/// @date    2018-07-18 19:57:53
///
#include "../../include/off_line_part/rmPunct.h"

wstring StringToWstring(const string str)
{
	unsigned len = str.size()*2;
	setlocale(LC_CTYPE,"");
	wchar_t *p = new wchar_t[len];
	mbstowcs(p,str.c_str(),len);
	wstring str1(p);
	delete[] p;
	return str1;
}
string WstringToString(wstring str)
{
	unsigned len = str.size()*4;
	setlocale(LC_CTYPE,"");
	char *p = new char[len];
	wcstombs(p,str.c_str(),len);
	string str1(p);
	delete[] p;
	return str1;
}

string rmPunt()   //删除所有中文文档的标点，并将所有的中文文档放到一个string中。
{
	string chinese_line; //这个string专门用于存放所有文章读到的所有的行并返回。

	ifstream ifs;
	string chinese_filepath;

	string s1("../../data/chinese/C3-Art00");
	string s2;

	for(int i=1 ; i<=45 ;i+=2)  //对所有的文件进行操作。
	{
		if(i<10)
		{
			s2="0"+to_string(i);
		}
		else
		{
			s2=to_string(i);
		}
		chinese_filepath=s1+s2+".txt";  //得到真正的文将路径。
		ifs.open(chinese_filepath);  //得到文件输入流，注意在本循环结束后得将这个输入流关闭，否则后面的文件无法打开。
		if(!ifs)
		{
			cout << "error ifs" << endl;
			return string();
		}
		
		//执行读取的文件
		string line;
		while(getline(ifs,line),!ifs.eof())
		{

			wstring ws = StringToWstring(line);
			wstring nws;
			//过滤每一行中的标点和空格
			for(wchar_t ch:ws)
			{
				//检查是否是标点和空格
				if(!iswpunct(ch)&&!iswdigit(ch))
				{
					nws.push_back(ch);  //不是的话就把它插入到新的wstring中。
				}
			}
			//将过滤后的文本重新转换成UTF-8编码的多字节类型
			string ns = WstringToString(nws);  //得到重新写的新的一行。
			
			chinese_line.append(ns); //继续添加到一行的末尾（所有的中文文档都是添加到一个string中，最后返回这个string即可）
		}
		ifs.close();
	}
	return chinese_line;
}

#if 0
int main()
{
	ifstream infile("cndict.txt");
	ofstream outfile("cnFile.dat");
	if(!infile)
	{
		cout << "Can not open file cnfilename.txt" << endl;
		return -1;
	}
	if(!outfile)
	{
		cout << "Can not file outfile.dat" << endl;
		return -1;
	}
	//执行读取的文件
	string line;
	while(getline(infile,line),!infile.eof())
	{
		//转换成宽字节类型
		wstring ws = StringToWstring(line);
		wstring nws;
		//过滤每一行中的标点和空格
		for(wchar_t ch:ws)
		{
			//检查是否是标点和空格
			if(!iswpunct(ch))
			{
				nws.push_back(ch);
			}
		}
		//将过滤后的文本重新转换成UTF-8编码的多字节类型
		string ns = WstringToString(nws);
		//重新写回文件
		outfile << ns << endl;
	}
	infile.close();
	outfile.close();
	return 0;
}
#endif 


