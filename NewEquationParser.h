#ifndef EQUATION2
#define EQUATION2
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

enum EquationPieceTypes {
    VARIABLE = 1,
    EQUATION = 2,
    NUMBER = 3,
};

struct EquationPiece {
    int EquationPieceTypeID;
    int operation;
    vector<EquationPiece> EquationPieces;
    double evaluate() {

    };
    char operation;
};

class NewEquationParser {
    public:
        NewEquationParser(string equationString);
        ~NewEquationParser();
        void evaluate(double variableValues[]);
    private:
        void translate(string equationString);
        vector<EquationPiece> equationPieces;
};


#endif EQUATION2
