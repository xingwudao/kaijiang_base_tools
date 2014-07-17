#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "./algorithm/expression.h"
using namespace std;
using namespace boost::algorithm;
using namespace kaijiang;
using boost::lexical_cast;

int main(int c, char** v)
{
	string line1, line2;
	ExpressionParser* parser = new ExpressionParser; 
	Expression* expression = NULL;
    string input;
    cout << "this is a expression work demo, I accept expression like this:" << endl;
    cout << "$a + $b * 4" << endl;
    cout << "please input expression, (input exit to exit):" << endl;
    while(getline(cin, input) && input != "exit")
    {
        expression = parser->Parse(input);
        if(expression == NULL)
        {
            cout << "expression systax error, please go on inputing or exit." << endl;
            continue;
        }
        cout << "please input variables' value like this: a = 3, b = 5" << endl;
        getline(cin, input);
        map<string, float> operand;
        vector<string> splites;
        split(splites, input, is_any_of(","), token_compress_on);
        for(auto &s : splites)
        {
            size_t i = s.find("=");
            if(i == s.npos) continue;
            string v = s.substr(0, i);
            trim(v);
            string vv = s.substr(i + 1, s.size() - i - 1);
            trim(vv);
            operand[v] = lexical_cast<float>(vv);
        }
        float result = 0.0;
        if(!expression->Operate(operand, result))
        {
            cout << "I think you maybe miss some variables' value, so I can't get result." << endl;
            delete expression;
            cout << "please input expression, (input exit to exit):" << endl;
            continue;
        }
        cout << "result = " << result << endl;
        cout << "please input expression, (input exit to exit):" << endl;
        delete expression;
    }

    delete parser;
    return 0;
}
