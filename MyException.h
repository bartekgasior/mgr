#pragma once

#include <iostream>
#include <exception>
using namespace std;

class MyException :	public exception
{
public:
	string s;
	MyException(string str) : s(str) {};
	//MyException();
	~MyException();

	virtual const char* what() const throw()
	{
		return s.c_str();
	}

};

