#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include "Graph.h"
#include "3DGraph.h"

using namespace std;

int main()
{
    int choice;
    cout << "1. 3D graph\n2. 2D graph" << endl;
    cin >> choice;
    if(choice == 1) {
    ThreeDGraph threeDimensionalGraph;
    threeDimensionalGraph.init();
    } else {
        Graph graph;
        graph.init();
    }

    /*NewEquationParser parser("x-y+3");
    unordered_map<char, double> values;
    values['x'] = 2;
    values['y'] = 4.5;
    cout << parser.evaluate(values) << endl;*/
    
    return 0;
}