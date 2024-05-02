#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

class Equation {
    private:
    enum operations {
        multiply = 0,
        divide = 1,
        add = 2,
        subtract = 3,
        power = 4
    };

    struct equationBit {
        bool numberOrVeariable;
        char variable;
        float number;
        char operation;
    };
    int parameterCount;
    vector<equationBit> equationPieces;
    set<char> variables;
    
    public:
    void translate(string equationString);
    float evaluate(vector<float>);

    void iterate();
};