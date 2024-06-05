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

    vector<int> getOperationIndexes(string equationString) {
        vector<int> plusOrMinusIndexes;
        vector<int> multOrDivIndexes;
        vector<int> powerIndexes;
        int parenthesesCount = 0;
        int index = 0;
        //first check if there are operations
        for (char c : equationString) {
            if (c == '(') {
                parenthesesCount++;
            } else if (c == ')') {
                parenthesesCount--;
            } else if (parenthesesCount == 0) {
                if (c == '+') {
                    plusOrMinusIndexes.push_back(index);
                } else if (c == '-') {
                    plusOrMinusIndexes.push_back(index);
                } else if (c == '*') {
                    multOrDivIndexes.push_back(index);
                } else if (c == '/') {
                    multOrDivIndexes.push_back(index);
                } else if (c == '^' && powerIndexes.size() == 0) {
                    powerIndexes.push_back(index);
                }
            }
            index++;
        }
        if (plusOrMinusIndexes.size() > 0) {
            return plusOrMinusIndexes;
        } else if (multOrDivIndexes.size() > 0) {
            return multOrDivIndexes;
        } else if (powerIndexes.size() > 0) {
            return powerIndexes;
        }
        //return -1 if no operations were found.
        return plusOrMinusIndexes;
    }
public:
    ~EquationPiece() {
        for (EquationPiece* piece : equationPieces) {
            delete piece;
        }
        equationPieces.clear();
        operations.clear();
        funcList.clear();
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
        while (
            (
                equationString.substr(0, 4) == "sin(" ||
                equationString.substr(0, 4) == "cos(" ||
                equationString.substr(0, 4) == "tan("
                )
            && equationString[length - 1 == ')']
            ) {
            funcList.emplace(funcList.begin(), equationString.substr(0, 3));
            equationString = equationString.substr(4, length - 5);

        }

        vector<int> operationIndexes = getOperationIndexes(equationString);

        if (operationIndexes.size() == 0) {
            try {
                stod(equationString);
                EquationPieceTypeID = NUMBER;
            } catch (const std::invalid_argument& invalid_argument) {
                EquationPieceTypeID = VARIABLE;
            }

        } else {
            int start = 0;
            int index = 0;
            operations.push_back('+');
            for (int i : operationIndexes) {
                operations.push_back(equationString[operationIndexes[index]]);

                equationPieces.push_back(new EquationPiece(equationString.substr(start, i - start)));
                index++;
                start = i + 1;
            }
            equationPieces.push_back(new EquationPiece(equationString.substr(start)));

            EquationPieceTypeID = SOLVABLEEQUATION;

        }
    }
    int EquationPieceTypeID;
    vector<EquationPiece*> equationPieces;
    string equationString;
    double evaluate(double x, double y) {
        double result = 0;
        if (EquationPieceTypeID == VARIABLE) {
            if (equationString[0] == 'e') {
                result = exp(1);
            }
            result = equationString[0] == 'x' ? x : y;
        } else if (EquationPieceTypeID == SOLVABLEEQUATION) {
            int index = 0;
            for (EquationPiece* piece : equationPieces) {
                if (operations[index] == '+') {
                    result += piece->evaluate(x, y);
                } else if (operations[index] == '-') {
                    result -= piece->evaluate(x, y);
                } else if (operations[index] == '*') {
                    result *= piece->evaluate(x, y);
                } else if (operations[index] == '/') {
                    result /= piece->evaluate(x, y);
                } if (operations[index] == '^') {
                    result = pow(result, piece->evaluate(x, y));
                }
                index++;
            }

        } else if (EquationPieceTypeID == NUMBER) {
            result = stod(equationString);
        }
        for (string function : funcList) {
            if (function == "sin") {
                result = sin(result);
            } else if (function == "cos") {
                result = cos(result);
            } else if (function == "tan") {
                result = tan(result);
            }
        }
        return result;

    };

    vector<char> operations;
    vector<string> funcList;
    int VARIABLE = 1;
    int SOLVABLEEQUATION = 2;
    int NUMBER = 3;

};

class NewEquationParser {
public:
    NewEquationParser(string equationString);
    ~NewEquationParser();
    double evaluate(double x, double y);
private:
    void translate(string equationString);
    EquationPiece* equationRoot;
};


#endif
