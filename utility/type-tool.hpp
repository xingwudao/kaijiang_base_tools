#ifndef CTYPETOOL_H
#define CTYPETOOL_H

///////////////////////////////////////////////////////////
// 数据类型的工具类，主要用于各种数据类型与字符串之间的转换
//
// 作者：开江
//
// 日期：2011-06-22
//
// 版本：
//		增加：数值型转换为字符串时可以设置最终的字符串位数，当数字不足时则在字符串前面用0补齐
//
//

#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <string>
#include <bitset> 
using namespace std;

namespace kaijiang
{
		template<typename Type>
		class CTypeTool
		{
			public:
				// 字符串转换为其他类型
				//
				static Type StrTo(string str)
				{
					stringstream stream;
					stream.str(str);

					Type t ;
					stream>>t;

					return t;
				}

				// 其他类型转换为字符串
				//
				// 参数：t -- 其他类型
				//		 system -- 进制：2 -- 2进制 8 -- 八进制 10 -- 十进制 16 -- 十六进制
				//		 length -- 转换的字符串的长度，默认为-1，表示保持原来的长度，
				//		 如果转换后的字符串长度小于length，则在其前面补0。
				//		 例如要转换的是数字3，如果length设置为4，则转换结果为0003
				static string ToStr(Type t, int length = -1)
				{
					stringstream stream;
					stream<<t;
					
					if((int)stream.str().size() >= length)
					  return stream.str();
					else
					{
						int num = length - stream.str().size();
						string result = stream.str();
						while(num > 0)
						{
							result = "0" + result;
							num--;
						}
						return result;
					}
				}
				// 整型的转换
				// 参数：	
				//		t -- 整型及其兼容类型，包括：int, unsigned int, unsigned short,short
				//		system -- 进制，暂不支持二进制，默认十进制
				static string IntToStr(Type t, int system = 10)
				{
					stringstream stream;
					switch(system)
					{
						case 8:
							stream<<oct<<t;
							break;
						case 16:
							stream<<hex<<t;
							break;
						default:
							stream<<t;
					}
					return stream.str();
				}
		};

	/**********************使用方法*****************************/
	/*
	 * unsigned long long mid;
	 * string str_mid = CTypeTool<unsigned long long>::ToStr(mid);
	 * mid = CTypeTool<unsigned long long>::StrTo(str_mid);
	 */
};
#endif
