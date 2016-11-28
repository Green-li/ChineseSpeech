#pragma once
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cwchar>
#include <Windows.h>

#include <mmsystem.h> //import the head of the radio file
#pragma comment(lib,"winmm.lib")//import the MIC radio lib

using namespace std;

bool isLetter(char ch)
{
	if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'))
		return true;
	else
		return false;
}

bool isLetter(wchar_t wch)
{
	char ch;

	if ((wch & 0xff00) >> 4 != 0)
		return false;
	else
	{
		ch = wch;
		return isLetter(ch);
	}
}

wstring str2wstr(const string& s)								//convert str to wstr
{
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t converted = 0;
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];

	wmemset(_Dest, 0, _Dsize);
	mbstowcs_s(&converted, _Dest, _Dsize - 1, _Source, _Dsize);

	wstring result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, "C");
	return result;
}

wstring bytes2wstr(const string& utf16line)							//convert bytes to wstr
{
	std::wstring result = L"";
	for (int i = 0; i<utf16line.length() - 1; i += 2)
	{
		unsigned char c1 = utf16line[i];
		unsigned char c2 = utf16line[i + 1];
		unsigned short wc;
		if (c2 == 0)
		{
			wc = c1;
		}
		else
		{
			wc = c2;
			wc = wc << 8;
			wc += c1;
		}
		result += wc;
	}
	return result;
}

string wstr2bytes(const wstring& utf16wline)					//convert wstring to bytes
{
	std::string result = "";
	for (int i = 0; i<utf16wline.length(); i++)
	{
		wchar_t wc = utf16wline[i];
		unsigned char c1 = wc & 0x00ff;
		unsigned char c2 = (wc >> 8);
		result += c1;
		result += c2;
	}
	return result;
}