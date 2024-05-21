#include "NewEquationParser.h"

//This is the function that will take an equation given by the user, and prepare it to be evaluated at any value
NewEquationParser::NewEquationParser(string equationString) {
    translate(equationString);
}

void NewEquationParser::evaluate(double variableValues[]) {

}

void NewEquationParser::translate(string _equationString) {
    size_t length = _equationString.size();
    //unwrap equation of parentheses
    while(_equationString[0] == '(' && _equationString[length - 1] == ')') {
        _equationString = _equationString.substr(1, length - 2);
    }
}
