#include "Equation.h"
#include <vector>
#include <set>
#include <iostream>
#include <math.h>

using namespace std;

Equation::~Equation()
{
    equationPieces.clear();
};

Equation::Equation()
{
}

void Equation::translate(string equationString)
{

    if (equationString[equationString.size() - 1] == ')')
    {
        // this is to make it so there is always an operation after parentheses.
        equationString += "+0";
    }
    bool negative = false;

    size_t i = 0;
    string numberOrVariable = "";
    bool floatOrVariable = false;
    while (
        i < equationString.length())
    {
        // we first check if we are looking at a number
        if (
            equationString[i] == '0' ||
            equationString[i] == '1' ||
            equationString[i] == '2' ||
            equationString[i] == '3' ||
            equationString[i] == '4' ||
            equationString[i] == '5' ||
            equationString[i] == '6' ||
            equationString[i] == '7' ||
            equationString[i] == '8' ||
            equationString[i] == '9' ||
            equationString[i] == '.')
        {
            // if we are looking at a number, we add it to the numberorvariable feild
            floatOrVariable = true;
            numberOrVariable += equationString[i];
        }
        else if (
            // if we get an operation charachter, we can
            equationString[i] == '+' ||
            equationString[i] == '-' ||
            equationString[i] == '/' ||
            equationString[i] == '*' ||
            equationString[i] == '^')
        {
            if (numberOrVariable == "" && equationString[i] == '-')
            {
                negative = true;
            }
            else
                // this is a float
                if (floatOrVariable)
                {
                    equationBit bit = {
                        true,
                        '\0', // no variable charachter
                        stof(numberOrVariable),
                        equationString[i],
                        negative};
                    if (negative)
                    {
                        // bit.number = bit.number * -1;
                        negative = false;
                    }
                    equationPieces.push_back(bit);
                    numberOrVariable = "";
                }
                else
                {
                    // this is a variable
                    equationBit bit = {
                        false,
                        numberOrVariable[0],
                        0, // no number yet
                        equationString[i],
                        negative};

                    if (negative)
                    {
                        // bit.number = bit.number * -1;
                        negative = false;
                    }
                    equationPieces.push_back(bit);
                    numberOrVariable = "";
                }
        }
        // we are looking at a section of the equation that is in parentheses, so we need to evaluate this section before proceeding
        else if (equationString[i] == '(')
        {
            int j = i + 1;
            int openCount = 1;
            while (openCount != 0)
            {
                if (equationString[j] == '(')
                {
                    openCount++;
                }
                if (equationString[j] == ')')
                {
                    openCount--;
                }
                if (openCount != 0)
                {
                    j++;
                }
            }
            equationBit bit = {
                NULL,        // not a number or a variable
                (char)NULL,  // no variable
                (float)NULL, // no number
                equationString[j + 1],
                negative,
                equationString.substr(i + 1, j - i - 1)};
            if (negative)
            {
                negative = false;
            }
            equationPieces.push_back(bit);
            // skip past parentheses and operation
            i = j + 1;
        }

        else
        {
            if (i + 2 < equationString.length() && (equationString.substr(i, 3) == "sin" || equationString.substr(i, 3) == "cos"))
            {
                int j = i + 4;
                int openCount = 1;
                while (openCount != 0)
                {
                    if (equationString[j] == '(')
                    {
                        openCount++;
                    }
                    if (equationString[j] == ')')
                    {
                        openCount--;
                    }
                    if (openCount != 0)
                    {
                        j++;
                    }
                }
                equationBit bit = {
                    NULL,        // not a number or a variable
                    (char)NULL,  // no variable
                    (float)NULL, // no number
                    equationString[j + 1],
                    negative,
                    equationString.substr(i, j - i + 1)};
                if (negative)
                {
                    negative = false;
                }
                equationPieces.push_back(bit);
                // skip past parentheses and operation
                i = j + 1;
            }
            else
            {
                // here we add the variable string (one char) to numberOrVariable
                floatOrVariable = false;
                numberOrVariable += equationString[i];
                variables.insert(equationString[i]);
            }
        }
        i++;
    }

    if (floatOrVariable)
    {
        equationBit bit = {
            floatOrVariable,
            '\0', // no variable for last number
            stof(numberOrVariable),
            '\0', // no operation for last number,
            negative,
        };
        if (negative)
        {
            negative = false;
        }
        equationPieces.push_back(bit);
    }
    else
    {
        equationBit bit = {
            false,
            numberOrVariable[0],
            (float)NULL, // no number for last variable
            (char)NULL,  // no operation for last variable,
            negative};
        negative = false;
        equationPieces.push_back(bit);
    }
}

float Equation::evaluate(vector<float> variable_values)
{
    vector<char> varList(variables.begin(), variables.end());
    for (size_t j = 0; j < equationPieces.size(); j++)
    {
        if (!equationPieces[j].numberOrVariable)
        {
            for (size_t i = 0; i < varList.size(); i++)
            {
                if (equationPieces[j].variable == varList[i])
                {
                    equationPieces[j].number = variable_values[i];
                }
            }
        }
        if (equationPieces[j].negative)
        {
            equationPieces[j].number *= -1;
        }
    }
    // check for parentheses
    for (int i = equationPieces.size() - 1; i >= 0; i--)
    {
        if (equationPieces[i].innerFunction != "")
        {
            float result = 0;
            if (equationPieces[i].innerFunction.substr(0, 3) == "sin")
            {
                Equation thisEq;
                thisEq.translate(equationPieces[i].innerFunction.substr(4, equationPieces[i].innerFunction.length() - 5));
                result = sin(thisEq.evaluate(variable_values));
            }
            else if (equationPieces[i].innerFunction.substr(0, 3) == "cos")
            {
                Equation thisEq;
                thisEq.translate(equationPieces[i].innerFunction.substr(4, equationPieces[i].innerFunction.length() - 5));
                result = cos(thisEq.evaluate(variable_values));
            }
            else
            {
                Equation thisEq;
                thisEq.translate(equationPieces[i].innerFunction);
                result = thisEq.evaluate(variable_values);
            }
            equationPieces[i].number = result;         // set the number to the evaluated function
            equationPieces[i].numberOrVariable = true; // indicate that this is a number
            equationPieces[i].innerFunction = "";      // set the inner function as "" to indicate we are no longer using it
            // operation should already be defined
        }
    }

    // check for powers
    for (int i = equationPieces.size() - 2; i >= 0; i--)
    {
        if (equationPieces[i].operation == '^')
        {
            equationPieces[i].number = pow(equationPieces[i].number, equationPieces[i + 1].number);
            equationPieces[i].operation = equationPieces[i + 1].operation;
            equationPieces.erase(equationPieces.begin() + i + 1);
        }
    }

    // check for multiplication
    for (int i = equationPieces.size() - 2; i >= 0; i--)
    {
        if (equationPieces[i].operation == '*')
        {
            equationPieces[i].number = equationPieces[i].number * equationPieces[i + 1].number;
            equationPieces[i].operation = equationPieces[i + 1].operation;
            equationPieces.erase(equationPieces.begin() + i + 1);
        }
    }
    // check for division
    for (int i = equationPieces.size() - 2; i >= 0; i--)
    {
        if (equationPieces[i].operation == '/')
        {
            equationPieces[i].number = equationPieces[i].number / equationPieces[i + 1].number;
            equationPieces[i].operation = equationPieces[i + 1].operation;
            equationPieces.erase(equationPieces.begin() + i + 1);
        }
    }
    // check for addition
    for (int i = equationPieces.size() - 2; i >= 0; i--)
    {
        if (equationPieces[i].operation == '+')
        {
            equationPieces[i].number = equationPieces[i].number + equationPieces[i + 1].number;
            equationPieces[i].operation = equationPieces[i + 1].operation;
            equationPieces.erase(equationPieces.begin() + i + 1);
        }
    }
    // check for subtraction
    for (int i = equationPieces.size() - 2; i >= 0; i--)
    {
        if (equationPieces[i].operation == '-')
        {
            equationPieces[i].number = equationPieces[i].number - equationPieces[i + 1].number;
            equationPieces[i].operation = equationPieces[i + 1].operation;
            equationPieces.erase(equationPieces.begin() + i + 1);
        }
    }

    return equationPieces[0].number;
}