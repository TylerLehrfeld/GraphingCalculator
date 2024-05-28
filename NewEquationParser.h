#ifndef EQUATION2
#define EQUATION2
#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <math.h>
#include <stdexcept>
using std::string, std::min, std::unordered_map, std::vector;


class EquationPiece {

    int getOperationIndex(string equationString) {
        size_t length = equationString.size();
        int parenthesesCount = 0;
        int index = 0;
        int divIndex = length;
        int multIndex = length;
        int powIndex = length;
        //first check if there are operations
        for (char c : equationString) {
            if (c == '(') {
                parenthesesCount++;
            } else if (c == ')') {
                parenthesesCount--;
            } else if (parenthesesCount == 0) {
                if (c == '+') {
                    return index;
                } else if (c == '-') {
                    return index;
                } else if (c == '*') {
                    if (index < multIndex) {
                        multIndex = index;
                    }
                } else if (c == '/') {
                    if (index < divIndex) {
                        divIndex = index;
                    }
                } else if (c == '^') {
                    if (index < powIndex) {
                        powIndex = index;
                    }
                }
            }
            index++;
        }
        //return -1 if no operations were found.
        if ((size_t)min(multIndex, divIndex) != length) {
            return min(multIndex, divIndex);
        }
        return (size_t)powIndex == length ? -1 : powIndex;
    }
public:
    ~EquationPiece() {
        delete leftPiece;
        delete rightPiece;
    }
    EquationPiece(string _equationString) {

        equationString = _equationString;
        size_t length = equationString.size();
        if (length == 0) {
            equationString = "0";
            EquationPieceTypeID = NUMBER;
            return;
        }
        //unwrap equation of parentheses
        while (equationString[0] == '(' && equationString[length - 1] == ')') {
            equationString = equationString.substr(1, length - 2);
        }

        int operationIndex = getOperationIndex(equationString);

        if (operationIndex == -1) {
            try {
                stod(equationString);
                EquationPieceTypeID = NUMBER;
            } catch (const std::invalid_argument& ia) {
                EquationPieceTypeID = VARIABLE;
            }

        } else {
            operation = equationString[operationIndex];
            leftPiece = new EquationPiece(equationString.substr(0, operationIndex));
            rightPiece = new EquationPiece(equationString.substr(operationIndex + 1));
            EquationPieceTypeID = SOLVABLEEQUATION;

        }
    }
    int EquationPieceTypeID;
    EquationPiece* leftPiece;
    EquationPiece* rightPiece;
    string equationString;
    double evaluate(unordered_map<char, double>& variableMap) {
        if (EquationPieceTypeID == VARIABLE) {
            return variableMap[equationString[0]];
        } else if (EquationPieceTypeID == SOLVABLEEQUATION) {
            if (operation == '+') {
                return leftPiece->evaluate(variableMap) + rightPiece->evaluate(variableMap);
            } else if (operation == '-') {

                return leftPiece->evaluate(variableMap) - rightPiece->evaluate(variableMap);
            } else if (operation == '*') {

                return leftPiece->evaluate(variableMap) * rightPiece->evaluate(variableMap);
            } else if (operation == '/') {

                return leftPiece->evaluate(variableMap) / rightPiece->evaluate(variableMap);
            } else if (operation == '^') {
                return pow(leftPiece->evaluate(variableMap), rightPiece->evaluate(variableMap));
            }
        } else if (EquationPieceTypeID == NUMBER) {
            return stod(equationString);
        }
        //error
        return -1;
    };

    char operation;

    int VARIABLE = 1;
    int SOLVABLEEQUATION = 2;
    int NUMBER = 3;

};

class NewEquationParser {
public:
    NewEquationParser(string equationString);
    ~NewEquationParser();
    double evaluate(unordered_map<char, double>& varMap);
private:
    void translate(string equationString);
    EquationPiece* equationRoot;
    unordered_map<char, double> variableValuesMap;
};


#endif
