#ifndef CTYPETOOL_H
#define CTYPETOOL_H

///////////////////////////////////////////////////////////
// �������͵Ĺ����࣬��Ҫ���ڸ��������������ַ���֮���ת��
//
// ���ߣ�����
//
// ���ڣ�2011-06-22
//
// �汾��
//		���ӣ���ֵ��ת��Ϊ�ַ���ʱ�����������յ��ַ���λ���������ֲ���ʱ�����ַ���ǰ����0����
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
				// �ַ���ת��Ϊ��������
				//
				static Type StrTo(string str)
				{
					stringstream stream;
					stream.str(str);

					Type t ;
					stream>>t;

					return t;
				}

				// ��������ת��Ϊ�ַ���
				//
				// ������t -- ��������
				//		 system -- ���ƣ�2 -- 2���� 8 -- �˽��� 10 -- ʮ���� 16 -- ʮ������
				//		 length -- ת�����ַ����ĳ��ȣ�Ĭ��Ϊ-1����ʾ����ԭ���ĳ��ȣ�
				//		 ���ת������ַ�������С��length��������ǰ�油0��
				//		 ����Ҫת����������3�����length����Ϊ4����ת�����Ϊ0003
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
				// ���͵�ת��
				// ������	
				//		t -- ���ͼ���������ͣ�������int, unsigned int, unsigned short,short
				//		system -- ���ƣ��ݲ�֧�ֶ����ƣ�Ĭ��ʮ����
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

	/**********************ʹ�÷���*****************************/
	/*
	 * unsigned long long mid;
	 * string str_mid = CTypeTool<unsigned long long>::ToStr(mid);
	 * mid = CTypeTool<unsigned long long>::StrTo(str_mid);
	 */
};
#endif
