#include <iostream>
#include <sstream>
using namespace std;

#include "./algorithm/expression.h"

int main(int c, char** v)
{
	string line1, line2;
	kaijiang::CExpressionParser* parser = new kaijiang::CExpressionParser; 
	kaijiang::CPostfixExpression* expression = NULL;
	while(getline(cin, line1) && getline(cin, line2))
	{
		expression = parser->ParseNifixExp(line1);
		if(expression == NULL)
		{
			cerr<<"parse expression error."<<endl;
			continue;
		}
		stringstream stream;
		stream.str(line2);
		string variable;
		float value;
		map<string, float> operand;
		while(stream>>variable>>value)
		{
			cout<<variable<<" = "<<value<<endl;
			operand[variable] = value;
		}
		float result = 0;
		cout<<"[INFO] parse successfully!! the expression: ";
		expression->print(cout);
		cout<<endl;
		if(!expression->Operate(operand, result))
		{
			cerr<<"operate error."<<endl;
			delete expression;
			continue;
		}
		cerr<<"result = "<<result<<endl;
		delete expression;
	}

	return 0;
}
