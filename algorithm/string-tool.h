
//////////////////////////////////////////////////
//�ַ�������
//
//���ߣ�����
//
//���ڣ�2012-11-12
//		���ӣ������ַ�������ַ����ķ���
//		���ӣ�ȥ���ַ�������β��λ�Ŀհ��ַ�
//		���ӣ�����ַ���ʱ���԰���һ����������ÿ����ֵõ��Ĵ�
//�汾��0.3
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
	// ˵���������ַ�����ö�����ͣ�
	//  STR_PAD_RIGHT - �����ұߣ�
	//  STR_PAD_LEFT  - ������ߣ�
	//  STR_PAD_BOTH  - ���߲���.
	enum STR_PAD {STR_PAD_RIGHT, STR_PAD_LEFT, STR_PAD_BOTH};

	// ˵�������庯��ָ��
	// ������
	//  [IN]cSplitElement -- һ����ֵõ����ַ���Ԫ��
	//			[IN]index -- ��ǰ����ڼ���Ԫ��
	//       [OUT]pOutput -- ���ָ�룬���ݾ������Ҫ���벻ͬ���͵�ָ�룬
	//                       ����ͬһ��ǿתΪvoid*,���Զ���ĺ����ڲ�����
	//                       ������Ҫ��ת��ԭ������
	// ���أ�
	//  ��ǰ����ɹ����򷵻�true�����򷵻�false����ֺ����������ͳ�Ƶõ���Ԫ�ظ���
	typedef bool (*HandleSpitElement)(const char* cSplitElement, const unsigned int index, void* pOutput);

	class CStringTool
	{
		public:
			// ˵������ָ�����ַ�����ַ���
			// ������
			//  [IN]strInput -- �����ַ���
			//  [IN]chSplit  -- ����ַ�
			// ���أ�
			//  ��ֵõ����ַ�������
			static vector<string> SplitByChar(const string& strInput, const char chSplit);

			// ˵������ָ���ַ�������ַ���
			// ������
			//  [IN] strInput -- �����ַ���
			//  [IN] strSplit -- �ָ��ַ���
			// ���أ�
			//  ��ֵõ�������
			static vector<string> SplitByStr(const string& strInput, const string strSplit);

			// ˵������ָ���ַ�������ַ���������ÿ����ֵõ����Ӵ�Ԫ�ؽ����Զ���Ĳ���
			// ������
			//  [IN] strInput -- �����ַ���
			//  [IN] strSplit -- �ָ��ַ���
			//  [IN/OUT] pOutput -- ���������
			//  [IN] pHandleFunc -- �Բ�ֵõ���ÿ�����ݵĴ�����
			// ���أ�
			//  �ɹ������Ԫ�ظ���
			static unsigned int SplitByStr(const string& strInput, const string& strSplit, void* pOutput, HandleSpitElement pHandleFunc);
			static unsigned int SplitByStr(const char* cInput, const uint32_t unInputSize, const char* cSplit, void* pOutput, HandleSpitElement pHandleFunc);

			// ˵����ȥ����β�Ŀո��table
			// ������
			//  [IN] strInput -- �����ַ���
			// ���أ�
			// ��
			static void Trim(string& strInput);

			// ˵����ȥ���ײ��Ŀհ��ַ�
			// ������
			//  [IN] strInput -- ȥ����ߵĿհ��ַ�
			// ���أ�
			// ��
			static void LeftTrim(string& strInput);

			// ˵����ȥ��β���Ŀհ��ַ�
			// ������
			//  [IN] strInput -- ȥ���ұߵĿհ��ַ�
			// ���أ�
			// ��
			static void RightTrim(string& strInput);

			// ���ַ������Ϊָ���ĳ���
			// ������
			//		str -- Ҫ�����ַ���
			//		pad_length -- ���ַ����ĳ��ȡ������ֵС��ԭʼ�ַ����ĳ��ȣ��򲻽����κβ���
			//		pad_string -- ���ʹ�õ��ַ�����Ĭ���ǿհ�
			//		pad_type -- �涨����ַ������ı�
			// ���أ�
			//  ��ȫ����ַ���
			static string Pad(const string &str, int pad_length, string pad_string = " ", STR_PAD pad_type=STR_PAD_RIGHT);
	};
};
#endif
