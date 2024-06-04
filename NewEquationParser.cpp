#include "NewEquationParser.h"



string nonVariableChars = "().0123456789+-/*^e";

//This is the function that will take an equation given by the user, and prepare it to be evaluated at any value
NewEquationParser::NewEquationParser(string equationString) {
    translate(equationString);
}

NewEquationParser::~NewEquationParser() {
    delete equationRoot;
}

double NewEquationParser::evaluate(double x, double y) {
    return equationRoot->evaluate(x, y);
}

void NewEquationParser::translate(string _equationString) {
    equationRoot = new EquationPiece(_equationString);
}
