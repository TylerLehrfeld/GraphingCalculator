#ifndef EQUATION
#define EQUATION

#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

class Equation {
    private:
    
    struct equationBit {
        bool numberOrVariable;
        char variable;
        float number;
        char operation;
        bool negative;
        string innerFunction = "";
    };
    int parameterCount;
    vector<equationBit> equationPieces;
    set<char> variables;
    
    public:
    void translate(string equationString);
    float evaluate(vector<float>);

    void iterate();
    ~Equation();
    Equation();
};

#endif