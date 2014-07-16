#include "CStringTool.h"
#include <algorithm> 
#include <functional>   
#include <cctype>

using namespace std;
using namespace kaijiang;

// 将字符串按照指定的字符拆分成字符串数组
vector<string> kaijiang::CStringTool::SplitByChar(const string& input, const char ch)
{
	string str;
	str.push_back(ch);
	return SplitByStr(input, str);
}

// 按照字符串对字符串进行拆分
vector<string> kaijiang::CStringTool::SplitByStr(const string& input, const string str)
{
	vector<string> result;
	if(input.size() == 0)
	{
		return result;
	}
	size_t lastpos = 0;
	size_t pos = input.find(str, lastpos);
	while(pos != input.npos)
	{
		result.push_back(input.substr(lastpos, pos - lastpos));
		lastpos = pos + str.size();
		pos = input.find(str, lastpos);
	}
	if(lastpos < input.size())
	{
		result.push_back(input.substr(lastpos, input.size() - lastpos));
	}

	return result;
}

unsigned int CStringTool::SplitByStr(const string& strInput, const string& strSplit, void* pOutput, HandleSpitElement pHandleFunc)
{
	return SplitByStr(strInput.c_str(), strInput.size(), strSplit.c_str(), pOutput, pHandleFunc);
}

// 说明：用指定字符串拆分字符串，并对每个拆分得到的子串元素进行自定义的操作
unsigned int CStringTool::SplitByStr(const char* src, const uint32_t unInputSize, const char* split_char, void* pOutput, HandleSpitElement pHandleFunc)
{
	if(src == NULL || unInputSize == 0 || split_char == NULL)
	{
		return 0;
	}
	uint32_t split_length = strlen(split_char);
	char* split = strstr((char*)src, split_char);
	char* buffer = new char[unInputSize + 1];
	unsigned int elements = 0;
	unsigned int index = 0;
	while(split != NULL)
	{
		strncpy(buffer, src, split - src);
		buffer[split - src] = 0;
		if((*pHandleFunc)(buffer, index, pOutput))
		  elements++;
		index++;
		src = split + split_length;
		split = strstr((char*)src, split_char);
	}
	if(strlen(src) > 0 && (*pHandleFunc)(src, index, pOutput))
	  elements++;
	delete [] buffer;

	return elements;
}

//去掉首尾的空白字符
void kaijiang::CStringTool::Trim(string& str)
{
	LeftTrim(str);
	RightTrim(str);
}

// 去掉左边的空白字符
void kaijiang::CStringTool::LeftTrim(string& ss)
{
	string::iterator p=find_if(ss.begin(),ss.end(),not1(ptr_fun(::isspace))); 
	ss.erase(ss.begin(),p);
}

// 去掉右边的空白字符
void kaijiang::CStringTool::RightTrim(string& ss)
{
	string::reverse_iterator p=find_if(ss.rbegin(),ss.rend(),not1(ptr_fun(::isspace))); 
	ss.erase(p.base(),ss.end()); 
}

// 字符串补全
string kaijiang::CStringTool::Pad(const string &str, int pad_length, string pad_string, STR_PAD pad_type)
{
	int i,j,x;
	int str_size = str.size();
	int pad_size = pad_string.size();
	if (pad_length<=str_size || pad_size<1)
	  return str;

	string o;
	o.reserve(pad_length);//allocate enough memory only once

	if (pad_type==STR_PAD_RIGHT)
	{
		for(i=0,x=str_size; i<x; i++)
		  o.push_back( str[i] );
		for(i=str_size; i<pad_length; )
		  for(j=0; j<pad_size && i<pad_length; j++,i++)
			o.push_back( pad_string[j] );
	}
	else if (pad_type==STR_PAD_LEFT)
	{
		int a1= pad_length-str_size;
		for(i=0; i<a1; )
		  for(j=0; j<pad_size && i<a1; j++,i++)
			o.push_back( pad_string[j] );
		for(i=0,x=str_size; i<x; i++)
		  o.push_back( str[i] );
	}
	else if (pad_type==STR_PAD_BOTH)
	{
		int a1= (pad_length-str_size)/2;
		int a2= pad_length-str_size-a1;
		for(i=0; i<a1; )
		  for(j=0; j<pad_size && i<a1; j++,i++)
			o.push_back( pad_string[j] );
		for(i=0,x=str_size; i<x; i++)
		  o.push_back( str[i] );
		for(i=0; i<a2; )
		  for(j=0; j<pad_size && i<a2; j++,i++)
			o.push_back( pad_string[j] );
	}
	return o;
}
