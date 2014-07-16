#include "CExpressionParser.h"
#include "CTypeTool.h"
#include <set>

using namespace kaijiang;

/////////////////////////////////////////////////
// 全局函数和变量

// 说明：操作符的优先级和所需操作数定义
//		 操作符优先级定义,越小优先级越高
//		 value : first是优先级，second是所需操作符
//static map<OperatorType, pair<uint32_t, uint32_t> > mapOperatorPriority;
static map<OperatorType, OperatorParameter> mapOperatorPriority;

// 说明：操作符的字符串形式
static map<string, OperatorType> mapOperatorName;

/////////////////////////////////////////////////
// 说明：初始化操作符优先级
void InitializePriority()
{
	if(mapOperatorPriority.size() > 0)
	  return;
	OperatorParameter operator_parameter;
	operator_parameter.priority = 5;
	operator_parameter.operands = 0;
	operator_parameter.type = 8;
	mapOperatorPriority[LEFT_BRACKET] = operator_parameter;
	//mapOperatorPriority[LEFT_BRACKET] = pair<uint32_t, uint32_t>(5, 0);
	mapOperatorName["("] = LEFT_BRACKET;
	mapOperatorPriority[RIGHT_BRACKET] = operator_parameter;
	mapOperatorName[")"] = RIGHT_BRACKET;
	// 负号不保存名字
	operator_parameter.priority = 6;
	operator_parameter.operands = 1;
	operator_parameter.type = 1;
	mapOperatorPriority[NEGATIVE] = operator_parameter;
	//mapOperatorPriority[NEGATIVE] = pair<uint32_t, uint32_t>(6,1);

	// 优先级相同的在运算时按照出现的先后顺序计算
	mapOperatorName["*"] = MUTIPLY;
	operator_parameter.priority = 7;
	operator_parameter.operands = 2;
	operator_parameter.type = 1;
	mapOperatorPriority[MUTIPLY] = operator_parameter;
	//mapOperatorPriority[MUTIPLY] = pair<uint32_t, uint32_t>(7, 2);
	mapOperatorName["/"] = DEVIDE;
	mapOperatorPriority[DEVIDE] = operator_parameter;
	//mapOperatorPriority[DEVIDE] = pair<uint32_t, uint32_t>(7, 2);
	mapOperatorName["%"] = MODE;
	mapOperatorPriority[MODE] = operator_parameter;
	//mapOperatorPriority[MODE] = pair<uint32_t, uint32_t>(7, 2);

	mapOperatorName["+"] = PLUS;
	operator_parameter.priority = 8;
	operator_parameter.operands = 2;
	operator_parameter.type = 1;
	mapOperatorPriority[PLUS] = operator_parameter;
	//mapOperatorPriority[PLUS] = pair<uint32_t, uint32_t>(8, 2);
	mapOperatorName["-"] = MINUS;
	mapOperatorPriority[MINUS] = operator_parameter;
	//mapOperatorPriority[MINUS] = pair<uint32_t, uint32_t>(8, 2);

	mapOperatorName["<"] = LESS_THAN;
	operator_parameter.priority = 9;
	operator_parameter.operands = 2;
	operator_parameter.type = 4;
	mapOperatorPriority[LESS_THAN] = operator_parameter;
	//mapOperatorPriority[LESS_THAN] = pair<uint32_t, uint32_t>(9, 2);
	mapOperatorName[">"] = MORE_THAN;
	mapOperatorPriority[MORE_THAN] = operator_parameter;
	//mapOperatorPriority[MORE_THAN] = pair<uint32_t, uint32_t>(9, 2);
	mapOperatorName["<="] = LESS_EQUAL;
	mapOperatorPriority[LESS_EQUAL] = operator_parameter;
	//mapOperatorPriority[LESS_EQUAL] = pair<uint32_t, uint32_t>(9, 2);
	mapOperatorName[">="] = MORE_EQUAL;
	mapOperatorPriority[MORE_EQUAL] = operator_parameter;
	//mapOperatorPriority[MORE_EQUAL] = pair<uint32_t, uint32_t>(9, 2);

	mapOperatorName["=="] = EQUAL_TO;
	operator_parameter.priority = 10;
	operator_parameter.operands = 2;
	operator_parameter.type = 4;
	mapOperatorPriority[EQUAL_TO] = operator_parameter;
	//mapOperatorPriority[EQUAL_TO] = pair<uint32_t, uint32_t>(10, 2);
	mapOperatorName["!="] = NOT_EQUAL;
	mapOperatorPriority[NOT_EQUAL] = operator_parameter;
	//mapOperatorPriority[NOT_EQUAL] = pair<uint32_t, uint32_t>(10, 2);

	mapOperatorName["^"] = XOR;
	operator_parameter.priority = 11;
	operator_parameter.operands = 2;
	operator_parameter.type = 2;
	mapOperatorPriority[XOR] = operator_parameter;
	//mapOperatorPriority[XOR] = pair<uint32_t, uint32_t>(11, 2);
	mapOperatorName["&&"] = AND;
	operator_parameter.priority = 12;
	mapOperatorPriority[AND] = operator_parameter;
	//mapOperatorPriority[AND] = pair<uint32_t, uint32_t>(12, 2);
	mapOperatorName["||"] = OR;
	operator_parameter.priority = 13;
	mapOperatorPriority[OR] = operator_parameter;
	//mapOperatorPriority[OR] = pair<uint32_t, uint32_t>(13, 2);
}

// 说明：操作符相关的符号集合定义，凡是表达式出现了既非操作数也非这个集合中的符号，就认为是非法表达式
static set<char> setOperatorChar;
static set<char> valid_char_for_variables;

// 说明：初始化操作符相关的符号集合
void InitializeOperChar()
{
	if(setOperatorChar.size() > 0)
	  return;
	setOperatorChar.insert('(');
	setOperatorChar.insert(')');
	setOperatorChar.insert('-');
	setOperatorChar.insert('+');
	setOperatorChar.insert('*');
	setOperatorChar.insert('%');
	setOperatorChar.insert('<');
	setOperatorChar.insert('>');
	setOperatorChar.insert('=');
	setOperatorChar.insert('!');
	setOperatorChar.insert('&');
	setOperatorChar.insert('^');
	setOperatorChar.insert('/');
	setOperatorChar.insert('|');
}

void InitializeVariableChars()
{
	if(valid_char_for_variables.size() > 0)
		return;
	valid_char_for_variables.insert('-');
	valid_char_for_variables.insert(':');
}

// 说明：根据操作符字符串判断其类型
// 参数：
//  [IN] strOper -- 字符串形式的操作符
// 返回：
//  枚举型操作符类型
OperatorType GetOperator(string& strOper)
{
#ifdef DEBUG
	cout<<"search "<<strOper<<endl;
#endif
	map<string, OperatorType>::iterator iter = mapOperatorName.find(strOper);
	if(iter == mapOperatorName.end())
	  return UNKNOWN_OPERATOR_TYPE;

	return iter->second;
}

// 说明：根据操作符和操作数运算结果
// 重载1：接受整型操作数
// 重载2：接受浮点型操作数
// 参数：
//  [IN] OperateType -- 操作符类型
//  [IN] vecOperands -- 操作数，右操作数在前面，左操作数在后面
int32_t DoOperating(OperatorType& OperateType, vector<int32_t>& vecOperands)
{
	switch(OperateType)
	{
		case NEGATIVE:
			return -1 * vecOperands.front();
		case MUTIPLY:
			return vecOperands[1] * vecOperands[0];
		case DEVIDE:
			return vecOperands[1] / vecOperands[0];
		case MODE:
			return vecOperands[1] % vecOperands[0];  
		case PLUS:
			return vecOperands[1] + vecOperands[0];
		case MINUS:
			return vecOperands[1] - vecOperands[0];   
		case LESS_THAN:
			return (int32_t)(vecOperands[1] < vecOperands[0]); 
		case MORE_THAN:
			return (int32_t)(vecOperands[1] > vecOperands[0]); 
		case LESS_EQUAL:
			return (int32_t)(vecOperands[1] <= vecOperands[0]); 
		case MORE_EQUAL:
			return (int32_t)(vecOperands[1] >= vecOperands[0]); 
		case EQUAL_TO:
			return (int32_t)(vecOperands[1] == vecOperands[0]); 
		case NOT_EQUAL:
			return (int32_t)(vecOperands[1] != vecOperands[0]); 
		case AND:
			return (int32_t)((bool)vecOperands[1] && (bool)vecOperands[0]); 
		case OR:
			return (int32_t)((bool)vecOperands[1] || (bool)vecOperands[0]); 
		case XOR:
			return (int32_t)(vecOperands[1] ^ vecOperands[0]); 
		default:
			return vecOperands.front();
	}
}
float DoOperating(OperatorType& OperateType, vector<float>& vecOperands)
{
	switch(OperateType)
	{
		case NEGATIVE:
			return -1 * vecOperands.front();
		case MUTIPLY:
			return vecOperands[1] * vecOperands[0];
		case DEVIDE:
			return vecOperands[1] / vecOperands[0];
		case MODE:
			return (float)((int32_t)vecOperands[1] % (int32_t)vecOperands[0]);  
		case PLUS:
			return vecOperands[1] + vecOperands[0];
		case MINUS:
			return vecOperands[1] - vecOperands[0];   
		case LESS_THAN:
			return (float)(vecOperands[1] < vecOperands[0]); 
		case MORE_THAN:
			return (float)(vecOperands[1] > vecOperands[0]); 
		case LESS_EQUAL:
			return (float)(vecOperands[1] <= vecOperands[0]); 
		case MORE_EQUAL:
			return (float)(vecOperands[1] >= vecOperands[0]); 
		case EQUAL_TO:
			return (float)(vecOperands[1] == vecOperands[0]); 
		case NOT_EQUAL:
			return (float)(vecOperands[1] != vecOperands[0]); 
		case AND:
			return (float)((bool)vecOperands[1] && (bool)vecOperands[0]); 
		case OR:
			return (float)((bool)vecOperands[1] || (bool)vecOperands[0]); 
		case XOR:
			return (float)((int32_t)vecOperands[1] ^ (int32_t)vecOperands[0]); 
		default:
			return vecOperands.front();
	}
}

// 说明：根据枚举型操作符返回字符串类型
string GetOperatorName(OperatorType OperateType)
{
	switch(OperateType)
	{
		case NEGATIVE:
			return "-";
		case MUTIPLY:
			return "*";
		case DEVIDE:
			return "/";
		case MODE:
			return "%";  
		case PLUS:
			return "+";
		case MINUS:
			return "-";   
		case LESS_THAN:
			return "<";
		case MORE_THAN:
			return ">"; 
		case LESS_EQUAL:
			return "<="; 
		case MORE_EQUAL:
			return ">="; 
		case EQUAL_TO:
			return "=="; 
		case NOT_EQUAL:
			return "!="; 
		case AND:
			return "&&"; 
		case OR:
			return "||"; 
		case XOR:
			return "^"; 
		default:
			return "";
	}
}

// 说明：CExpressionParser对象的流输出操作符重载
//ostream& operator<<(ostream& os, const CPostfixExpression& Oper)
ostream& CPostfixExpression::print(ostream& os)
{
	for(unsigned int index = 0; index < m_vecPostfixExp.size(); index++)
	{
		if(m_vecPostfixExp[index].enum_item_type == OPERAND)
		{
			os<<operands->GetKey(*(uint32_t*)m_vecPostfixExp[index].item)<<" ";
		}
		else 
		{
			OperatorType opera = *(OperatorType*)m_vecPostfixExp[index].item;     
			os<<GetOperatorName(opera)<<" ";
		}
	}

	return os;
}

////////////////////////////////////////////
//CExpressionParser 类方法实现：


// 说明：构造函数
CExpressionParser::CExpressionParser()
{
	InitializePriority();
	InitializeOperChar();
	InitializeVariableChars();
}

// 说明：西沟函数
CExpressionParser::~CExpressionParser()
{
}


// 说明：解析中缀表达式,转换成方便运算的后缀表达式
CPostfixExpression* CExpressionParser::ParseNifixExp(const string& strNifixExp)
{
	// 后缀表达式
	CPostfixExpression* pPostfixExpression = new CPostfixExpression;
	string strOperand;//操作数
	stack<OperatorType> stackOperator;
	ExpressionItemType LastItemType = UNKNOWN_ITEM_TYPE; 
	OperatorType LastOperator = UNKNOWN_OPERATOR_TYPE; 
	for(size_t i = 0; i < strNifixExp.size(); i++);
	size_t i = 0;
	while(i < strNifixExp.size())
	{
#ifdef DEBUG
		cout<<"ch = "<<strNifixExp.at(i)<<endl;
#endif
		// 空白字符
		if(isspace(strNifixExp.at(i)))
		{
			i++;
			continue;
		}
		// 以$开始的是变量，否则就是常数，总之都是操作数
		else if(strNifixExp.at(i) == '$' || isdigit(strNifixExp.at(i)))
		{
			strOperand = "";
			bool constant_value = true;
			if(strNifixExp.at(i) == '$')
			{
				// after '$', the '_' is banned.
				if(i + 1 < strNifixExp.size() && strNifixExp.at(i+1) == '#')
				{
					cerr<<"[EXPRESSION ERROR] : after '$', the '#' is banned."<<endl;
					delete pPostfixExpression;
					return NULL;
				}
				constant_value = false;
				i++;
			}
			while(i < strNifixExp.size() && (setOperatorChar.count(strNifixExp.at(i)) == 0) && (!isspace(strNifixExp.at(i))))
			{
				strOperand.push_back(strNifixExp.at(i++));
			}
			if(strOperand.size() == 0)
			{
				cerr<<"[EXPRESSION ERROR] : after '$', there is nothing valid."<<endl;
				delete pPostfixExpression;
				return NULL;
			}
			ExpressionItem ExpItem;
			ExpItem.enum_item_type = OPERAND; 
			uint32_t* oper = new uint32_t;
			if(constant_value)
				*oper = pPostfixExpression->AddConstant(CTypeTool<float>::StrTo(strOperand));
			else
				*oper = pPostfixExpression->GetOperandCode(strOperand.c_str());
			if(*oper == 0)
			{
				cerr<<"[EXPRESSION ERROR] : it look like the expression instance don't wanna accept this operand, which is "<<strOperand<<endl;
				delete pPostfixExpression;
				return NULL;  
			}
			ExpItem.item = (void*)oper;
			strOperand = "";
			pPostfixExpression->Push(ExpItem);
			if(LastItemType == OPERAND)
			{
				cerr<<"[EXPRESSION ERROR] : hey man, you can not write two operands successively."<<endl;
				delete pPostfixExpression;
				return NULL;
			}
			LastItemType = OPERAND;
			continue;
		}
		// 不以$开始的字母都是耍流氓
		else if(isalpha(strNifixExp.at(i)))
		{
			cerr<<"[EXPRESSION ERROR] : operands have to started by '$', location char = "<<strNifixExp.at(i)<<", the "<<i<<"th char."<<endl;
			delete pPostfixExpression;
			return NULL;
		}
		// 其他符号
		else
		{
			strOperand = "";
			OperatorType OperType = UNKNOWN_OPERATOR_TYPE;
			// 这里要处理两个操作符连在一起的情况：
			// 我们预先知道我们所定义的所有操作符最多只有两个字母：
			while(i < strNifixExp.size() && strOperand.size() < 2)
			{
#ifdef DEBUG
				cout<<strNifixExp.at(i)<<endl;
#endif
				if(isspace(strNifixExp.at(i)) || strNifixExp.at(i) == '$' || isdigit(strNifixExp.at(i)))
					break;
				if(setOperatorChar.count(strNifixExp.at(i)) == 0)
				{
					cerr<<"[EXPRESSION ERROR] : meet some unknown chars: "<<strNifixExp.at(i)<<endl;
					delete pPostfixExpression;
					return NULL;
				}
				strOperand.push_back(strNifixExp.at(i));
				OperatorType OperTypeNew = GetOperator(strOperand);
				if(OperTypeNew == UNKNOWN_OPERATOR_TYPE && strOperand.size() == 2)
				{
					break;
				}
				OperType = OperTypeNew == UNKNOWN_OPERATOR_TYPE ? OperType : OperTypeNew;
#ifdef DEBUG
				//cout<<strOperand<<endl;
#endif
				i++;
			}
			//OperatorType OperType = GetOperator(strOperand); 
#ifdef DEBUG
			cout<<"type:"<<OperType<<endl;
#endif
			if(OperType == UNKNOWN_OPERATOR_TYPE)
			{
				cerr<<"[EXPRESSION ERROR] : this is an undefining operator: "<<strOperand<<endl;
#ifdef DEBUG
				cout<<"unknown operator."<<endl;
#endif
				delete pPostfixExpression;
				return NULL;
			}
			// 如果是减号，判断前一个是否操作数，不是的话则将其改为负号,这里有一个特殊情况需要考虑：
			// 如果前一个操作符是右括号，那么右括号之前的可以整体看作一个操作数
			if(OperType == MINUS && LastItemType != OPERAND && LastOperator != RIGHT_BRACKET)
			{
#ifdef DEBUG
				cout<<"change minus to negative."<<endl;
#endif
				OperType = NEGATIVE;
			}
			LastItemType = OPERATOR;     
			LastOperator = OperType;
			// 如果当前是左括号
			if(OperType == LEFT_BRACKET)
			{
#ifdef DEBUG
				cout<<"left bracket."<<endl;
#endif
				stackOperator.push(OperType);
#ifdef DEBUG
				cout<<"top is "<<stackOperator.top()<<"stack size:"<<stackOperator.size()<<endl;
#endif
			}
			// 右括号
			else if(OperType == RIGHT_BRACKET)
			{
#ifdef DEBUG
				cout<<"right bracket."<<endl;
#endif
				//输出栈中的操作符，直到遇到左括号
				bool hasleftbracket = false;
#ifdef DEBUG
				cout<<"top is "<<stackOperator.top()<<"stack size:"<<stackOperator.size()<<endl;
#endif
				while(stackOperator.size() > 0)
				{
					OperatorType OldOper = stackOperator.top();
					stackOperator.pop();
#ifdef DEBUG
					cout<<"pop "<<OldOper<<endl;
#endif
					if(OldOper  == LEFT_BRACKET)
					{
						hasleftbracket = true;
						break;
					}
					// 依次输出操作符
					ExpressionItem ExpItem;
					ExpItem.enum_item_type = OPERATOR; 
					OperatorType* oper = new OperatorType;
					*oper = OldOper;
					ExpItem.item = (void*)oper;
					pPostfixExpression->Push(ExpItem);
#ifdef DEBUG
					cout<<"current expression:";
					pPostfixExpression->print(cout)<<endl;
#endif
				}
				// 有右括号，而无左括号
				if(!hasleftbracket)
				{
					cerr<<"[EXPRESSION ERROR] : one right bracket missed its left bracket."<<endl;
					delete pPostfixExpression;
#ifdef DEBUG
					cout<<"there is right bracket, but no left bracket."<<endl;
#endif
					return NULL;
				}
			}
			else
			{
#ifdef DEBUG
				cout<<"other operator."<<strOperand<<endl;
#endif
				// 当前操作符与栈顶操作符的优先级相比
				if(stackOperator.size() > 0)
				{
					OperatorType OldOper = stackOperator.top();
					// 如果当前的比栈顶的低或相等,则弹出栈顶元素
					while(OldOper != LEFT_BRACKET && mapOperatorPriority[OperType].priority >= mapOperatorPriority[OldOper].priority)
					{
						ExpressionItem ExpItem;
						ExpItem.enum_item_type = OPERATOR; 
						OperatorType* oper = new OperatorType;
						*oper = OldOper;
						ExpItem.item = (void*)oper;
						pPostfixExpression->Push(ExpItem);
#ifdef DEBUG
						cout<<"pop:"<<stackOperator.top()<<endl;
#endif
						stackOperator.pop();
						if(stackOperator.size() == 0)
							break;
						OldOper = stackOperator.top();
					}
				}
				stackOperator.push(OperType);
#ifdef DEBUG
				cout<<"push "<<OperType<<endl;
				cout<<"top is "<<stackOperator.top()<<", stack size:"<<stackOperator.size()<<endl;
#endif
			}
		}
	}
	while(stackOperator.size())
	{
		OperatorType OldOper = stackOperator.top();
		ExpressionItem ExpItem;
		ExpItem.enum_item_type = OPERATOR; 
		OperatorType* oper = new OperatorType;
		*oper = OldOper;
		ExpItem.item = (void*)oper;
		pPostfixExpression->Push(ExpItem);
		stackOperator.pop();
	}
#ifdef DEBUG
	cout<<"current expression:";
	pPostfixExpression->print(cout)<<endl;
#endif

	return pPostfixExpression;
}

//////////////////////////////////////////////////////
// 类CPostfixExpression的方法实现
// 说明：构造函数
CPostfixExpression::CPostfixExpression()
{
	m_unOperands = 0;
	operands = new CTrieTree<uint32_t>;
}

// 说明：拷贝构造
CPostfixExpression::CPostfixExpression(const CPostfixExpression& other)
{
	operands = new CTrieTree<uint32_t>(*(other.operands));
	m_unOperands = other.m_unOperands;
	constant_float = other.constant_float;
	constant_int = other.constant_int;
	for(unsigned int index = 0; index < other.m_vecPostfixExp.size(); index++)
	{
		if(other.m_vecPostfixExp[index].enum_item_type == OPERAND)
		{
			ExpressionItem item;
			item.enum_item_type = OPERAND;
			uint32_t* oper = new uint32_t;
			*oper = *(uint32_t*)(other.m_vecPostfixExp[index].item);
			item.item = (void*)oper;
			m_vecPostfixExp.push_back(item);
		}
		else 
		{
			ExpressionItem item;
			item.enum_item_type = OPERATOR;
			OperatorType* oper = new OperatorType;
			*oper = *(OperatorType*)(other.m_vecPostfixExp[index].item);
			item.item = (void*)oper;
			m_vecPostfixExp.push_back(item);
		}
	}
}

// 说明：赋值操作符
CPostfixExpression& CPostfixExpression::operator=(const CPostfixExpression& other)
{
	if(&other == this) return *this;
	this->m_unOperands = other.m_unOperands;
	if(this->operands) delete this->operands;
	this->operands = new CTrieTree<uint32_t>(*(other.operands));
	
	this->constant_float = other.constant_float;
	this->constant_int = other.constant_int;

	for(unsigned int index = 0; index < this->m_vecPostfixExp.size(); index++)
	{
		if(this->m_vecPostfixExp[index].enum_item_type == OPERAND)
		{
			delete (uint32_t*)(this->m_vecPostfixExp[index].item);
		}
		else 
		{
			delete (OperatorType*)(this->m_vecPostfixExp[index].item);     
		}
	}
	this->m_vecPostfixExp.clear();
	for(unsigned int index = 0; index < other.m_vecPostfixExp.size(); index++)
	{
		if(other.m_vecPostfixExp[index].enum_item_type == OPERAND)
		{
			ExpressionItem item;
			item.enum_item_type = OPERAND;
			uint32_t* oper = new uint32_t;
			*oper = *(uint32_t*)(other.m_vecPostfixExp[index].item);
			item.item = (void*)oper;
			this->m_vecPostfixExp.push_back(item);
		}
		else 
		{
			ExpressionItem item;
			item.enum_item_type = OPERATOR;
			OperatorType* oper = new OperatorType;
			*oper = *(OperatorType*)(other.m_vecPostfixExp[index].item);
			item.item = (void*)oper;
			this->m_vecPostfixExp.push_back(item);
		}
	}

	return *this;
}

// 说明：析构函数
CPostfixExpression::~CPostfixExpression()
{
	for(unsigned int index = 0; index < m_vecPostfixExp.size(); index++)
	{
		if(m_vecPostfixExp[index].enum_item_type == OPERAND)
		{
			delete (uint32_t*)m_vecPostfixExp[index].item;
		}
		else 
		{
			delete (OperatorType*)m_vecPostfixExp[index].item;     
		}
	}
	m_vecPostfixExp.clear();
	if(operands)
		delete operands;
}

uint32_t CPostfixExpression::GetOperandCode(const char* operand_variable)
{
	if(operands == NULL)
		operands = new CTrieTree<uint32_t>;
	uint32_t count = 0;
	int code = operands->Add(operand_variable, count)->TheCode();
	if(code <= 0)
		return 0;

	return code;
}

uint32_t CPostfixExpression::AddConstant(const float value)
{
	uint32_t sequence = constant_float.size();
	string variable = "#" + CTypeTool<uint32_t>::ToStr(sequence);
	constant_float[variable] = value;
	constant_int[variable] = (int32_t)value;
#ifdef DEBUG
	cerr<<"[EXPRESSION INFO] add variable : name = "<<variable<<", value = "<<value<<", const size = "<<constant_float.size()<<", "<<constant_int.size()<<", this="<<this<<endl;
#endif
	return GetOperandCode(variable.c_str());
}

// 说明：加入一个后缀表达式元素
void CPostfixExpression::Push(ExpressionItem& ExpItem)
{
	if(ExpItem.enum_item_type == OPERAND)
	{
		m_unOperands++;
		uint32_t operand = *((uint32_t*)ExpItem.item);
		operands->GetValue((int)operand)++;
	}
	m_vecPostfixExp.push_back(ExpItem);
}

// 说明：获取操作数
uint32_t CPostfixExpression::GetOperands(map<string, uint32_t>& mapOperands)
{
	for(int code = 1; code <= operands->GetKeyCount(); code++)
	{
		mapOperands[operands->GetKey(code)] = operands->GetValue(code);
	}
	return mapOperands.size();
}

// 说明：输入表达式中各个操作数的值，计算结果
bool CPostfixExpression::Operate(const map<string, int32_t>& mapOperandValue, int32_t& Result)
{
	if(operands->GetKeyCount() > mapOperandValue.size() + constant_int.size())
		return false;
	stack<uint32_t> stackOperand;//操作数栈
	Result = 0;
	std::string variable;
	map<string, int32_t>::const_iterator iter;
	vector<int32_t> operand_values;
	for(unsigned int index = 0; index < m_vecPostfixExp.size(); index++)
	{
		if(m_vecPostfixExp[index].enum_item_type == OPERAND)
		{
			variable = operands->GetKey(*((uint32_t*)m_vecPostfixExp[index].item));
			iter = mapOperandValue.find(variable);
			if(iter == mapOperandValue.end())
			{
				iter = constant_int.find(variable);
				if(iter == constant_int.end())
					return false;
			}
#ifdef DEBUG
			cout<<"push operand:"<<iter->second<<endl;
#endif
			stackOperand.push(iter->second);
		}
		else
		{
			// 从栈中弹出需要的操作数个数
			OperatorType Oper = *(OperatorType*)(m_vecPostfixExp[index].item);
			map<OperatorType, OperatorParameter>::iterator iterOper = mapOperatorPriority.find(Oper);
			if(iterOper == mapOperatorPriority.end())
				return false;

			// 栈中的操作数小于操作符所需的操作数
			if(stackOperand.size() < iterOper->second.operands)
				return false;
			// 依次弹出所需的操作数,先弹出的操作数在最右边
			// 为了简化，本解析器暂时不处理三个及三个以上的操作数
			operand_values.clear();
#ifdef DEBUG
			cout<<"operate : "<<Oper<<" on:";
#endif
			while(stackOperand.size() > 0 && operand_values.size() < iterOper->second.operands)
			{
#ifdef DEBUG
				cout<<stackOperand.top()<<",";
#endif
				operand_values.push_back(stackOperand.top());
				stackOperand.pop();
			}
#ifdef DEBUG
			cout<<endl;
#endif
			uint32_t result = DoOperating(Oper, operand_values);
#ifdef DEBUG
			cout<<"result = "<<result<<endl;
#endif
			stackOperand.push(result);
		}
	}

	// 正确运算结果是最后留下在栈中的那个值
	if(stackOperand.size() != 1)
		return false;

	Result = stackOperand.top();
	return true;
}

bool CPostfixExpression::Operate(const map<string, float>& mapOperandValue, float& Result)
{
	if(operands->GetKeyCount() > mapOperandValue.size() + constant_float.size())
		return false;
	stack<float> stackOperand;//操作数
	Result = 0;
	string variable;
	map<string, float>::const_iterator iter;
	vector<float> operand_values;
	for(unsigned int index = 0; index < m_vecPostfixExp.size(); index++)
	{
		if(m_vecPostfixExp[index].enum_item_type == OPERAND)
		{
			variable = operands->GetKey(*((uint32_t*)m_vecPostfixExp[index].item));
			iter = mapOperandValue.find(variable);
			if(iter == mapOperandValue.end())
			{
				iter = constant_float.find(variable);
				if(iter == constant_float.end())
				{
#ifdef DEBUG
					cout<<"[EXPRESSION ERROR] find operand's value failed [operand = "<<this<<","<<constant_float.size()<<","<<constant_int.size()<<", "<<variable<<"]"<<endl;
#endif
					return false;
				}
			}
#ifdef DEBUG
			cout<<"push operand:"<<iter->second<<endl;
#endif
			stackOperand.push(iter->second);
		}
		else
		{
			// 从栈中弹出需要的操作数个数
			OperatorType Oper = *(OperatorType*)(m_vecPostfixExp[index].item);
			map<OperatorType, OperatorParameter>::iterator iterOper = mapOperatorPriority.find(Oper);
			if(iterOper == mapOperatorPriority.end())
			{
#ifdef DEBUG
				cout<<"[EXPRESSION ERROR] "<<Oper<<": unknown operator."<<endl;
#endif
				return false;
			}
#ifdef DEBUG
			cout<<"Oper : "<<GetOperatorName(Oper)<<" need "<<iterOper->second.operands<<" operands."<<endl;
#endif

			// 栈中的操作数小于操作符所需的操作数
			if(stackOperand.size() < iterOper->second.operands)
			{
#ifdef DEBUG
				cout<<"[EXPRESSION ERROR] operator needs more operands than "<<stackOperand.size()<<endl;
#endif
				return false;
			}
			// 依次弹出所需的操作数,先弹出的操作数在最右边
			// 为了简化，本解析器暂时不处理三个及三个以上的操作数
			operand_values.clear();
#ifdef DEBUG
			cout<<"operate : "<<Oper<<" on:";
#endif
			while(stackOperand.size() > 0 && operand_values.size() < iterOper->second.operands)
			{
#ifdef DEBUG
				cout<<stackOperand.top()<<",";
#endif
				operand_values.push_back(stackOperand.top());
				stackOperand.pop();
			}
			float result = DoOperating(Oper, operand_values);
#ifdef DEBUG
			cout<<"result = "<<result<<endl;
#endif
			stackOperand.push(result);
		}
	}

	// 正确运算结果是最后留下在栈中的那个值
	if(stackOperand.size() != 1)
	{
#ifdef DEBUG
		cout<<"[EXPRESSION ERROR] En? It looks like something wrong happened."<<endl;
#endif
		return false;
	}

	Result = stackOperand.top();

	return true;
}

bool CPostfixExpression::IsActive()
{
	return operands->GetKeyCount() > 0; 
}

//void CPostfixExpression::GetOperators(map<OperatorType, pair<OperatorParameter, uint32_t> >& operators)
//{
//}
