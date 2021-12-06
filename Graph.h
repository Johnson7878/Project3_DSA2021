#pragma once
#include <vector>
#include <string>

using namespace std;

struct Vertex{

    string name;
    float price;
    float volume; //need volume?
    bool organic; //is organic, do we need this?

    //attribute for time???

    Vertex(string _name, float _price, float _volume, bool _organic){
        name = _name; price = _price; volume = _volume; organic = _organic;
    }

};

class Graph{

    //probably should also have a 2-D vector for distances (look-up table )

    int gridSize = 200000; //200k

    vector <vector <string> > distMatrix;

    vector < vector<Vertex> > adjMatrix;



public:

    Graph( vector<vector<string>> &_distMatrix); //need to include size maybe for the




};


Graph::Graph(vector <vector <string> > &_distMatrix){

    adjMatrix =  vector < vector<Vertex> >( gridSize, vector<Vertex> (gridSize) );  ///200k by 200k (maybe change)

    distMatrix = _distMatrix;

}

