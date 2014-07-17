#ifndef CEXPRESSIONPARSER_H
#define CEXPRESSIONPARSER_H

#include <vector>
#include <string>
#include <stack>
#include <map>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include "./data-structure/trie-tree.hpp"
using namespace std;

namespace kaijiang
{
	// ˵�����������׺���ʽԪ�����Ͷ���
	typedef enum
	{
		OPERAND,		 //������
		OPERATOR,		 //������
		UNKNOWN_ITEM_TYPE //δ֪
	}ExpressionItemType;

	// ˵�������������Ͷ���
	typedef enum
	{
		LEFT_BRACKET,	//	(
		RIGHT_BRACKET,	//	)
		NEGATIVE,		//	-
		MUTIPLY,		//	*
		DEVIDE,			//	/
		MODE,			//	%
		PLUS,			//	+
		MINUS,			//	-
		LESS_THAN,		//  <
		MORE_THAN,		//	>
		XOR,			//	^
		LESS_EQUAL,		//	<=
		MORE_EQUAL,		//	>=
		EQUAL_TO,		//	==
		NOT_EQUAL,		//	!=
		AND,			//	&&
		OR,				//	||
		UNKNOWN_OPERATOR_TYPE	//δ֪
	}OperatorType;

	typedef struct
	{
		uint32_t priority; // ���ȼ�
		uint32_t operands; // �����������������2
		uint32_t type;	   // ���ͣ�1Ϊ�������㣬2Ϊ�߼�����, 4Ϊ�Ƚ�����, 8Ϊ��������
	}OperatorParameter;

	// ˵�������ʽԪ��
	typedef struct
	{
		ExpressionItemType enum_item_type; //Ԫ�����ͣ����������ǲ�����
		void* item;//Ԫ�أ�����ǲ���������������OperatorTypeö���ͣ�����ǲ������������޷������ͱ�ʾ��������
	}ExpressionItem;

	// ��׺���ʽ��
	class Expression
	{
		public:
			// Ĭ�Ϲ���
			Expression();
			// ��������
			Expression(const Expression& other);
			// ��ֵ������
			Expression& operator=(const Expression& other);
			// ��������
			virtual ~Expression();

			// ˵��������һ�����ʽԪ��
			// ������
			//  [IN] ExpItem -- ���ʽԪ��
			// ���أ�
			//  ��
			void Push(ExpressionItem& ExpItem);

			// ˵�����������ֵ�����ձ��ʽ������
			// ������
			//  [IN] mapOperandValue -- ������ֵ
			//  [OUT] Result -- ������
			// ���أ�
			//  ��ɼ��� -- true
			//  δ��ɼ��� -- false
			bool Operate(const map<string, int32_t>& operand_value, int32_t& result);
			bool Operate(const map<string, float>& operand_value, float& result);

			// ˵�������������������(��Ԫ)
			// ������
			//  [IN] os -- �����
			//  [IN] Oper -- ��׺���ʽ
			// ���أ�
			//  �����
			//friend ostream& operator<<(ostream& os, const Expression& Oper);
			ostream& print(ostream& os);

			// ˵��������������
			// ������
			//  ��
			// ���أ�
			//  ����������
			inline uint32_t GetOperandSize(){return  m_unOperands; }

			// ˵������ȡ���в�����
			// ������
			//  [OUT] mapOperands -- key: operand, value: times
			// ���أ�
			//  ����������
			uint32_t GetOperands(map<string, uint32_t>& mapOperands);

			// ˵�������ݱ�������ȡ�������
			// ������
			//  [IN] operand_variable -- ������
			// ���أ�
			//  �����ı��
			uint32_t GetOperandCode(const char* operand_variable);

			// ˵��������һ������
			// ������
			//  [IN] value -- ֵ
			// ���أ�
			//  �����������Ĵ���
			uint32_t AddConstant(const float value);

			// ˵�����жϱ��ʽ�Ƿ���Ч
			// ������
			//  ��
			// ���أ�
			//  true -- ��Ч
			bool IsActive();

			// ˵������ȡ���ʽ�Ĳ�����
			// ������
			//  [OUT] operators -- �������map��
			// ���أ�
			//  ��
			//void GetOperators(map<OperatorType, pair<OperatorParameter, uint32_t> >& operators);
		private:
			vector<ExpressionItem> m_vecPostfixExp;//��׺���ʽ
			uint32_t m_unOperands;			 //����������
			CTrieTree<uint32_t>* operands;   //������������ִ���
			map<string, float> constant_float;
			map<string, int32_t> constant_int;
	};
	//ostream& operator<<(ostream& os, const Expression& Oper);

	// ���ʽ������
	class ExpressionParser
	{
		public:
			// ˵�������캯������������
			ExpressionParser();
			~ExpressionParser();

			// ˵��������һ����׺���ʽ�����ʽ
			// ������
			//  [IN] strNifixExp -- ��׺���ʽ
			// ���أ�
			//  ��׺���ʽָ�룬�������ʧ���򷵻�NULL
			Expression* Parse(const string& strNifixExp);
	};
	//ostream& operator<<(ostream& os, const Expression& Oper);
};
#endif
