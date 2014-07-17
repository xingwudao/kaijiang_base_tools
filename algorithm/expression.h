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
	// 说明：输入的中缀表达式元素类型定义
	typedef enum
	{
		OPERAND,		 //操作数
		OPERATOR,		 //操作符
		UNKNOWN_ITEM_TYPE //未知
	}ExpressionItemType;

	// 说明：操作符类型定义
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
		UNKNOWN_OPERATOR_TYPE	//未知
	}OperatorType;

	typedef struct
	{
		uint32_t priority; // 优先级
		uint32_t operands; // 所需操作数，不大于2
		uint32_t type;	   // 类型，1为四则运算，2为逻辑运算, 4为比较运算, 8为其他运算
	}OperatorParameter;

	// 说明：表达式元素
	typedef struct
	{
		ExpressionItemType enum_item_type; //元素类型：操作数还是操作符
		void* item;//元素，如果是操作符，则类型是OperatorType枚举型，如果是操作数，则是无符号整型表示变量名称
	}ExpressionItem;

	// 后缀表达式类
	class Expression
	{
		public:
			// 默认构造
			Expression();
			// 拷贝构造
			Expression(const Expression& other);
			// 赋值操作符
			Expression& operator=(const Expression& other);
			// 析构函数
			virtual ~Expression();

			// 说明：加入一个表达式元素
			// 参数：
			//  [IN] ExpItem -- 表达式元素
			// 返回：
			//  无
			void Push(ExpressionItem& ExpItem);

			// 说明：输入变量值，按照表达式计算结果
			// 参数：
			//  [IN] mapOperandValue -- 操作数值
			//  [OUT] Result -- 计算结果
			// 返回：
			//  完成计算 -- true
			//  未完成计算 -- false
			bool Operate(const map<string, int32_t>& operand_value, int32_t& result);
			bool Operate(const map<string, float>& operand_value, float& result);

			// 说明：流输出操作符重载(友元)
			// 参数：
			//  [IN] os -- 输出流
			//  [IN] Oper -- 后缀表达式
			// 返回：
			//  输出流
			//friend ostream& operator<<(ostream& os, const Expression& Oper);
			ostream& print(ostream& os);

			// 说明：操作数个数
			// 参数：
			//  无
			// 返回：
			//  操作数个数
			inline uint32_t GetOperandSize(){return  m_unOperands; }

			// 说明：读取所有操作数
			// 参数：
			//  [OUT] mapOperands -- key: operand, value: times
			// 返回：
			//  操作数个数
			uint32_t GetOperands(map<string, uint32_t>& mapOperands);

			// 说明：根据变量名获取变量编号
			// 参数：
			//  [IN] operand_variable -- 变量名
			// 返回：
			//  变量的编号
			uint32_t GetOperandCode(const char* operand_variable);

			// 说明：增加一个常数
			// 参数：
			//  [IN] value -- 值
			// 返回：
			//  常数操作数的代码
			uint32_t AddConstant(const float value);

			// 说明：判断表达式是否有效
			// 参数：
			//  无
			// 返回：
			//  true -- 有效
			bool IsActive();

			// 说明：获取表达式的操作符
			// 参数：
			//  [OUT] operators -- 输出到该map中
			// 返回：
			//  无
			//void GetOperators(map<OperatorType, pair<OperatorParameter, uint32_t> >& operators);
		private:
			vector<ExpressionItem> m_vecPostfixExp;//后缀表达式
			uint32_t m_unOperands;			 //操作数个数
			CTrieTree<uint32_t>* operands;   //操作数及其出现次数
			map<string, float> constant_float;
			map<string, int32_t> constant_int;
	};
	//ostream& operator<<(ostream& os, const Expression& Oper);

	// 表达式解析类
	class ExpressionParser
	{
		public:
			// 说明：构造函数和西沟函数
			ExpressionParser();
			~ExpressionParser();

			// 说明：解析一个中缀表达式，表达式
			// 参数：
			//  [IN] strNifixExp -- 中缀表达式
			// 返回：
			//  后缀表达式指针，如果解析失败则返回NULL
			Expression* Parse(const string& strNifixExp);
	};
	//ostream& operator<<(ostream& os, const Expression& Oper);
};
#endif
