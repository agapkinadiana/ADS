#include "stdafx.h"
#include <iostream>

extern "C"
{
	int __stdcall len(char* source)
	{
		int result = 0;
		while (source[result] != '\0')
		{
			result++;
		}
		return result;
	}

	int __stdcall pows(int val1, int val2)
	{
		int result = 1;
		for (int i = 0; i < val2; i++)
		{
			result *= val1;
		}
		return result;
	}

	int __stdcall prints(char* value)
	{
		setlocale(0, "");
		std::cout << value << std::endl;
		return 0;
	}

	int __stdcall printi(int value)
	{
		std::cout << value << std::endl;
		return 0;
	}

	int __stdcall incr(int sq) {
		int r;
		r = ++sq;
		return r;
	}

	int __stdcall decr(int s) {
		int ris;
		ris = --s;
		return ris;
	}
}