#include "NewEquationParser.h"



string nonVariableChars = "().0123456789+-/*^e";

//This is the function that will take an equation given by the user, and prepare it to be evaluated at any value
NewEquationParser::NewEquationParser(string equationString) {
    translate(equationString);
}

NewEquationParser::~NewEquationParser() {
    delete equationRoot;
}

double NewEquationParser::evaluate(unordered_map<char, double> &varMap) {
    for(auto i : varMap) {
        variableValuesMap[i.first] = i.second;
    }
    return equationRoot->evaluate(variableValuesMap);
}

void NewEquationParser::translate(string _equationString) {
    equationRoot = new EquationPiece(_equationString);
    variableValuesMap['e'] = exp(1);
    for(char c : _equationString) {
        size_t ind = nonVariableChars.find(c);
        if(ind != std::string::npos) {
            if(ind + 3 < _equationString.size() && ((c == 's' && _equationString.substr(ind, 3) != "sin") || (c == 'c' && _equationString.substr(ind, 3) != "cos"))) {
                ind += 2;
            } else {
                variableValuesMap[c] = 0;
            }
        }
    }
    
}
