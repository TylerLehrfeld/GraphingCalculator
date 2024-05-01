#include <string>
#include <vector>

using namespace std;

class Equation {
    public:
    void translate(string equationString);
    float evaluate(vector<float> &parameters);

    private:
    enum operations {
        multiply = '*',
        divide = '/',
        add = '+',
        subtract = '-',
        power = '^'
    };
    int parameterCount;
    vector<int> operationsList;
    

};