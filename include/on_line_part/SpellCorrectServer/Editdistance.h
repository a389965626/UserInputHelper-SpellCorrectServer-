 ///
 /// @file    Editdistance.h
 /// @author  a389965626(389965626@qq.com)
 /// @date    2018-07-18 10:19:15
 ///
 
#ifndef __MY_EDIT_DISTANCE_H_
#define __MY_EDIT_DISTANCE_H_
#include <iostream>
#include <string>

size_t nBytesCode(const char ch);

std::size_t length(const std::string &str);

int tripleMin(const int &a, const int &b, const int &c);

int editDistance(const std::string & lhs, const std::string &rhs);

#endif
