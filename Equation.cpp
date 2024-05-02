#include "Equation.h"
#include <vector>
#include <set>
#include <iostream>
#include <math.h>

using namespace std;

void Equation::translate(string equationString)
{
    int i = 0;
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
            // this is a float
            if (floatOrVariable)
            {
                equationBit bit = {
                    true,
                    NULL,
                    stof(numberOrVariable),
                    equationString[i]};
                equationPieces.push_back(bit);
                numberOrVariable = "";
            }
            else
            {
                // this is a variable
                equationBit bit = {
                    false,
                    numberOrVariable[0],
                    NULL,
                    (char)equationString[i]};
                equationPieces.push_back(bit);
                numberOrVariable = "";
            }
        }
        else
        { // here we add the variable string (one char) to numberOrVariable
            floatOrVariable = false;
            numberOrVariable += equationString[i];
            variables.insert(equationString[i]);
        }
        i++;
    }

    if (floatOrVariable)
    {
        equationBit bit = {
            floatOrVariable,
            NULL,
            stof(numberOrVariable),
            NULL,
        };
        equationPieces.push_back(bit);
    }
    else
    {
        equationBit bit = {
            false,
            numberOrVariable[0],
            NULL,
            NULL};
        equationPieces.push_back(bit);
    }
}

float Equation::evaluate(vector<float> variable_values)
{
    vector<char> varList(variables.begin(), variables.end());
    /*for(int i = 0; i < equationPieces.size() - 1; i++) {
        if(equationPieces[i].numberOrVeariable) {
            cout << "number: " << equationPieces[i].number << " operation: " << (equationPieces[i].operation == NULL ? ' ' : equationPieces[i].operation) << " " << endl;
        }
    }*/
    for (int j = 0; j < equationPieces.size(); j++)
    {
        if (!equationPieces[j].numberOrVeariable)
        {
            for (int i = 0; i < varList.size(); i++)
            {
                if (equationPieces[j].variable == varList[i])
                {
                    equationPieces[j].number = variable_values[i];
                    cout << "replacing " << equationPieces[j].variable <<" with " << variable_values[i] << endl;
                }
            }
        }
    }
    // check for powers
    for (int i = equationPieces.size() - 2; i >= 0; i--)
    {
        if (equationPieces[i].operation == '^')
        {
            // cout << "in power" << endl;
            equationPieces[i].number = pow(equationPieces[i].number, equationPieces[i + 1].number);
            equationPieces[i].operation = equationPieces[i + 1].operation;
            equationPieces.erase(equationPieces.begin() + i + 1);
            // iterate();
        }
    }

    // check for multiplication
    for (int i = equationPieces.size() - 2; i >= 0; i--)
    {
        if (equationPieces[i].operation == '*')
        {
            // cout << "in multiply" << endl;
            equationPieces[i].number = equationPieces[i].number * equationPieces[i + 1].number;
            equationPieces[i].operation = equationPieces[i + 1].operation;
            equationPieces.erase(equationPieces.begin() + i + 1);
            // iterate();
        }
    }
    // check for division
    for (int i = equationPieces.size() - 2; i >= 0; i--)
    {
        if (equationPieces[i].operation == '/')
        {
            // cout << "in divide" << endl;
            equationPieces[i].number = equationPieces[i].number / equationPieces[i + 1].number;
            equationPieces[i].operation = equationPieces[i + 1].operation;
            equationPieces.erase(equationPieces.begin() + i + 1);
            // iterate();
        }
    }
    // check for addition
    for (int i = equationPieces.size() - 2; i >= 0; i--)
    {
        if (equationPieces[i].operation == '+')
        {
            // cout << "in add" << endl;
            equationPieces[i].number = equationPieces[i].number + equationPieces[i + 1].number;
            equationPieces[i].operation = equationPieces[i + 1].operation;
            equationPieces.erase(equationPieces.begin() + i + 1);
            // iterate();
        }
    }
    // check for subtraction
    for (int i = equationPieces.size() - 2; i >= 0; i--)
    {
        if (equationPieces[i].operation == '-')
        {
            // cout << "in subtract" << endl;
            equationPieces[i].number = equationPieces[i].number - equationPieces[i + 1].number;
            equationPieces[i].operation = equationPieces[i + 1].operation;
            equationPieces.erase(equationPieces.begin() + i + 1);
            // iterate();
        }
    }

    return equationPieces[0].number;
}

void Equation::iterate()
{
    for (int i = 0; i < equationPieces.size() - 1; i++)
    {
        if (equationPieces[i].numberOrVeariable)
        {
            cout << "number: " << equationPieces[i].number << " operation: " << (equationPieces[i].operation == NULL ? ' ' : equationPieces[i].operation) << " " << endl;
        }
    }
}