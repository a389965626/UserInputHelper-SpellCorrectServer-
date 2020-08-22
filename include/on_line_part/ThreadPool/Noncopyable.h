///
/// @file    Nocopyable.h
/// @author  a389965626(389965626@qq.com)
/// @date    2018-07-04 09:52:39
///

#ifndef __NOCOPYABLE_H__
#define __NOCOPYABLE_H__
class Noncopyable
{
	protected:
		Noncopyable(){}
		~Noncopyable(){}
	private:
		Noncopyable(const Noncopyable&);  //基类的复制构造函数放在私有区
		Noncopyable &operator=(const Noncopyable&);//基类的赋值运算符放在私有区  
		//因此凡是继承自Noccopyable类的派生类都不允许复制或赋值，凡是继承自Copyable的派生类都允许赋值或复制。
};
#endif

