#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include "Graph.h"

using namespace std;

int main()
{
    string inputEquation;
    cout << "Enter your equation here" << endl;
    cin >> inputEquation;
    cout << inputEquation << endl;
    Graph graph;
    graph.graphLine(inputEquation);
    graph.init();
    
    return 0;
}