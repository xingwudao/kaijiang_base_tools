
//////////////////////////////////////////////////
//字符串工具
//
//作者：开江
//
//日期：2012-11-12
//		增加：按照字符串拆分字符串的方法
//		增加：去掉字符串中首尾部位的空白字符
//		增加：拆分字符串时可以按照一个函数处理每个拆分得到的串
//版本：0.3
//
//
#ifndef KAIJIANG_STRINGTOOL_H
#define KAIJIANG_STRINGTOOL_H

#include <vector>
#include <string>
#include <stdint.h>
#include <string.h>
using namespace std;

namespace kaijiang
{
	// 说明：补齐字符串的枚举类型：
	//  STR_PAD_RIGHT - 补齐右边，
	//  STR_PAD_LEFT  - 补齐左边，
	//  STR_PAD_BOTH  - 两边补齐.
	enum STR_PAD {STR_PAD_RIGHT, STR_PAD_LEFT, STR_PAD_BOTH};

	// 说明：定义函数指针
	// 参数：
	//  [IN]cSplitElement -- 一个拆分得到的字符串元素
	//			[IN]index -- 当前处理第几个元素
	//       [OUT]pOutput -- 输出指针，根据具体的需要传入不同类型的指针，
	//                       但是同一都强转为void*,在自定义的函数内部根据
	//                       具体需要再转回原来类型
	// 返回：
	//  当前处理成功，则返回true，否则返回false，拆分函数根据这个统计得到的元素个数
	typedef bool (*HandleSpitElement)(const char* cSplitElement, const unsigned int index, void* pOutput);

	class CStringTool
	{
		public:
			// 说明：用指定的字符拆分字符串
			// 参数：
			//  [IN]strInput -- 输入字符串
			//  [IN]chSplit  -- 拆分字符
			// 返回：
			//  拆分得到的字符串向量
			static vector<string> SplitByChar(const string& strInput, const char chSplit);

			// 说明：用指定字符串拆分字符串
			// 参数：
			//  [IN] strInput -- 输入字符串
			//  [IN] strSplit -- 分割字符串
			// 返回：
			//  拆分得到的数组
			static vector<string> SplitByStr(const string& strInput, const string strSplit);

			// 说明：用指定字符串拆分字符串，并对每个拆分得到的子串元素进行自定义的操作
			// 参数：
			//  [IN] strInput -- 输入字符串
			//  [IN] strSplit -- 分割字符串
			//  [IN/OUT] pOutput -- 输出的数据
			//  [IN] pHandleFunc -- 对拆分得到的每个数据的处理函数
			// 返回：
			//  成功处理的元素个数
			static unsigned int SplitByStr(const string& strInput, const string& strSplit, void* pOutput, HandleSpitElement pHandleFunc);
			static unsigned int SplitByStr(const char* cInput, const uint32_t unInputSize, const char* cSplit, void* pOutput, HandleSpitElement pHandleFunc);

			// 说明：去掉首尾的空格和table
			// 参数：
			//  [IN] strInput -- 输入字符串
			// 返回：
			// 无
			static void Trim(string& strInput);

			// 说明：去掉首部的空白字符
			// 参数：
			//  [IN] strInput -- 去掉左边的空白字符
			// 返回：
			// 无
			static void LeftTrim(string& strInput);

			// 说明：去掉尾部的空白字符
			// 参数：
			//  [IN] strInput -- 去掉右边的空白字符
			// 返回：
			// 无
			static void RightTrim(string& strInput);

			// 把字符串填充为指定的长度
			// 参数：
			//		str -- 要填充的字符串
			//		pad_length -- 新字符串的长度。如果该值小于原始字符串的长度，则不进行任何操作
			//		pad_string -- 填充使用的字符串。默认是空白
			//		pad_type -- 规定填充字符串的哪边
			// 返回：
			//  补全后的字符串
			static string Pad(const string &str, int pad_length, string pad_string = " ", STR_PAD pad_type=STR_PAD_RIGHT);
	};
};
#endif
